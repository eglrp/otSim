/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Input.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Input class using OIS library wrapper around Microsoft DirectInput API.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Input.h"
#ifdef _DEBUG
#include <iostream>
#endif
#include <string>
#include <sstream>
#include <queue>
#include <map>

#include "OIS.h"

//#include "Log.h"
//#include "Utilities.h"
#include "InputMapper.h"
#include "CommandManager.h"
#include "Command.h"
#include "ActionCommands.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otInput {

HWND FindMyTopMostWindow()
{
	DWORD dwProcID = GetCurrentProcessId();
	HWND hWnd = GetTopWindow(GetDesktopWindow());
	while (hWnd)
	{
		DWORD dwWndProcID = 0;
		GetWindowThreadProcessId(hWnd, &dwWndProcID);
		if (dwWndProcID == dwProcID)
			return hWnd;
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return NULL;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

inline float getAxisCurve(float input, float curvePower, float deadzone, float saturationXLow, float saturationXHigh, float saturationYLow, float saturationYHigh, float offsetX, float trimY)
{
	deadzone = deadzone < 0.0f ? 0.0f : deadzone > 1.0f ? 1.0f : deadzone;

	if (deadzone >= 0.99999f)
		return trimY;

	saturationXLow = saturationXLow < (-offsetX + deadzone + 0.01f) ? (-offsetX + deadzone + 0.01f) : saturationXLow > 1.0f ? 1.0f : saturationXLow;
	saturationXHigh = saturationXHigh < (offsetX + deadzone + 0.01f) ? (offsetX + deadzone + 0.01f) : saturationXHigh > 1.0f ? 1.0f : saturationXHigh;

	trimY = trimY < -saturationYLow ? -saturationYLow : trimY > saturationYHigh ? saturationYHigh : trimY;

	if (input <= -saturationXLow)
		return -saturationYLow;
	else if (input >= saturationXHigh)
		return saturationYHigh;

	if (input < (offsetX - deadzone))
	{
		float b = (saturationYLow + trimY) / pow(abs(-saturationXLow + deadzone - offsetX), curvePower);
		return b*(-pow(abs(input + deadzone - offsetX), curvePower)) + trimY;
	}
	else if (input >(offsetX + deadzone))
	{
		float a = (saturationYHigh - trimY) / pow(abs(-saturationXHigh + deadzone + offsetX), curvePower);
		return a*(pow(abs(input - deadzone - offsetX), curvePower)) + trimY;
	}
	else
		return trimY;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class DeviceInput {
public:
	DeviceInput(InputDeviceType _deviceType) :
		deviceType(_deviceType) {}
	DeviceInput(InputDeviceType _deviceType, int _button, unsigned int _modifiers, bool _pressed) :
		deviceType(_deviceType), button(_button), modifiers(_modifiers), pressed(_pressed) {}
	InputDeviceType deviceType;
	int button = 0;
	unsigned int modifiers = 0;
	bool pressed = false;
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class KeyInput : public DeviceInput {
public:
	KeyInput() : DeviceInput(KEYBOARD) {}
	KeyInput(OIS::KeyCode _keyCode, unsigned int _modifiers, bool _pressed) :
		DeviceInput(KEYBOARD, _keyCode, _modifiers, _pressed) {}
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class MouseInput : public DeviceInput {
public:
	MouseInput() : DeviceInput(MOUSE) {}
	MouseInput(OIS::MouseButtonID _buttonID, unsigned int _modifiers, bool _pressed) :
		DeviceInput(MOUSE, _buttonID, _modifiers, _pressed) {}
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class JoystickInput : public DeviceInput {
public:
	JoystickInput() : DeviceInput(JOYSTICK) {}
	JoystickInput(int _buttonID, unsigned int _modifiers, bool _pressed, unsigned int _joyID = 0) :
		DeviceInput(JOYSTICK, _buttonID, _modifiers, _pressed), joyID(_joyID) {}
	unsigned int joyID = 0;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct JoystickAnalog {
	std::vector<float> axisValues;
	std::vector<float> sliderValues;
	std::vector<bool> axisChanged;
	std::vector<bool>sliderChanged;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct JoystickPOVState {
	std::vector<int> directions;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class Input::Impl : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
public:
	Impl()
	{
		//otCore::Log::message("---- Input ----");
		reset();
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void reset()
	{
		numDevices = 0;	numJoysticks = 0;
		hasMouse = false; hasKeyboard = false;
		handle = nullptr; globalInput = nullptr;
		keyboardObject = nullptr; mouseObject = nullptr;
		initialized = false;
		mouseRelAxisValue[0] = 0; mouseRelAxisValue[1] = 0; mouseRelAxisValue[2] = 0;
		mouseAbsAxisValue[0] = 0; mouseAbsAxisValue[1] = 0; mouseAbsAxisValue[2] = 0;
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	~Impl()
	{
		clearInput();
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void clearInput()
	{
		if (keyboardObject)
			globalInput->destroyInputObject(keyboardObject);
		keyboardObject = nullptr;

		if (mouseObject)
			globalInput->destroyInputObject(mouseObject);
		mouseObject = nullptr;

		for (auto joystick : joystickObjects) {
			if (joystick)
				globalInput->destroyInputObject(joystick);
			joystick = nullptr;
		}
		joystickObjects.clear();

		joystickAnalogs.clear();
		joystickPOVStates.clear();
		joystickIDMap.clear();

		while (!deviceInputs.empty()) {
			delete deviceInputs.front();
			deviceInputs.pop();
		}

		heldButtons.clear();

		if (globalInput)
			globalInput->destroyInputSystem(globalInput);
		globalInput = nullptr;
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	//Redefine KeyListener virtual functions
	bool keyPressed(const OIS::KeyEvent &arg);
	bool keyReleased(const OIS::KeyEvent &arg);

	//Redefine MouseListener virtual functions
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	//Redefine JoyStickListener virtual functions
	/// Joystick button down event
	bool buttonPressed(const OIS::JoyStickEvent &arg, int button);

	/// Joystick button up event
	bool buttonReleased(const OIS::JoyStickEvent &arg, int button);

	/// Joystick axis moved event
	bool axisMoved(const OIS::JoyStickEvent &arg, int axis);

	//-- Not so common control events, so are not required --//
	/// Joystick Event, and sliderID
	bool sliderMoved(const OIS::JoyStickEvent &arg, int sliderID);

	/// Joystick Event, and povID
	bool povMoved(const OIS::JoyStickEvent &arg, int povID);

	/*
	//! Joystick Event, and Vector3ID
	bool vector3Moved( const JoyStickEvent &arg, int index) {return true;}
	*/

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void Initialize()
	{
		//deviceData.Devices.clear();
		//deviceData.DeviceInfo.clear();
		/*
		initialized = InitializeDirectInput();
		if (initialized) {
		Input::enumerateDevices();
		}*/

		std::size_t windowhandle;
		if (handle)
			windowhandle = (std::size_t)handle;
		else
			windowhandle = (std::size_t)FindMyTopMostWindow();

		//otCore::Log::message("Window handle: %d", windowhandle);

		OIS::ParamList pl;
		std::ostringstream wnd;
		wnd << windowhandle;
		pl.insert(std::make_pair(std::string("WINDOW"), wnd.str()));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

		globalInput = OIS::InputManager::createInputSystem(pl);
		//Log::message("---- Devices ----:\n");
		//OIS::DeviceList list = globalInput->listFreeDevices();

		numJoysticks = globalInput->getNumberOfDevices(OIS::OISJoyStick);


		for (int i = 0; i<numJoysticks; i++)
		{
			joystickObjects.push_back(static_cast<OIS::JoyStick*>(globalInput->createInputObject(OIS::OISJoyStick, true)));
			joystickObjects.back()->setEventCallback(this);
			joystickIDMap.insert(std::pair<std::string, int>(joystickObjects.back()->vendor(), i));

			JoystickAnalog joystickAnalog;
			int joyAxes = joystickObjects.back()->getNumberOfComponents(OIS::OIS_Axis);
			for (int j = 0; j<joyAxes; j++) {
				joystickAnalog.axisValues.push_back(0);
				joystickAnalog.axisChanged.push_back(false);
			}

			int joySliders = joystickObjects.back()->getNumberOfComponents(OIS::OIS_Slider);
			for (int j = 0; j<joySliders; j++) {
				joystickAnalog.sliderValues.push_back(0);
				joystickAnalog.sliderChanged.push_back(false);
			}

			joystickAnalogs.push_back(joystickAnalog);

			JoystickPOVState joystickPOVState;
			int joyPOVs = joystickObjects.back()->getNumberOfComponents(OIS::OIS_POV);
			for (int j = 0; j<joyPOVs; j++)
				joystickPOVState.directions.push_back(0);

			joystickPOVStates.push_back(joystickPOVState);

			numDevices++;
		}

		//Only get one object of each keyboard/mouse (There should be only one, anyway)
		if (globalInput->getNumberOfDevices(OIS::OISKeyboard) > 0) {
			keyboardObject = static_cast<OIS::Keyboard*>(globalInput->createInputObject(OIS::OISKeyboard, true));
			hasKeyboard = true;
			keyboardObject->setEventCallback(this);
			numDevices++;
		}

		if (globalInput->getNumberOfDevices(OIS::OISMouse) > 0) {
			mouseObject = static_cast<OIS::Mouse*>(globalInput->createInputObject(OIS::OISMouse, true));
			hasMouse = true;
			mouseObject->setEventCallback(this);
			numDevices++;
		}

		commandManager = &CommandManager::getInstance();
		inputMapper = &InputMapper::getInstance();

		initialized = true;
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void printModifiers(unsigned int modifiers)
	{
#ifdef _DEBUG
		if (modifiers > 0) {
			std::cout << " MODIFIER: ";
			if (modifiers & LCtrl)
				std::cout << "LCtrl | ";
			if (modifiers & RCtrl)
				std::cout << "RCtrl | ";
			if (modifiers & LAlt)
				std::cout << "LAlt | ";
			if (modifiers & RAlt)
				std::cout << "RAlt | ";
			if (modifiers & LShift)
				std::cout << "LShift | ";
			if (modifiers & RShift)
				std::cout << "RShift | ";
			if (modifiers & LWin)
				std::cout << "LWin | ";
			if (modifiers & RWin)
				std::cout << "RWin | ";
		}
#endif
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	//additional pressed boolean to overwrite the input pressed flag
	void executeCommand(DeviceInput* input, bool pressed)
	{
		ButtonBindingReturn action;
		unsigned int joyID = 0;
		std::string joystickName = "";
		if (input->deviceType == JOYSTICK) {
			JoystickInput* joyInput = static_cast<JoystickInput*>(input);
			joyID = joyInput->joyID;
			for (const auto& it : joystickIDMap)
			{
				if (it.second == joyID)
				{
					joystickName = it.first;
					break;
				}
			}
		}

		if (inputMapper->getActionFromInput(input->deviceType, input->button, input->modifiers, pressed, action, joystickName))
		{
			//keep track of held buttons so that the proper release command (with modifiers)
			//can be dispatched if the button is released later (even if modifiers are not still pressed)
			if (action.hold && pressed)
				heldButtons.push_back(*input);

			commandManager->getCommandByID(action.command)->setValue(action.value);
#ifdef _DEBUG
			std::cout << "COMMAND EXECUTED: " << action.command << " Value: " << action.value << " (" << inputMapper->getCommandNameByID(action.command) << ")" << std::endl;
#endif
		}

#ifdef _DEBUG
		if (pressed) std::cout << "DN    ";
		else std::cout << "UP    ";
		std::cout << input->button;
		if (input->deviceType == JOYSTICK)
			std::cout << "   JOY" << joyID << "  ";
		printModifiers(input->modifiers);
		std::cout << std::endl;
#endif
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void executeMouseAxisCommand(unsigned int axisID, int axisValue)
	{
		AxisBindingReturn action;

		if (inputMapper->getActionFromAxisInput(MOUSE, axisID, action))
		{
			//mouse axes ignore curvatures, deadzone, saturationX/Y
			float invValue = (!action.invert ? 1.0f : -1.0f);
			float value = invValue * action.scalar * axisValue;

			commandManager->getAxisCommandByID(action.axisCommand)->setValue(value, action.relative);
		}
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void executeJoystickAxisCommand(const std::string& joystickName, unsigned int axisID, float axisValue)
	{
		AxisBindingReturn action;

		if (inputMapper->getActionFromAxisInput(JOYSTICK, axisID, action, joystickName))
		{
			const float maxCurvePower = 4.0f; //power
			float curvature = action.curvature < -1.0f ? -1.0f : action.curvature > 1.0f ? 1.0f : action.curvature;
			float curvePower = (abs(action.curvature) - 0.0f) / (1.0f - 0.0f) * (maxCurvePower - 1.0f) + 1.0f;
			if (action.curvature < 0)
				curvePower = 1.0f / curvePower;

			float value = getAxisCurve(axisValue, curvePower, action.deadzone, action.saturationXLow, action.saturationXHigh, action.saturationYLow, action.saturationYHigh, 0, 0);

			float invValue = (!action.invert ? 1.0f : -1.0f);
			value *= invValue * action.scalar;

			commandManager->getAxisCommandByID(action.axisCommand)->setValue(value, action.relative);
		}
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	void executeJoystickSliderCommand(const std::string& joystickName, unsigned int sliderID, float sliderValue)
	{
		if (sliderID < NUM_JOYSTICK_SLIDERS) {
			executeJoystickAxisCommand(joystickName, (NUM_JOYSTICK_AXES + sliderID), sliderValue);
		}
	}

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	bool initialized;

	int numDevices;
	int numJoysticks;

	bool hasMouse;
	bool hasKeyboard;

	HWND handle;

	OIS::InputManager* globalInput;
	OIS::Keyboard* keyboardObject;
	OIS::Mouse* mouseObject;

	CommandManager* commandManager;
	InputMapper* inputMapper;

	std::vector<OIS::JoyStick*> joystickObjects;
	std::vector<JoystickAnalog> joystickAnalogs;
	std::vector<JoystickPOVState> joystickPOVStates;
	std::map<std::string, int>  joystickIDMap;

	std::queue<DeviceInput*> deviceInputs;
	std::vector<DeviceInput> heldButtons;

	int mouseRelAxisValue[3];
	int mouseAbsAxisValue[3];
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Input::Input() : mImpl(new Impl)
{

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Input::~Input()
{
	if (mImpl) delete mImpl;
	mImpl = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Input::enumerateDevices()
{
	HWND handle = mImpl->handle;
	mImpl->clearInput();
	mImpl->reset();
	setHandle(handle);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Input::setHandle(HWND handle)
{
	mImpl->handle = handle;
	mImpl->Initialize();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Input::reloadInputMappings()
{
	if (mImpl->inputMapper)
		mImpl->inputMapper->reloadInputMappings();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Input::update()
{
	if (!mImpl->initialized)
		mImpl->Initialize();

	if (mImpl->initialized) {
		//get commands since last iteration
		//device press/release commands
		while (!mImpl->deviceInputs.empty()) {
			DeviceInput* deviceInput = mImpl->deviceInputs.front();
			int button = deviceInput->button;
			if (deviceInput->pressed)
			{
				//don't need to worry about key modifiers being pressed
				//OIS was modified to not send those events by themselves (only releases)
				mImpl->executeCommand(deviceInput, true);
			}
			//on release of a button, check the currently held buttons of the "hold" type
			//to see if that button needs to be released
			else {
				bool skipSecondLoop = false;
				for (int i = 0; i<mImpl->heldButtons.size(); i++)
				{
					DeviceInput* heldButton = &mImpl->heldButtons.at(i);
					//only keyboard buttons can be modifiers, so check if a modifer was released,
					//even if the associated button is still held down
					if (deviceInput->deviceType == KEYBOARD) {
						OIS::KeyCode kc = static_cast<OIS::KeyCode>(deviceInput->button);
						if (mImpl->inputMapper->isModifierKey(kc)) {
							unsigned int releasedModifier = mImpl->inputMapper->getKeyModifierByKeyCode(kc);
							//must make sure to release all held button combos on all devices using the modifier that was released
							if (heldButton->modifiers & releasedModifier) {
								mImpl->executeCommand(heldButton, false);
								mImpl->heldButtons.erase(mImpl->heldButtons.begin() + i);
								i--; //set the counter back by one, since the vector size we are checking has also been reduced
							}
							skipSecondLoop = true;
						}
					}

					//otherwise, we care about when only a button is released
					if (!skipSecondLoop && deviceInput->deviceType == heldButton->deviceType) {
						if (deviceInput->deviceType == JOYSTICK) {
							JoystickInput* joystickInput = static_cast<JoystickInput*>(deviceInput);
							if (joystickInput->joyID != static_cast<JoystickInput*>(heldButton)->joyID)
								continue;
						}
						if (deviceInput->button == heldButton->button) {
							mImpl->executeCommand(heldButton, false);
							mImpl->heldButtons.erase(mImpl->heldButtons.begin() + i);
							//break out of for loop, since button combos can only exist in the list once
							break;
						}
					}
				}
			}

			if (!mImpl->deviceInputs.empty())
				mImpl->deviceInputs.pop();

			if (deviceInput->button == button)
				delete deviceInput;
			deviceInput = nullptr;
		}

		//mouse movement
		for (int i = 0; i<3; i++) {
			if (mImpl->mouseRelAxisValue[i] != 0) {
				mImpl->executeMouseAxisCommand(i, mImpl->mouseRelAxisValue[i]);
				mImpl->mouseRelAxisValue[i] = 0;
			}
		}

		//joystick movement
		for (int i = 0; i<mImpl->numJoysticks; i++) {
			std::string joystickName = "";
			for (const auto& it : mImpl->joystickIDMap)
			{
				if (it.second == i)
				{
					joystickName = it.first;
					break;
				}
			}

			for (unsigned int j = 0; j<mImpl->joystickAnalogs.at(i).axisValues.size(); j++) {
				if (mImpl->joystickAnalogs.at(i).axisChanged.at(j)) {
					mImpl->executeJoystickAxisCommand(joystickName, j, mImpl->joystickAnalogs.at(i).axisValues.at(j));
					mImpl->joystickAnalogs.at(i).axisChanged.at(j) = false;
				}
			}
		}

		//slider movement
		for (int i = 0; i<mImpl->numJoysticks; i++) {
			std::string joystickName = "";
			for (const auto& it : mImpl->joystickIDMap)
			{
				if (it.second == i)
				{
					joystickName = it.first;
					break;
				}
			}

			for (unsigned int j = 0; j<mImpl->joystickAnalogs.at(i).sliderValues.size(); j++) {
				if (mImpl->joystickAnalogs.at(i).sliderChanged.at(j)) {
					mImpl->executeJoystickSliderCommand(joystickName, j, mImpl->joystickAnalogs.at(i).sliderValues.at(j));
					mImpl->joystickAnalogs.at(i).sliderChanged.at(j) = false;
				}
			}
		}


		//keep listeners alive
		if (mImpl->mouseObject) mImpl->mouseObject->capture();

		if (mImpl->keyboardObject) mImpl->keyboardObject->capture();

		for (int i = 0; i<mImpl->numJoysticks; i++) {
			if (mImpl->joystickObjects.at(i)) mImpl->joystickObjects.at(i)->capture();
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int Input::getNumDevices() const
{
	return mImpl->numDevices;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Input callbacks

bool Input::Impl::keyPressed(const OIS::KeyEvent &arg) {
	//turned off sending of modifier key presses (but not releases) in OIS library
	DeviceInput* _keyInput = new KeyInput(arg.key, keyboardObject->getModifiers(), true);
	deviceInputs.push(_keyInput);
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::keyReleased(const OIS::KeyEvent &arg) {
	DeviceInput* _keyInput = new KeyInput(arg.key, keyboardObject->getModifiers(), false);
	deviceInputs.push(_keyInput);
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::mouseMoved(const OIS::MouseEvent &arg) {
	mouseAbsAxisValue[0] += arg.state.X.abs;
	mouseAbsAxisValue[1] += arg.state.Y.abs;
	mouseAbsAxisValue[2] += arg.state.Z.abs;

	mouseRelAxisValue[0] += arg.state.X.rel;
	mouseRelAxisValue[1] += arg.state.Y.rel;
	mouseRelAxisValue[2] += arg.state.Z.rel;
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	unsigned int modifiers = 0;
	if (hasKeyboard)
		modifiers = keyboardObject->getModifiers();
	DeviceInput* _mouseInput = new MouseInput(id, modifiers, true);
	deviceInputs.push(_mouseInput);
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
	unsigned int modifiers = 0;
	if (hasKeyboard)
		modifiers = keyboardObject->getModifiers();
	DeviceInput* _mouseInput = new MouseInput(id, modifiers, false);
	deviceInputs.push(_mouseInput);
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::buttonPressed(const OIS::JoyStickEvent &arg, int button) {
	int joyID = -1;
	if (joystickIDMap.find(arg.device->vendor()) != joystickIDMap.end()) {
		joyID = joystickIDMap[arg.device->vendor()];
	}
	if (joyID >= 0) {
		unsigned int modifiers = 0;
		if (hasKeyboard)
			modifiers = keyboardObject->getModifiers();

		JoystickInput* _joystickInput = new JoystickInput(button, modifiers, true, joyID);
		deviceInputs.push(_joystickInput);
	}
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::buttonReleased(const OIS::JoyStickEvent &arg, int button) {
	int joyID = -1;
	if (joystickIDMap.find(arg.device->vendor()) != joystickIDMap.end()) {
		joyID = joystickIDMap[arg.device->vendor()];
	}
	if (joyID >= 0) {
		unsigned int modifiers = 0;
		if (hasKeyboard)
			modifiers = keyboardObject->getModifiers();
		DeviceInput* _joystickInput = new JoystickInput(button, modifiers, false, joyID);
		deviceInputs.push(_joystickInput);
	}
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::axisMoved(const OIS::JoyStickEvent &arg, int axis) {
	int joyID = -1;
	if (joystickIDMap.find(arg.device->vendor()) != joystickIDMap.end()) {
		joyID = joystickIDMap[arg.device->vendor()];
	}
	if (joyID >= 0) {
		float value = ((float)arg.state.mAxes.at(axis).abs) / (float)32768;
		value = value < -1.0f ? -1.0f : value > 1.0f ? 1.0f : value;
		joystickAnalogs.at(joyID).axisValues.at(axis) = value;
		joystickAnalogs.at(joyID).axisChanged.at(axis) = true;
		//std::cout << "Joy " << joyID << " Axis: " << axis << " Value: " << joystickAnalogs.at(joyID).axisValues.at(axis) << std::endl;
	}
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::sliderMoved(const OIS::JoyStickEvent &arg, int sliderID)
{
	int joyID = -1;
	if (joystickIDMap.find(arg.device->vendor()) != joystickIDMap.end()) {
		joyID = joystickIDMap[arg.device->vendor()];
	}
	if (joyID >= 0) {
		float value = ((float)arg.state.mSliders[sliderID].abX) / (float)32768;
		value = value < -1.0f ? -1.0f : value > 1.0f ? 1.0f : value;
		joystickAnalogs.at(joyID).sliderValues.at(sliderID) = value;
		joystickAnalogs.at(joyID).sliderChanged.at(sliderID) = true;
		//std::cout << "Joy " << joyID << " Sldier: " << sliderID << " Value: " << joystickAnalogs.at(joyID).sliderValues.at(index) << std::endl;
	}
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int getPOVButton(int direction)
{
	switch (direction) {
	case OIS::Pov::Centered:
		return 0;
	case OIS::Pov::North:
		return 1;
	case OIS::Pov::South:
		return 2;
	case OIS::Pov::East:
		return 3;
	case OIS::Pov::West:
		return 4;
	case OIS::Pov::NorthEast:
		return 5;
	case OIS::Pov::SouthEast:
		return 6;
	case OIS::Pov::NorthWest:
		return 7;
	case OIS::Pov::SouthWest:
		return 8;
	default:
		return 0;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Input::Impl::povMoved(const OIS::JoyStickEvent &arg, int povID)
{
	int joyID = -1;
	if (joystickIDMap.find(arg.device->vendor()) != joystickIDMap.end()) {
		joyID = joystickIDMap[arg.device->vendor()];
	}
	if (joyID >= 0) {
		int direction = arg.state.mPOV[povID].direction;

		unsigned int modifiers = 0;
		if (hasKeyboard)
			modifiers = keyboardObject->getModifiers();

		bool pressed = true;
		int button = 31 + povID * 8;

		if (direction == OIS::Pov::Centered) {
			pressed = false;
			//get last direction for the release action
			direction = joystickPOVStates.at(joyID).directions.at(povID);
		}

		int POVButton = getPOVButton(direction);

		if (POVButton > 0) {
			button += POVButton;

			//update state
			joystickPOVStates.at(joyID).directions.at(povID) = arg.state.mPOV[povID].direction;

			JoystickInput* _joystickInput = new JoystickInput(button, modifiers, pressed, joyID);
			deviceInputs.push(_joystickInput);
		}
	}
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otInput

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%