/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       otSim.h
Author:       Uriah George & Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef otSim_H
#define otSim_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

#include "otSimPluginExport.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

typedef void(*PFN_INIT)		(void);
typedef void(*PFN_UPDATE)	  (float dt);

namespace otsim {

class OTSIM_API otSim : public otsim_wrapper::otSimPlugin
{
public:
	otSim(void);
	~otSim(void);

	static otSim& getInstance();

	void initialize(const char* pluginsDir) override;

	void updateSimulation(float dt) override;

	void updatePhysics(float dt) override;


private:
	void loadMainDll(const std::string& dllName);
	void unloadMainDll();

	///////////////////
	// MAIN DLL INFO //
	///////////////////

	bool mainDllLoaded = false;
	std::string mainDllName = "";
	HMODULE mainDllHMODULE = NULL;
	bool mainDllInitialized = false;

	PFN_INIT initializePFN = nullptr;
	PFN_UPDATE updateSimulationPFN = nullptr;
	PFN_UPDATE updatePhysicsPFN = nullptr;
};

} //namespace otsim

#endif //otSim_H