/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       otMain.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Outerra Simulation Framework Main class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "otMain.h"

#include "otMath.h"
#include "ITime.h"
#include "AddonManager.h"
#include "WorldManager.h"
#include "Stopwatch.h"
#include "Conversions.h"
#include "Paths.h"
#include "JSON.h"
#include "Input.h"


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void initialize()
{
	otMain::Main::getInstance();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void updateSimulation(float dt)
{
	otMain::Main* main = &otMain::Main::getInstance();
	if (main) {
		main->updateSimulation(dt);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void updatePhysics(float dt)
{
	otMain::Main* main = &otMain::Main::getInstance();
	if (main) {
		main->updatePhysics(dt);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

namespace otMain {

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Main& Main::getInstance()
{
	static Main instance;
	return instance;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

otWorld::Ellipsoid* earth;

Main::Main()
{
	otCore::Paths::createGamePreferencesStructure();

	otInput::Input::getInstance().update();

	//Initialize time class
	otCore::TimeInitilizer::initialize();

	AddonManager* addonManager = &AddonManager::getInstance();
	addonManager->enumerateAddons();
	//otCore::Log::message("Number of addons found: %d", addonManager->getNumberAddons());

	otCore::Stopwatch loadTimer;

	loadTimer.start();

	//otCore::Log::message("Number of object types added to the database: %d ... %.0f ms", objectDB->getNumberObjectTypes(), loadTimer.getElapsedTime());


	//Read Celestial Object JSON files
	std::string coreCelestialBodiesPath = otCore::Paths::getAddonsDir() + "\\Core_Celestial_Bodies\\bodies";
	std::vector<std::string> celestialBodyJSONFiles = otCore::Paths::findFilesInFolder(coreCelestialBodiesPath, "json", true);
	for (unsigned int i = 0; i < celestialBodyJSONFiles.size(); i++) {
		otWorld::WorldManager::getInstance().parseCelestialBodyConfig(celestialBodyJSONFiles.at(i));
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Main::~Main()
{

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Main::updateSimulation(float dt)
{
	//Poll input devices at the graphics frame rate
	otInput::Input::getInstance().update();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Main::updatePhysics(float dt)
{
	//Run time forward
	otCore::globalTime->update();

	//Update Solar System
	otWorld::WorldManager::getInstance().update();
}

} //namespace otMain

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%