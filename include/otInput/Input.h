/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Input.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Input_H
#define Input_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Singleton.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef INPUT_EXPORTS
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif

namespace otInput {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Input class using OIS library wrapper around Microsoft DirectInput API.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class INPUT_API Input : public otCore::Singleton<Input>
{
	friend class otCore::Singleton < Input >;
public:

	///Set the window handle for the application so that inputs can be properly captured
	void setHandle(HWND handle);

	///Main update function to call each frame to capture all inputs since the last call
	void update();

	///Returns the number of detected input devices (only one mouse and one keyboard allowed)
	unsigned int getNumDevices() const;

	///Enumerates the input devices currently active.
	///This function is already called at initialization, but this function is useful
	///for subsequent changes to active input devices on the computer after the program has started.
	void enumerateDevices();

	///Reloads the input configuration files to update input mappings.
	///Called automatically at initialization, but this function is useful
	///for subsequent changes to input mappings that need to be detected after program start.
	void reloadInputMappings();

	/*
	void getDeviceNames(std::vector<std::string>& devices);
	void getInstanceGUID(std::vector<GUID>& devices);
	void getProductGUID(std::vector<GUID>& devices);

	void getAxisNames(std::vector<std::string>& axisNames, const GUID &guidID);
	*/

private:
	/// Constructor
	Input();
	/// Destructor
	~Input();

	class Impl;
	Impl* mImpl = nullptr;
};


} //namespace otInput

#endif //Input_H