/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       InputMapper.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef InputMapper_H
#define InputMapper_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Singleton.h"

#include <string>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace OIS {
enum KeyCode : int;
}

namespace otInput {
enum InputDeviceType : int;
enum ActionKeyCommands : int;
enum ActionAxisCommands : int;
enum KeyModifiers : int;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otInput {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Button Binding Return struct

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

struct ButtonBindingReturn
{
	ActionKeyCommands command = static_cast<ActionKeyCommands>(0);
	bool hold = false;
	float value = 1.0f;
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Axis Binding Return struct

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

struct AxisBindingReturn
{
	ActionAxisCommands axisCommand = static_cast<ActionAxisCommands>(0);
	float scalar = 1.0f;
	float curvature = 0.0f;
	float deadzone = 0.0f;
	float saturationXHigh = 1.0f;
	float saturationYHigh = 1.0f;
	float saturationXLow = 1.0f;
	float saturationYLow = 1.0f;
	bool invert = false;
	bool relative = false;
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Input mapper class.  Reads input configuration file for key bindings and
maps the key combos to commands.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class InputMapper : public otCore::Singleton<InputMapper>
{
	friend class otCore::Singleton < InputMapper >;
public:

	///Reloads the input configuration files to update input mappings.
	///Called automatically at InputMapper constructor, but this function is useful
	///for subsequent changes to input mappings that need to be detected after program start.
	void reloadInputMappings();

	///Given the input device type, button, key modifiers, and pressed or unpressed status,
	///this function returns a corresponding Command ID, command value, and a flag
	///indicating whether this command is a 'hold' type.  These are output parameters in the
	///ButtonBindingReturn structure and its reference must be passed to the function.
	///The function returns false if a corresponding command could not be found.
	bool getActionFromInput(InputDeviceType deviceType, int button, unsigned int modifiers, bool pressed, ButtonBindingReturn &actionReturn, const std::string& joystickName = "") const;

	///Given the axis ID,
	///this function returns a corresponding axis command ID, curvature, deadzone, saturation, and flags
	///indicating whether this axis is inverted or is a 'relative' type.  These are output parameters
	///in the AxisBindingReturn structure and its reference must be passed to the function.
	///The function returns false if a corresponding axis command could not be found.
	bool getActionFromAxisInput(InputDeviceType deviceType, unsigned int axisID, AxisBindingReturn &actionReturn, const std::string& joystickName = "") const;

	///Returns a string of the command name corresponding to the ActionKeyCommands ID
	///Ex: COMMAND_NULL returns "COMMAND_NULL"
	std::string getCommandNameByID(ActionKeyCommands commandID) const;

	///Returns a string of the axis command name corresponding to the ActionAxisCommands ID
	///Ex: COMMAND_AXIS_NULL returns "COMMAND_AXIS_NULL"
	std::string getAxisCommandNameByID(ActionAxisCommands axisCommandID) const;

	///Returns boolean indicating whether the given KeyCode ID is a modifier type key
	bool isModifierKey(OIS::KeyCode key) const;

	///Return the key modifier value corresponding to the given KeyCode ID
	///Ex: KC_LSHIFT=0x2A returns LShift=0x01
	KeyModifiers getKeyModifierByKeyCode(OIS::KeyCode key) const;

	///Return the key modifier value corresponding to the given key name string
	///Ex: "LShift" returns LShift=0x01
	KeyModifiers getKeyModifierByKeyName(const std::string &keyName) const;

private:
	/// Constructor
	InputMapper();
	/// Destructor
	~InputMapper();

	class Impl;
	Impl* mImpl;
};


} //namespace otInput

#endif //InputMapper_H