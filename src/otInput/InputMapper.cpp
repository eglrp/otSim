/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       InputMapper.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Input mapper class.  Reads input configuration file for key bindings and
maps the key combos to commands.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "InputMapper.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

#include "OISKeyboard.h"
#include "OISMouse.h"

#include "Paths.h"
#include "Stopwatch.h"
//#include "Log.h"
#include "JSON.h"
#include "ActionCommands.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otInput {

std::array<std::string, COMMAND_MAXIMUM> commandNames;
std::array<std::string, COMMAND_AXIS_MAXIMUM> axisCommandNames;
std::array<std::string, NUM_KEYBOARD_KEYS> keyNames;
std::array<std::string, NUM_MOUSE_BUTTONS> mouseButtonNames;
std::array<std::string, NUM_MOUSE_AXES> mouseAxisNames;
std::array<std::string, NUM_JOYSTICK_AXES + NUM_JOYSTICK_SLIDERS> joystickAxisNames;
std::array<std::string, NUM_JOYSTICK_BUTTONS + NUM_JOYSTICK_POVS*NUM_POV_DIRECTIONS> joystickButtonNames;
std::array<std::string, NUM_JOYSTICK_SLIDERS> joystickSliderNames;

class ButtonBinding
{
public:
	unsigned int id = 0;
	ActionKeyCommands pressedCommand = COMMAND_NULL;
	ActionKeyCommands downCommand = COMMAND_NULL;
	ActionKeyCommands upCommand = COMMAND_NULL;
	bool hold = false;
	float pressedValue = 1.0f;
	float downValue = 1.0f;
	float upValue = 0.0f;
	std::string displayName = "ERROR: NO NAME";
	std::string category = "Other";
	std::vector<KeyModifiers> modifiers;
};

//class JoystickButtonBinding : public ButtonBinding
//{
//public:
//	JoystickButtonBinding::JoystickButtonBinding() : {}
//	//std::string joystickName;
//};

class AxisBinding
{
public:
	unsigned int id = 0;
	ActionAxisCommands axisCommand = COMMAND_AXIS_NULL;
	float scalar = 1.0f;
	float curvature = 0.0f;
	float deadzone = 0.0f;
	float saturationXLow = 1.0f;
	float saturationYLow = 1.0f;
	float saturationXHigh = 1.0f;
	float saturationYHigh = 1.0f;
	bool relative = false;
	bool invert = false;
	std::string displayName = "ERROR: NO NAME";
	std::string category = "Other";
};

struct JoystickBindings {
	std::vector<ButtonBinding*> buttonBindingList;
	std::vector<AxisBinding*> axisBindingList;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class InputMapper::Impl
{
public:
	Impl()
	{
		commandNames[COMMAND_NULL] = "COMMAND_NULL";
		commandNames[COMMAND_VIEW_FIRST_PERSON] = "COMMAND_VIEW_FIRST_PERSON";
		commandNames[COMMAND_VIEW_EXTERNAL_ORBIT] = "COMMAND_VIEW_EXTERNAL_ORBIT";
		commandNames[COMMAND_VIEW_EXTERNAL_FLYBY] = "COMMAND_VIEW_EXTERNAL_FLYBY";
		commandNames[COMMAND_VIEW_EXTERNAL_FIXED] = "COMMAND_VIEW_EXTERNAL_FIXED";
		commandNames[COMMAND_VIEW_EXTERNAL_PADLOCK] = "COMMAND_VIEW_EXTERNAL_PADLOCK";
		commandNames[COMMAND_VIEW_EXTERNAL_CHASE] = "COMMAND_VIEW_EXTERNAL_CHASE";
		commandNames[COMMAND_VIEW_EXTERNAL_TOWER] = "COMMAND_VIEW_EXTERNAL_TOWER";
		commandNames[COMMAND_VIEW_EXTERNAL_FLYBY_SWITCH] = "COMMAND_VIEW_EXTERNAL_FLYBY_SWITCH";
		commandNames[COMMAND_VIEW_EXTERNAL_TOWER_SWITCH] = "COMMAND_VIEW_EXTERNAL_TOWER_SWITCH";
		commandNames[COMMAND_VIEW_ROTATE_STOP] = "COMMAND_VIEW_ROTATE_STOP";
		commandNames[COMMAND_VIEW_ROTATE_LEFT] = "COMMAND_VIEW_ROTATE_LEFT";
		commandNames[COMMAND_VIEW_ROTATE_RIGHT] = "COMMAND_VIEW_ROTATE_RIGHT";
		commandNames[COMMAND_VIEW_ROTATE_UP] = "COMMAND_VIEW_ROTATE_UP";
		commandNames[COMMAND_VIEW_ROTATE_DOWN] = "COMMAND_VIEW_ROTATE_DOWN";
		commandNames[COMMAND_VIEW_ROTATE_UP_LEFT] = "COMMAND_VIEW_ROTATE_UP_LEFT";
		commandNames[COMMAND_VIEW_ROTATE_UP_RIGHT] = "COMMAND_VIEW_ROTATE_UP_RIGHT";
		commandNames[COMMAND_VIEW_ROTATE_DOWN_LEFT] = "COMMAND_VIEW_ROTATE_DOWN_LEFT";
		commandNames[COMMAND_VIEW_ROTATE_DOWN_RIGHT] = "COMMAND_VIEW_ROTATE_DOWN_RIGHT";
		commandNames[COMMAND_VIEW_ROTATE_LEFT_FAST] = "COMMAND_VIEW_ROTATE_LEFT_FAST";
		commandNames[COMMAND_VIEW_ROTATE_RIGHT_FAST] = "COMMAND_VIEW_ROTATE_RIGHT_FAST";
		commandNames[COMMAND_VIEW_ROTATE_UP_FAST] = "COMMAND_VIEW_ROTATE_UP_FAST";
		commandNames[COMMAND_VIEW_ROTATE_DOWN_FAST] = "COMMAND_VIEW_ROTATE_DOWN_FAST";
		commandNames[COMMAND_VIEW_ROTATE_UP_LEFT_FAST] = "COMMAND_VIEW_ROTATE_UP_LEFT_FAST";
		commandNames[COMMAND_VIEW_ROTATE_UP_RIGHT_FAST] = "COMMAND_VIEW_ROTATE_UP_RIGHT_FAST";
		commandNames[COMMAND_VIEW_ROTATE_DOWN_LEFT_FAST] = "COMMAND_VIEW_ROTATE_DOWN_LEFT_FAST";
		commandNames[COMMAND_VIEW_ROTATE_DOWN_RIGHT_FAST] = "COMMAND_VIEW_ROTATE_DOWN_RIGHT_FAST";
		commandNames[COMMAND_VIEW_CENTER] = "COMMAND_VIEW_CENTER";
		commandNames[COMMAND_VIEW_MOVE_STOP] = "COMMAND_VIEW_MOVE_STOP";
		commandNames[COMMAND_VIEW_MOVE_FORWARD] = "COMMAND_VIEW_MOVE_FORWARD";
		commandNames[COMMAND_VIEW_MOVE_BACKWARD] = "COMMAND_VIEW_MOVE_BACKWARD";
		commandNames[COMMAND_VIEW_MOVE_FORWARD_FAST] = "COMMAND_VIEW_MOVE_FORWARD_FAST";
		commandNames[COMMAND_VIEW_MOVE_BACKWARD_FAST] = "COMMAND_VIEW_MOVE_BACKWARD_FAST";
		commandNames[COMMAND_VIEW_ZOOM_STOP] = "COMMAND_VIEW_ZOOM_STOP";
		commandNames[COMMAND_VIEW_ZOOM_IN] = "COMMAND_VIEW_ZOOM_IN";
		commandNames[COMMAND_VIEW_ZOOM_OUT] = "COMMAND_VIEW_ZOOM_OUT";
		commandNames[COMMAND_VIEW_ZOOM_RESET] = "COMMAND_VIEW_ZOOM_RESET";
		commandNames[COMMAND_VIEW_PADLOCK] = "COMMAND_VIEW_PADLOCK";
		commandNames[COMMAND_VIEW_UNPADLOCK] = "COMMAND_VIEW_UNPADLOCK";
		commandNames[COMMAND_INCREASE_SIMULATION_RATE] = "COMMAND_INCREASE_SIMULATION_RATE";
		commandNames[COMMAND_DECREASE_SIMULATION_RATE] = "COMMAND_DECREASE_SIMULATION_RATE";
		commandNames[COMMAND_NORMAL_SIMULATION_RATE] = "COMMAND_NORMAL_SIMULATION_RATE";
		commandNames[COMMAND_PAUSE_SIMULATION] = "COMMAND_PAUSE_SIMULATION";
		commandNames[COMMAND_QUIT] = "COMMAND_QUIT";
		commandNames[COMMAND_INFO_TOGGLE] = "COMMAND_INFO_TOGGLE";
		commandNames[COMMAND_SCREENSHOT] = "COMMAND_SCREENSHOT";
		commandNames[COMMAND_FPS] = "COMMAND_FPS";
		commandNames[COMMAND_NETWORK_WINDOW] = "COMMAND_NETWORK_WINDOW";
		commandNames[COMMAND_WHEEL_BRAKE_ALL] = "COMMAND_WHEEL_BRAKE_ALL";
		commandNames[COMMAND_WHEEL_BRAKE_LEFT] = "COMMAND_WHEEL_BRAKE_LEFT";
		commandNames[COMMAND_WHEEL_BRAKE_RIGHT] = "COMMAND_WHEEL_BRAKE_RIGHT";
		commandNames[COMMAND_TEST] = "COMMAND_TEST";

		axisCommandNames[COMMAND_AXIS_NULL] = "COMMAND_AXIS_NULL";
		axisCommandNames[COMMAND_AXIS_VIEW_ROTATE_HORIZONTAL] = "COMMAND_AXIS_VIEW_ROTATE_HORIZONTAL";
		axisCommandNames[COMMAND_AXIS_VIEW_ROTATE_VERTICAL] = "COMMAND_AXIS_VIEW_ROTATE_VERTICAL";
		axisCommandNames[COMMAND_AXIS_VIEW_ROTATE_ROLL] = "COMMAND_AXIS_VIEW_ROTATE_ROLL";
		axisCommandNames[COMMAND_AXIS_VIEW_MOVE_LONGITUDINAL] = "COMMAND_AXIS_VIEW_MOVE_LONGITUDINAL";
		axisCommandNames[COMMAND_AXIS_VIEW_MOVE_LATERAL] = "COMMAND_AXIS_VIEW_MOVE_LATERAL";
		axisCommandNames[COMMAND_AXIS_VIEW_MOVE_VERTICAL] = "COMMAND_AXIS_VIEW_MOVE_VERTICAL";
		axisCommandNames[COMMAND_AXIS_CONTROL_PITCH] = "COMMAND_AXIS_CONTROL_PITCH";
		axisCommandNames[COMMAND_AXIS_CONTROL_ROLL] = "COMMAND_AXIS_CONTROL_ROLL";
		axisCommandNames[COMMAND_AXIS_CONTROL_YAW] = "COMMAND_AXIS_CONTROL_YAW";
		axisCommandNames[COMMAND_AXIS_CONTROL_THROTTLE] = "COMMAND_AXIS_CONTROL_THROTTLE";

		keyNames[OIS::KC_UNASSIGNED] = "Unassigned";
		keyNames[OIS::KC_ESCAPE] = "Esc";
		keyNames[OIS::KC_1] = "1";
		keyNames[OIS::KC_2] = "2";
		keyNames[OIS::KC_3] = "3";
		keyNames[OIS::KC_4] = "4";
		keyNames[OIS::KC_5] = "5";
		keyNames[OIS::KC_6] = "6";
		keyNames[OIS::KC_7] = "7";
		keyNames[OIS::KC_8] = "8";
		keyNames[OIS::KC_9] = "9";
		keyNames[OIS::KC_0] = "0";
		keyNames[OIS::KC_MINUS] = "-";
		keyNames[OIS::KC_EQUALS] = "=";
		keyNames[OIS::KC_BACK] = "Back";
		keyNames[OIS::KC_TAB] = "Tab";
		keyNames[OIS::KC_Q] = "Q";
		keyNames[OIS::KC_W] = "W";
		keyNames[OIS::KC_E] = "E";
		keyNames[OIS::KC_R] = "R";
		keyNames[OIS::KC_T] = "T";
		keyNames[OIS::KC_Y] = "Y";
		keyNames[OIS::KC_U] = "U";
		keyNames[OIS::KC_I] = "I";
		keyNames[OIS::KC_O] = "O";
		keyNames[OIS::KC_P] = "P";
		keyNames[OIS::KC_LBRACKET] = "[";
		keyNames[OIS::KC_RBRACKET] = "]";
		keyNames[OIS::KC_RETURN] = "Enter";
		keyNames[OIS::KC_LCONTROL] = "LCtrl";
		keyNames[OIS::KC_A] = "A";
		keyNames[OIS::KC_S] = "S";
		keyNames[OIS::KC_D] = "D";
		keyNames[OIS::KC_F] = "F";
		keyNames[OIS::KC_G] = "G";
		keyNames[OIS::KC_H] = "H";
		keyNames[OIS::KC_J] = "J";
		keyNames[OIS::KC_K] = "K";
		keyNames[OIS::KC_L] = "L";
		keyNames[OIS::KC_SEMICOLON] = ";";
		keyNames[OIS::KC_APOSTROPHE] = "'";
		keyNames[OIS::KC_GRAVE] = "`";
		keyNames[OIS::KC_LSHIFT] = "LShift";
		keyNames[OIS::KC_BACKSLASH] = "\\";
		keyNames[OIS::KC_Z] = "Z";
		keyNames[OIS::KC_X] = "X";
		keyNames[OIS::KC_C] = "C";
		keyNames[OIS::KC_V] = "V";
		keyNames[OIS::KC_B] = "B";
		keyNames[OIS::KC_N] = "N";
		keyNames[OIS::KC_M] = "M";
		keyNames[OIS::KC_COMMA] = ",";
		keyNames[OIS::KC_PERIOD] = ".";
		keyNames[OIS::KC_SLASH] = "/";
		keyNames[OIS::KC_RSHIFT] = "RShift";
		keyNames[OIS::KC_MULTIPLY] = "Num*";
		keyNames[OIS::KC_LMENU] = "LAlt";
		keyNames[OIS::KC_SPACE] = "Space";
		keyNames[OIS::KC_CAPITAL] = "CapsLock";
		keyNames[OIS::KC_F1] = "F1";
		keyNames[OIS::KC_F2] = "F2";
		keyNames[OIS::KC_F3] = "F3";
		keyNames[OIS::KC_F4] = "F4";
		keyNames[OIS::KC_F5] = "F5";
		keyNames[OIS::KC_F6] = "F6";
		keyNames[OIS::KC_F7] = "F7";
		keyNames[OIS::KC_F8] = "F8";
		keyNames[OIS::KC_F9] = "F9";
		keyNames[OIS::KC_F10] = "F10";
		keyNames[OIS::KC_NUMLOCK] = "NumLock";
		keyNames[OIS::KC_SCROLL] = "ScrollLock";
		keyNames[OIS::KC_NUMPAD7] = "Num7";
		keyNames[OIS::KC_NUMPAD8] = "Num8";
		keyNames[OIS::KC_NUMPAD9] = "Num9";
		keyNames[OIS::KC_SUBTRACT] = "Num-";
		keyNames[OIS::KC_NUMPAD4] = "Num4";
		keyNames[OIS::KC_NUMPAD5] = "Num5";
		keyNames[OIS::KC_NUMPAD6] = "Num6";
		keyNames[OIS::KC_ADD] = "Num+";
		keyNames[OIS::KC_NUMPAD1] = "Num1";
		keyNames[OIS::KC_NUMPAD2] = "Num2";
		keyNames[OIS::KC_NUMPAD3] = "Num3";
		keyNames[OIS::KC_NUMPAD0] = "Num0";
		keyNames[OIS::KC_DECIMAL] = "Num.";
		keyNames[OIS::KC_OEM_102] = "OEM_102";
		keyNames[OIS::KC_F11] = "F11";
		keyNames[OIS::KC_F12] = "F12";
		keyNames[OIS::KC_F13] = "F13";
		keyNames[OIS::KC_F14] = "F14";
		keyNames[OIS::KC_F15] = "F15";
		keyNames[OIS::KC_KANA] = "Kana";
		keyNames[OIS::KC_ABNT_C1] = "ABNT_C1";
		keyNames[OIS::KC_CONVERT] = "Convert";
		keyNames[OIS::KC_NOCONVERT] = "NoConvert";
		keyNames[OIS::KC_YEN] = "Yen";
		keyNames[OIS::KC_ABNT_C2] = "ABNT_C2";
		keyNames[OIS::KC_NUMPADEQUALS] = "Num=";
		keyNames[OIS::KC_PREVTRACK] = "PrevTrack";
		keyNames[OIS::KC_AT] = "At";
		keyNames[OIS::KC_COLON] = "Colon";
		keyNames[OIS::KC_UNDERLINE] = "Underline";
		keyNames[OIS::KC_KANJI] = "Kanji";
		keyNames[OIS::KC_STOP] = "STOP";
		keyNames[OIS::KC_AX] = "Ax";
		keyNames[OIS::KC_UNLABELED] = "Unlabeled";
		keyNames[OIS::KC_NEXTTRACK] = "NextTrack";
		keyNames[OIS::KC_NUMPADENTER] = "NumEnter";
		keyNames[OIS::KC_RCONTROL] = "RCtrl";
		keyNames[OIS::KC_MUTE] = "Mute";
		keyNames[OIS::KC_CALCULATOR] = "Calculator";
		keyNames[OIS::KC_PLAYPAUSE] = "PlayPause";
		keyNames[OIS::KC_MEDIASTOP] = "MediaStop";
		keyNames[OIS::KC_VOLUMEDOWN] = "VolumeDown";
		keyNames[OIS::KC_VOLUMEUP] = "VolumeUp";
		keyNames[OIS::KC_WEBHOME] = "WebHome";
		keyNames[OIS::KC_NUMPADCOMMA] = "Num,";
		keyNames[OIS::KC_DIVIDE] = "Num/";
		keyNames[OIS::KC_SYSRQ] = "SysRQ";
		keyNames[OIS::KC_RMENU] = "RAlt";
		keyNames[OIS::KC_PAUSE] = "Pause";
		keyNames[OIS::KC_HOME] = "Home";
		keyNames[OIS::KC_UP] = "Up";
		keyNames[OIS::KC_PGUP] = "PageUp";
		keyNames[OIS::KC_LEFT] = "Left";
		keyNames[OIS::KC_RIGHT] = "Right";
		keyNames[OIS::KC_END] = "End";
		keyNames[OIS::KC_DOWN] = "Down";
		keyNames[OIS::KC_PGDOWN] = "PageDown";
		keyNames[OIS::KC_INSERT] = "Insert";
		keyNames[OIS::KC_DELETE] = "Delete";
		keyNames[OIS::KC_LWIN] = "LWin";
		keyNames[OIS::KC_RWIN] = "RWin";
		keyNames[OIS::KC_APPS] = "AppMenu";
		keyNames[OIS::KC_POWER] = "Power";
		keyNames[OIS::KC_SLEEP] = "Sleep";
		keyNames[OIS::KC_WAKE] = "Wake";
		keyNames[OIS::KC_WEBSEARCH] = "WebSearch";
		keyNames[OIS::KC_WEBFAVORITES] = "WebFavorites";
		keyNames[OIS::KC_WEBREFRESH] = "WebRefresh";
		keyNames[OIS::KC_WEBSTOP] = "WebStop";
		keyNames[OIS::KC_WEBFORWARD] = "WebForward";
		keyNames[OIS::KC_WEBBACK] = "WebBack";
		keyNames[OIS::KC_MYCOMPUTER] = "MyComputer";
		keyNames[OIS::KC_MAIL] = "Mail";
		keyNames[OIS::KC_MEDIASELECT] = "MediaSelect";

		mouseButtonNames[OIS::MB_Left] = "MOUSE_LEFT";
		mouseButtonNames[OIS::MB_Right] = "MOUSE_RIGHT";
		mouseButtonNames[OIS::MB_Middle] = "MOUSE_MIDDLE";
		mouseButtonNames[OIS::MB_Button3] = "MOUSE_BTN4";
		mouseButtonNames[OIS::MB_Button4] = "MOUSE_BTN5";
		mouseButtonNames[OIS::MB_Button5] = "MOUSE_BTN6";
		mouseButtonNames[OIS::MB_Button6] = "MOUSE_BTN7";
		mouseButtonNames[OIS::MB_Button7] = "MOUSE_BTN8";

		mouseAxisNames[0] = "MOUSE_X";
		mouseAxisNames[1] = "MOUSE_Y";
		mouseAxisNames[2] = "MOUSE_Z";

		joystickAxisNames[0] = "JOY_X";
		joystickAxisNames[1] = "JOY_Y";
		joystickAxisNames[2] = "JOY_Z";
		joystickAxisNames[3] = "JOY_RX";
		joystickAxisNames[4] = "JOY_RY";
		joystickAxisNames[5] = "JOY_RZ";
		joystickAxisNames[6] = "JOY_U";
		joystickAxisNames[7] = "JOY_V";

		//add the 4 possible sliders to the axis list
		joystickAxisNames[8] = "JOY_SLIDER1";
		joystickAxisNames[9] = "JOY_SLIDER2";
		joystickAxisNames[10] = "JOY_SLIDER3";
		joystickAxisNames[11] = "JOY_SLIDER4";

		joystickButtonNames[0] = "JOY_BTN1";
		joystickButtonNames[1] = "JOY_BTN2";
		joystickButtonNames[2] = "JOY_BTN3";
		joystickButtonNames[3] = "JOY_BTN4";
		joystickButtonNames[4] = "JOY_BTN5";
		joystickButtonNames[5] = "JOY_BTN6";
		joystickButtonNames[6] = "JOY_BTN7";
		joystickButtonNames[7] = "JOY_BTN8";
		joystickButtonNames[8] = "JOY_BTN9";
		joystickButtonNames[9] = "JOY_BTN10";
		joystickButtonNames[10] = "JOY_BTN11";
		joystickButtonNames[11] = "JOY_BTN12";
		joystickButtonNames[12] = "JOY_BTN13";
		joystickButtonNames[13] = "JOY_BTN14";
		joystickButtonNames[14] = "JOY_BTN15";
		joystickButtonNames[15] = "JOY_BTN16";
		joystickButtonNames[16] = "JOY_BTN17";
		joystickButtonNames[17] = "JOY_BTN18";
		joystickButtonNames[18] = "JOY_BTN19";
		joystickButtonNames[19] = "JOY_BTN20";
		joystickButtonNames[20] = "JOY_BTN21";
		joystickButtonNames[21] = "JOY_BTN22";
		joystickButtonNames[22] = "JOY_BTN23";
		joystickButtonNames[23] = "JOY_BTN24";
		joystickButtonNames[24] = "JOY_BTN25";
		joystickButtonNames[25] = "JOY_BTN26";
		joystickButtonNames[26] = "JOY_BTN27";
		joystickButtonNames[27] = "JOY_BTN28";
		joystickButtonNames[28] = "JOY_BTN29";
		joystickButtonNames[29] = "JOY_BTN30";
		joystickButtonNames[30] = "JOY_BTN31";
		joystickButtonNames[31] = "JOY_BTN32";

		//add the 4 possible POV hats (8 directions each) to the button list
		joystickButtonNames[32] = "JOY_BTN_POV1_U";
		joystickButtonNames[33] = "JOY_BTN_POV1_D";
		joystickButtonNames[34] = "JOY_BTN_POV1_R";
		joystickButtonNames[35] = "JOY_BTN_POV1_L";
		joystickButtonNames[36] = "JOY_BTN_POV1_UR";
		joystickButtonNames[37] = "JOY_BTN_POV1_DR";
		joystickButtonNames[38] = "JOY_BTN_POV1_UL";
		joystickButtonNames[39] = "JOY_BTN_POV1_DL";

		joystickButtonNames[40] = "JOY_BTN_POV2_U";
		joystickButtonNames[41] = "JOY_BTN_POV2_D";
		joystickButtonNames[42] = "JOY_BTN_POV2_R";
		joystickButtonNames[43] = "JOY_BTN_POV2_L";
		joystickButtonNames[44] = "JOY_BTN_POV2_UR";
		joystickButtonNames[45] = "JOY_BTN_POV2_DR";
		joystickButtonNames[46] = "JOY_BTN_POV2_UL";
		joystickButtonNames[47] = "JOY_BTN_POV2_DL";

		joystickButtonNames[48] = "JOY_BTN_POV3_U";
		joystickButtonNames[49] = "JOY_BTN_POV3_D";
		joystickButtonNames[50] = "JOY_BTN_POV3_R";
		joystickButtonNames[51] = "JOY_BTN_POV3_L";
		joystickButtonNames[52] = "JOY_BTN_POV3_UR";
		joystickButtonNames[53] = "JOY_BTN_POV3_DR";
		joystickButtonNames[54] = "JOY_BTN_POV3_UL";
		joystickButtonNames[55] = "JOY_BTN_POV3_DL";

		joystickButtonNames[56] = "JOY_BTN_POV4_U";
		joystickButtonNames[57] = "JOY_BTN_POV4_D";
		joystickButtonNames[58] = "JOY_BTN_POV4_R";
		joystickButtonNames[59] = "JOY_BTN_POV4_L";
		joystickButtonNames[60] = "JOY_BTN_POV4_UR";
		joystickButtonNames[61] = "JOY_BTN_POV4_DR";
		joystickButtonNames[62] = "JOY_BTN_POV4_UL";
		joystickButtonNames[63] = "JOY_BTN_POV4_DL";

		//readInputConfiguration();
	}

	~Impl()
	{
		clearInputMapper();
	}

	void clearInputMapper()
	{
		for (auto keyBinding : keyBindingList)
			delete keyBinding;

		for (auto mouseButtonBinding : mouseButtonBindingList)
			delete mouseButtonBinding;

		for (auto mouseAxisBinding : mouseAxisBindingList)
			delete mouseAxisBinding;


		keyBindingList.clear();
		mouseButtonBindingList.clear();
		mouseAxisBindingList.clear();


		for (auto joystick : joystickList) {
			for (auto button : joystick->buttonBindingList)
				delete button;
			joystick->buttonBindingList.clear();

			for (auto axis : joystick->axisBindingList)
				delete axis;
			joystick->axisBindingList.clear();

			delete joystick;
		}
		joystickList.clear();
	}


	void readInputConfiguration();
	void parseInputOptions(otCore::JSON& inputOpt, InputDeviceType inputDevice);

	std::vector<std::string> inputCategoryList;
	std::vector<ButtonBinding*> keyBindingList;

	std::vector<ButtonBinding*> mouseButtonBindingList;
	std::vector<AxisBinding*> mouseAxisBindingList;

	std::vector<std::string> joystickNames;
	std::vector<JoystickBindings*> joystickList;
};

void InputMapper::reloadInputMappings()
{
	mImpl->clearInputMapper();
	mImpl->readInputConfiguration();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

InputMapper::InputMapper() : mImpl(new Impl())
{
	reloadInputMappings();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

InputMapper::~InputMapper()
{
	delete mImpl;
	mImpl = 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringReplace(std::string a, const std::string &b, const std::string &c)
{
	size_t pos;
	do
	{
		pos = a.find(b);
		if (pos != -1)  a.replace(pos, b.length(), c);
	} while (pos != -1);
	return a;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string InputMapper::getCommandNameByID(ActionKeyCommands commandID) const
{
	return commandNames[commandID];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string InputMapper::getAxisCommandNameByID(ActionAxisCommands axisCommandID) const
{
	return axisCommandNames[axisCommandID];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ActionKeyCommands getCommandIDByName(const std::string &commandName)
{
	for (int i = 0; i<COMMAND_MAXIMUM; i++)
	{
		if (commandNames[i] == commandName)
			return static_cast<ActionKeyCommands>(i);
	}
	return COMMAND_NULL;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ActionAxisCommands getAxisCommandIDByName(const std::string &axisCommandName)
{
	for (int i = 0; i<COMMAND_AXIS_MAXIMUM; i++)
	{
		if (axisCommandNames[i] == axisCommandName)
			return static_cast<ActionAxisCommands>(i);
	}
	return COMMAND_AXIS_NULL;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

OIS::KeyCode getKeyCodeByKeyName(const std::string &keyName)
{
	for (unsigned int i = 0; i < keyNames.size(); i++)
	{
		if (keyNames[i] == keyName)
			return static_cast<OIS::KeyCode>(i);
	}
	return OIS::KC_UNASSIGNED;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

OIS::MouseButtonID getMouseButtonByButtonName(const std::string &buttonName)
{
	for (unsigned int i = 0; i < mouseButtonNames.size(); i++)
	{
		if (mouseButtonNames[i] == buttonName)
			return static_cast<OIS::MouseButtonID>(i);
	}
	return static_cast<OIS::MouseButtonID>(mouseButtonNames.size());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int getJoystickButtonByButtonName(const std::string &buttonName)
{
	for (unsigned int i = 0; i < joystickButtonNames.size(); i++)
	{
		if (joystickButtonNames[i] == buttonName)
			return i;
	}
	return joystickButtonNames.size();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int getMouseAxisByAxisName(const std::string &axisName)
{
	for (unsigned int i = 0; i < mouseAxisNames.size(); i++)
	{
		if (mouseAxisNames[i] == axisName)
			return i;
	}
	return mouseAxisNames.size();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int getJoystickAxisByAxisName(const std::string &axisName)
{
	for (unsigned int i = 0; i < joystickAxisNames.size(); i++)
	{
		if (joystickAxisNames[i] == axisName)
			return i;
	}
	return joystickAxisNames.size();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/*
unsigned int getJoystickSliderBySliderName(const std::string &sliderName)
{
for (int i = 0; i < joystickSliderNames.size(); i++)
{
if (joystickSliderNames[i] == sliderName)
return i;
}
return joystickSliderNames.size();
}
*/
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

KeyModifiers getKeyModifierByKC(OIS::KeyCode key)
{
	switch (key) {
	case OIS::KC_LSHIFT:
		return LShift;
	case OIS::KC_RSHIFT:
		return RShift;
	case OIS::KC_LCONTROL:
		return LCtrl;
	case OIS::KC_RCONTROL:
		return RCtrl;
	case OIS::KC_LMENU:
		return LAlt;
	case OIS::KC_RMENU:
		return RAlt;
	case OIS::KC_LWIN:
		return LWin;
	case OIS::KC_RWIN:
		return RWin;
	default:
		return NoMod;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

KeyModifiers InputMapper::getKeyModifierByKeyCode(OIS::KeyCode key) const
{
	return getKeyModifierByKC(key);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

KeyModifiers getKeyModifierByKN(const std::string &keyName)
{
	OIS::KeyCode kc = getKeyCodeByKeyName(keyName);
	return getKeyModifierByKC(kc);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

KeyModifiers InputMapper::getKeyModifierByKeyName(const std::string &keyName) const
{
	return getKeyModifierByKN(keyName);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool InputMapper::isModifierKey(OIS::KeyCode key) const
{
	switch (key) {
	case OIS::KC_LSHIFT:
		return true;
	case OIS::KC_RSHIFT:
		return true;
	case OIS::KC_LCONTROL:
		return true;
	case OIS::KC_RCONTROL:
		return true;
	case OIS::KC_LMENU:
		return true;
	case OIS::KC_RMENU:
		return true;
	case OIS::KC_LWIN:
		return true;
	case OIS::KC_RWIN:
		return true;
	default:
		return false;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool InputMapper::getActionFromInput(InputDeviceType deviceType, int button, unsigned int modifiers, bool pressed, ButtonBindingReturn &actionReturn, const std::string& joystickName) const
{
	unsigned int bindingListSize;
	int joystickNumber = -1;
	switch (deviceType) {
	case KEYBOARD:
		bindingListSize = mImpl->keyBindingList.size();
		break;
	case MOUSE:
		bindingListSize = mImpl->mouseButtonBindingList.size();
		break;
	case JOYSTICK:
		for (unsigned int i = 0; i<mImpl->joystickNames.size(); i++) {
			if (mImpl->joystickNames.at(i) == joystickName)
				joystickNumber = i;
		}
		if (joystickNumber >= 0)
			bindingListSize = mImpl->joystickList.at(joystickNumber)->buttonBindingList.size();
		else
			return false;
		break;
	default:
		return false;
	}

	for (unsigned int i = 0; i<bindingListSize; i++)
	{
		ButtonBinding* buttonBinding = nullptr;
		switch (deviceType) {
		case KEYBOARD:
			buttonBinding = mImpl->keyBindingList.at(i);
			break;
		case MOUSE:
			buttonBinding = mImpl->mouseButtonBindingList.at(i);
			break;
		case JOYSTICK:
			buttonBinding = mImpl->joystickList.at(joystickNumber)->buttonBindingList.at(i);
			break;
		}

		if (buttonBinding && buttonBinding->id == button)
		{
			//make sure only required modifiers are pressed
			unsigned int combinedModifierReq = 0x00;
			for (unsigned int j = 0; j<buttonBinding->modifiers.size(); j++)
			{
				combinedModifierReq |= buttonBinding->modifiers[j];
			}
			if ((modifiers ^ combinedModifierReq) == 0)
			{
				actionReturn.hold = buttonBinding->hold;

				if (actionReturn.hold) {
					if (pressed) {
						actionReturn.command = buttonBinding->downCommand;
						actionReturn.value = buttonBinding->downValue;
					}
					else {
						actionReturn.command = buttonBinding->upCommand;
						actionReturn.value = buttonBinding->upValue;
					}
				}
				else {
					if (pressed) {
						actionReturn.command = buttonBinding->pressedCommand;
						actionReturn.value = buttonBinding->pressedValue;
					}
					else {
						//not an up/down "hold" type key, so do nothing with release
						actionReturn.command = COMMAND_NULL;
						actionReturn.value = 0.0f;
						return false;
					}
				}
				return true;
			}
		}
	}
	//didn't find matching button command, send back nothing
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool InputMapper::getActionFromAxisInput(InputDeviceType deviceType, unsigned int axisID, AxisBindingReturn &actionReturn, const std::string& joystickName) const
{
	unsigned int bindingListSize;
	int joystickNumber = -1;
	switch (deviceType) {
	case MOUSE:
		bindingListSize = mImpl->mouseAxisBindingList.size();
		break;
	case JOYSTICK:
		for (unsigned int i = 0; i<mImpl->joystickNames.size(); i++) {
			if (mImpl->joystickNames.at(i) == joystickName)
				joystickNumber = i;
		}
		if (joystickNumber >= 0)
			bindingListSize = mImpl->joystickList.at(joystickNumber)->axisBindingList.size();
		else
			return false;
		break;
	default:
		return false;
	}

	for (unsigned int i = 0; i<bindingListSize; i++)
	{
		AxisBinding* axisBinding = nullptr;
		switch (deviceType) {
		case MOUSE:
			axisBinding = mImpl->mouseAxisBindingList.at(i);
			break;
		case JOYSTICK:
			axisBinding = mImpl->joystickList.at(joystickNumber)->axisBindingList.at(i);
			break;
		}

		if (axisBinding && axisBinding->id == axisID) {
			actionReturn.axisCommand = axisBinding->axisCommand;
			actionReturn.invert = axisBinding->invert;
			actionReturn.curvature = axisBinding->curvature;
			actionReturn.deadzone = axisBinding->deadzone;
			actionReturn.relative = axisBinding->relative;
			actionReturn.scalar = axisBinding->scalar;
			actionReturn.saturationXLow = axisBinding->saturationXLow;
			actionReturn.saturationYLow = axisBinding->saturationYLow;
			actionReturn.saturationXHigh = axisBinding->saturationXHigh;
			actionReturn.saturationYHigh = axisBinding->saturationYHigh;
			return true;
		}

	}
	//didn't find matching axis command, send back nothing
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void InputMapper::Impl::parseInputOptions(otCore::JSON& inputOpt, InputDeviceType inputDevice)
{
	std::vector<otCore::JSON*> deviceOptions;

	if (inputDevice == JOYSTICK) {
		//make sure to get all joystick devices options
		if (inputOpt.hasObject("Joysticks")) {
			otCore::JSON joysticksOpt;
			if (inputOpt.getObject("Joysticks", &joysticksOpt)) {
				joystickNames = joysticksOpt.getObjectList();
				for (const auto& joystickName : joystickNames) {
					otCore::JSON *joystickOpt = new otCore::JSON();
					if (joysticksOpt.getObject(joystickName, joystickOpt))
						deviceOptions.push_back(joystickOpt);
				}
			}
		}
	}
	else { //other device types only have 1 device each
		deviceOptions.push_back(&inputOpt);
	}

	//parse and store key and axis commands structures and categories in vectors
	for (auto deviceOpt : deviceOptions) {
		if (inputDevice == JOYSTICK) {
			JoystickBindings* newJoystickBinding = new JoystickBindings();
			joystickList.push_back(newJoystickBinding);
		}

		otCore::JSON keyCategories;
		otCore::JSON axisCategories;

		//get key commands first

		if (deviceOpt->getObject("KeyBindings.Categories", &keyCategories))
		{
			std::vector<std::string> categoryList = keyCategories.getObjectList();
			for (const auto& category : categoryList) {
				//add category to category list if it doesn't already exist
				if (std::find(inputCategoryList.begin(), inputCategoryList.end(), category) == inputCategoryList.end())
					inputCategoryList.push_back(category);

				std::vector<otCore::JSON*> bindings = keyCategories.getObjectArray(category);

				for (auto binding : bindings) {
					ButtonBinding *newButtonBinding = new ButtonBinding();

					newButtonBinding->displayName = binding->getValue("displayName", std::string("ERROR: NO NAME"));

					std::string keyName = binding->getValue("key", std::string("Unassigned"));

					switch (inputDevice) {
					case KEYBOARD:
						newButtonBinding->id = getKeyCodeByKeyName(keyName);
						break;
					case MOUSE:
						newButtonBinding->id = getMouseButtonByButtonName(keyName);
						break;
					case JOYSTICK:
						newButtonBinding->id = getJoystickButtonByButtonName(keyName);
						break;
					case HEADTRACKER:
						break;
					}

					//if key command uses down/up specifier, it is a "hold" type and should not use "pressed" type data
					if (binding->hasObject("downCommand")) {
						newButtonBinding->hold = true;

						newButtonBinding->downCommand = getCommandIDByName(binding->getValue("downCommand", std::string("")));
						newButtonBinding->upCommand = getCommandIDByName(binding->getValue("upCommand", std::string("")));
						newButtonBinding->downValue = binding->getValue("downValue", 1.0f);
						newButtonBinding->upValue = binding->getValue("upValue", 0.0f);
					}
					else
					{
						newButtonBinding->pressedCommand = getCommandIDByName(binding->getValue("pressedCommand", std::string("")));
						newButtonBinding->pressedValue = binding->getValue("pressedValue", 1.0f);
					}

					newButtonBinding->category = category;

					std::vector<std::string> modifiers = binding->getValueStringArray("modifiers");

					for (const auto& modifier : modifiers) {
						newButtonBinding->modifiers.push_back(getKeyModifierByKN(modifier));
					}
					modifiers.clear();

					switch (inputDevice) {
					case KEYBOARD:
						keyBindingList.push_back(newButtonBinding);
						break;
					case MOUSE:
						mouseButtonBindingList.push_back(newButtonBinding);
						break;
					case JOYSTICK:
						joystickList.back()->buttonBindingList.push_back(newButtonBinding);
						break;
					case HEADTRACKER:
						break;
					}
				}
			}
		}

		//now axis commands
		if (deviceOpt->getObject("AxisBindings.Categories", &axisCategories))
		{
			std::vector<std::string> categoryList = axisCategories.getObjectList();
			for (const auto& category : categoryList) {
				if (std::find(inputCategoryList.begin(), inputCategoryList.end(), category) == inputCategoryList.end())
					inputCategoryList.push_back(category);

				std::vector<otCore::JSON*> bindings = axisCategories.getObjectArray(category);

				for (auto binding : bindings) {
					AxisBinding *newAxisBinding = new AxisBinding();

					newAxisBinding->displayName = binding->getValue("displayName", std::string("ERROR: NO NAME"));
					std::string axisName = binding->getValue("axis", std::string("Unassigned"));

					newAxisBinding->axisCommand = getAxisCommandIDByName(binding->getValue("axisCommand", std::string("")));

					newAxisBinding->relative = binding->getValue("relative", false);
					newAxisBinding->curvature = binding->getValue("curvature", 1.0f);
					newAxisBinding->deadzone = binding->getValue("deadzone", 0.0f);
					newAxisBinding->saturationXLow = binding->getValue("saturationXLow", 1.0f);
					newAxisBinding->saturationXHigh = binding->getValue("saturationXHigh", 1.0f);
					newAxisBinding->saturationYLow = binding->getValue("saturationYLow", 1.0f);
					newAxisBinding->saturationYHigh = binding->getValue("saturationYHigh", 1.0f);
					newAxisBinding->scalar = binding->getValue("scalar", 1.0f);
					newAxisBinding->invert = binding->getValue("invert", false);

					newAxisBinding->category = category;

					switch (inputDevice) {
					case MOUSE:
						newAxisBinding->id = getMouseAxisByAxisName(axisName);
						mouseAxisBindingList.push_back(newAxisBinding);
						break;
					case JOYSTICK:
						newAxisBinding->id = getJoystickAxisByAxisName(axisName);
						joystickList.back()->axisBindingList.push_back(newAxisBinding);
						break;
					case HEADTRACKER:
						break;
					}
				}
			}
		}

	}

	//clear memory
	if (inputDevice == JOYSTICK) {
		for (unsigned int i = 0; i<deviceOptions.size(); i++) {
			delete deviceOptions[i];
		}
		deviceOptions.clear();
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void InputMapper::Impl::readInputConfiguration(void)
{
	std::string inputDir = otCore::Paths::getInputDir();

	otCore::Stopwatch loadTimer;
	for (int inputDeviceInt = KEYBOARD; inputDeviceInt <= HEADTRACKER; inputDeviceInt++) {
		InputDeviceType inputDevice = static_cast<InputDeviceType>(inputDeviceInt);

		std::string inputFile = inputDir;
		switch (inputDevice) {
		case KEYBOARD:
			inputFile += "\\keyboard\\input.json";
			break;
		case MOUSE:
			inputFile += "\\mouse\\input.json";
			break;
		case JOYSTICK:
			inputFile += "\\joystick\\input.json";
			break;
		case HEADTRACKER:
			inputFile += "\\headtracker\\input.json";
			break;
		}



		loadTimer.start();

		otCore::JSON inputCfg;
		if (inputCfg.readFile(inputFile)) {
			parseInputOptions(inputCfg, inputDevice);
		}

		//otCore::Log::message("Loading default input mapping file: %s ... %.0f ms", inputFile.c_str(), loadTimer.getElapsedTime());
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


} //namespace otInput

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%