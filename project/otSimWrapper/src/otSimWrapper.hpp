/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       otSimWrapper.hpp
Author:       Uriah George & Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#pragma once

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

#include <comm/intergen/ifc.h>
#include "ifc/otSimWrapperInterface.h"
#include "ifc/otSimWrapperInterface.js.h"
//#include <ot/geomob.h>

///INTERGEN IFC
//ifc{
#ifndef OT_SIM_WRAPPER_HPP
#include <ot/vehicle_physics.h>
#include <ot/aircraft_physics.h>
#include <ot/environment.h>
//#include <ot/logger.h>
#endif
//}ifc

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otsim_wrapper {
class otSimPlugin;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

// Define the prototype for a function that should exist in the DLL
// that is used to create and return the plugin type in the DLL.
typedef otsim_wrapper::otSimPlugin* (*fnCreatePlugin)(void);
// Destroys the plugin type from the DLL before the library is unloaded.
typedef void(*fnDestroyPlugin)(void);

namespace otsim_wrapper {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Outerra API Simulation Wrapper Class

@author Uriah George & Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class otSimWrapper :
	public policy_intrusive_base
{
private:

	///Noncopyable
	otSimWrapper(const otSimWrapper&);
public:

	///Constructor declaration
	otSimWrapper();

	///Destructor
	~otSimWrapper();

	////////////////////////////////////
	// INTERGEN INTERFACE DECLARATION //
	////////////////////////////////////

	///[namespace::]name, path
	ifc_class(sf::otSimWrapperInterface, "ifc/");

	////////////////////////////////
	// INTERGEN INTERFACE CREATOR //
	////////////////////////////////

	ifc_fnx(get) static iref<otsim_wrapper::otSimWrapper> get() {
		static otSimWrapper instance;

		return &instance;
	}

	///////////////
	// FUNCTIONS //
	///////////////

	///Update simulation (global)
	ifc_fn void updateSimulation(float dt);

	///Update physics (global)
	ifc_fn void updatePhysics(float dt);


	///////////////////
	// SET FUNCTIONS //
	///////////////////

	//Time
	ifc_fn void set_time_of_day(double timeOfDay);
	ifc_fn void set_timeflow_multiplier(float timeAccel = 1.f);
	ifc_fn void set_day_of_year(int dayOfYear);

	///////////////////
	// GET FUNCTIONS //
	///////////////////

	//Time
	ifc_fn double get_time_of_day();
	ifc_fn float get_timeflow_multiplier();
	ifc_fn int get_day_of_year();

private:
	///Load the Simulation Plugin with the given plugin dll path
	void loadSimPlugin(const std::string& pluginName);

	///Unload the currently loaded Simulation Plugin
	void unloadSimPlugin();

	//////////////
	// POINTERS //
	//////////////

	iref<ot::geomob> m_geomob;
	iref<ot::sndgrp> m_sndgrp;
	iref<ot::vehicle_physics> m_vehicle_physics;
	iref<ot::aircraft_physics> m_aircraft_physics;
	iref<ot::jsb> m_jsb;
	iref<ot::environment> m_env;
	//iref<ot::logger> m_logger;

	/////////////////
	// PLUGIN INFO //
	/////////////////

	std::string pluginsDir;

	otSimPlugin* simPlugin = nullptr;
	std::string simPluginName = "";
	HMODULE simPluginHMODULE = NULL;
};


} //namespace otsim_wrapper