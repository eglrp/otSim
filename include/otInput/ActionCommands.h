/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       ActionCommands.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef ActionCommands_H
#define ActionCommands_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define NUM_KEYBOARD_KEYS 256
#define NUM_MOUSE_BUTTONS 8
#define NUM_MOUSE_AXES 3
#define NUM_JOYSTICK_AXES 8
#define NUM_JOYSTICK_SLIDERS 4
#define NUM_JOYSTICK_BUTTONS 32
#define NUM_JOYSTICK_POVS 4
#define NUM_POV_DIRECTIONS 8

namespace otInput {

///Fixed Command IDs
enum ActionKeyCommands
{
	COMMAND_NULL,							//No action
	COMMAND_VIEW_FIRST_PERSON,				//First person view of player
	COMMAND_VIEW_EXTERNAL_ORBIT,			//External view of player/aircraft - free to orbit around center
	COMMAND_VIEW_EXTERNAL_FLYBY,			//Flyby camera of player/aircraft
	COMMAND_VIEW_EXTERNAL_FIXED,			//External view of player/aircraft - fixed in place, able to rotate view
	COMMAND_VIEW_EXTERNAL_PADLOCK,			//External view of player/aircraft - padlocks nearest player/aircraft
	COMMAND_VIEW_EXTERNAL_CHASE,			//External view of player/aircraft - chase
	COMMAND_VIEW_EXTERNAL_TOWER,			//View from the tower camera, free moving
	COMMAND_VIEW_EXTERNAL_FLYBY_SWITCH,		//Switch the current camera to a flyby camera
	COMMAND_VIEW_EXTERNAL_TOWER_SWITCH,		//Switch the current camera to a tower free camera
	COMMAND_VIEW_ROTATE_STOP,				//Stop camera rotation
	COMMAND_VIEW_ROTATE_LEFT,				//Rotate camera view to the left
	COMMAND_VIEW_ROTATE_RIGHT,				//Rotate camera view to the right
	COMMAND_VIEW_ROTATE_UP,					//Rotate camera view upward
	COMMAND_VIEW_ROTATE_DOWN,				//Rotate camera view downward
	COMMAND_VIEW_ROTATE_UP_LEFT,			//Rotate camera view up and to the left
	COMMAND_VIEW_ROTATE_UP_RIGHT,			//Rotate camera view up and to the right
	COMMAND_VIEW_ROTATE_DOWN_LEFT,			//Rotate camera view down and to the left
	COMMAND_VIEW_ROTATE_DOWN_RIGHT,			//Rotate camera view down and to the right
	COMMAND_VIEW_ROTATE_LEFT_FAST,			//Rotate camera view to the left fast
	COMMAND_VIEW_ROTATE_RIGHT_FAST,			//Rotate camera view to the right fast
	COMMAND_VIEW_ROTATE_UP_FAST,			//Rotate camera view upward fast
	COMMAND_VIEW_ROTATE_DOWN_FAST,			//Rotate camera view downward fast
	COMMAND_VIEW_ROTATE_UP_LEFT_FAST,		//Rotate camera view up and to the left fast
	COMMAND_VIEW_ROTATE_UP_RIGHT_FAST,		//Rotate camera view up and to the right fast
	COMMAND_VIEW_ROTATE_DOWN_LEFT_FAST,		//Rotate camera view down and to the left fast
	COMMAND_VIEW_ROTATE_DOWN_RIGHT_FAST,	//Rotate camera view down and to the right fast
	COMMAND_VIEW_CENTER,					//Center the camera
	COMMAND_VIEW_MOVE_STOP,					//Stop the camera movement
	COMMAND_VIEW_MOVE_FORWARD,				//Move the camera forward
	COMMAND_VIEW_MOVE_BACKWARD,				//Move the camera backward
	COMMAND_VIEW_MOVE_FORWARD_FAST,			//Move the camera forward fast
	COMMAND_VIEW_MOVE_BACKWARD_FAST,		//Move the camera backward fast
	COMMAND_VIEW_ZOOM_STOP,					//Stop the camera zooming
	COMMAND_VIEW_ZOOM_IN,					//Zoom in the view
	COMMAND_VIEW_ZOOM_OUT,					//Zoom out the view
	COMMAND_VIEW_ZOOM_RESET,				//Reset the view zoom to normal
	COMMAND_VIEW_PADLOCK,					//
	COMMAND_VIEW_UNPADLOCK,					//
	COMMAND_INCREASE_SIMULATION_RATE,		//Increase the simulation rate
	COMMAND_DECREASE_SIMULATION_RATE,		//Decrease the simulation rate
	COMMAND_NORMAL_SIMULATION_RATE,			//Set simulation rate to normal (1.0)
	COMMAND_PAUSE_SIMULATION,				//Pause the simulation
	COMMAND_QUIT,							//Exit the simulation
	COMMAND_INFO_TOGGLE,					//Toggle the flight information on/off on the screen
	COMMAND_SCREENSHOT,						//Take a screenshot
	COMMAND_FPS,							//Toggle display of FPS and other info
	COMMAND_NETWORK_WINDOW,					//Open/Close the Network window
	COMMAND_WHEEL_BRAKE_ALL,				//Wheel brakes (all) value set with up/down values
	COMMAND_WHEEL_BRAKE_LEFT,				//Wheel brakes (left) value set with up/down values
	COMMAND_WHEEL_BRAKE_RIGHT,				//Wheel brakes (right) value set with up/down values
	COMMAND_TEST,							//This action is a general test command for development

	//DO NOT CHANGE
	COMMAND_MAXIMUM,
};

///Fixed Axis Command IDs
enum ActionAxisCommands
{
	COMMAND_AXIS_NULL,						//No action
	COMMAND_AXIS_VIEW_ROTATE_HORIZONTAL,	//Camera horizontal rotation axis
	COMMAND_AXIS_VIEW_ROTATE_VERTICAL,		//Camera vertical rotation axis
	COMMAND_AXIS_VIEW_ROTATE_ROLL,			//Camera roll rotation axis
	COMMAND_AXIS_VIEW_MOVE_LONGITUDINAL,	//Camera movement in the longitudinal axis
	COMMAND_AXIS_VIEW_MOVE_LATERAL,			//Camera movement in the lateral axis
	COMMAND_AXIS_VIEW_MOVE_VERTICAL,		//Camera movement in the vertical axis
	COMMAND_AXIS_CONTROL_PITCH,				//Controls pitch axis
	COMMAND_AXIS_CONTROL_ROLL,				//Controls roll  axis
	COMMAND_AXIS_CONTROL_YAW,				//Controls yaw axis
	COMMAND_AXIS_CONTROL_THROTTLE,			//Controls throttle axis

	//DO NOT CHANGE
	COMMAND_AXIS_MAXIMUM,
};

enum KeyModifiers
{
	NoMod = 0x00,
	LShift = 0x01,
	RShift = 0x02,
	LCtrl = 0x04,
	RCtrl = 0x08,
	LAlt = 0x10,
	RAlt = 0x20,
	LWin = 0x40,
	RWin = 0x80
};

enum InputDeviceType
{
	KEYBOARD = 0,
	MOUSE,
	JOYSTICK,
	HEADTRACKER,
};

} //namespace otInput

#endif //ActionCommands_H