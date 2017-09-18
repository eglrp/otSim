/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       otSim.cpp
Author:       Uriah George & Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
OT Simulation Plugin Implementation

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "otSim.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

using namespace otsim;

otSim* otsPlugin = nullptr;

otSim::otSim()
{
	otsPlugin = this;
}

otSim::~otSim()
{
	otsPlugin = nullptr;
	unloadMainDll();
}

otSim& otSim::getInstance()
{
	return (*otsPlugin);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSim::initialize(const char* pluginsDir)
{
	loadMainDll(std::string(pluginsDir) + "otMain.dll");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSim::updateSimulation(float dt)
{
	if (updateSimulationPFN) updateSimulationPFN(dt);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSim::updatePhysics(float dt)
{
	if (updatePhysicsPFN) updatePhysicsPFN(dt);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSim::loadMainDll(const std::string& dllName)
{
	if (mainDllLoaded)
		unloadMainDll();

	mainDllInitialized = false;

	// Try to load the plugin library
	HMODULE hModule = LoadLibrary(dllName.c_str());
	if (hModule)
	{
		initializePFN = (PFN_INIT)GetProcAddress(hModule, "initialize");
		updateSimulationPFN = (PFN_UPDATE)GetProcAddress(hModule, "updateSimulation");
		updatePhysicsPFN = (PFN_UPDATE)GetProcAddress(hModule, "updatePhysics");

		if (initializePFN && updateSimulationPFN && updatePhysicsPFN)
		{
			// Invoke the function to get the plugin from the DLL.
			mainDllLoaded = true;

			initializePFN();
			mainDllName = dllName;
			mainDllHMODULE = hModule;
		}
		else
		{
			//Core::FSLog::error("Could not find symbol \"CreatePlugin\" in %s", pluginNameStr.c_str());
			FreeLibrary(hModule);
		}
	}
	else
	{
		//Core::FSLog::warning(GetLastErrorAsString().c_str());
		//Core::FSLog::error("Could not load library: %s", pluginNameStr.c_str());
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSim::unloadMainDll()
{
	if (mainDllLoaded)
	{
		// Unload the library
		FreeLibrary(mainDllHMODULE);
		// NULL the plugin
		mainDllLoaded = false;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%