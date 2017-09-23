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

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void initialize()
{
	otMain::Main::getInstance();
}

void updateSimulation(float dt)
{
	otMain::Main* main = &otMain::Main::getInstance();
	if (main) {
		main->updateSimulation(dt);
	}
}

void updatePhysics(float dt)
{
	otMain::Main* main = &otMain::Main::getInstance();
	if (main) {
		main->updatePhysics(dt);
	}
}

namespace otMain {

Main& Main::getInstance()
{
	static Main instance;
	return instance;
}

Main::Main()
{
	otCore::TimeInitilizer::initialize();
}

Main::~Main()
{

}

void Main::updateSimulation(float dt)
{

}

void Main::updatePhysics(float dt)
{
	otCore::globalTime->update();
}

} //namespace otMain

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%