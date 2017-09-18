/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       otSimWrapper.cpp
Author:       Uriah George & Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Outerra API Simulation Wrapper Class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "otSimWrapper.hpp"


#include <comm\dir.h>
#include <comm\log\logger.h>

#include "otSimPlugin.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

using namespace otsim_wrapper;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

namespace otsim_wrapper
{
///Replace all instances of substring 'what' within string 's' with string 'with'
std::string stringReplaceAll(std::string s, const std::string &what, const std::string &with)
{
	size_t pos;
	do
	{
		pos = s.find(what);
		if (pos != -1)  s.replace(pos, what.length(), with);
	} while (pos != -1);
	return s;
}

///Returns file extension without dot from the given full path
std::string getFileExtension(const std::string& filePath)
{
	size_t lastdot = filePath.find_last_of(".");

	//if a dot was not found OR it was the last character, fail
	if (lastdot == std::string::npos || lastdot == (filePath.length() - 1))
		return "";

	//make sure the dot wasn't related to "../" or "./" relative directory commands
	if (filePath.at(lastdot - 1) == '.' || filePath.at(lastdot + 1) == '/')
		return "";

	return filePath.substr(lastdot + 1);
}

///Adds the given file extension to the given file
std::string addExtension(const std::string& file, const std::string& extension)
{
	if (extension.length() > 0) {
		if (extension[0] == '.')
			return (file + extension);
		else
			return (file + "." + extension);
	}
	else
		return file;
}
} //namespace otsim_wrapper

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///Constructor
otSimWrapper::otSimWrapper()
{
	m_env = ot::environment::get();

	coid::charstr exeDir;
	exeDir = coid::directory::get_ped(exeDir);
	pluginsDir = (exeDir + "plugins\\otSim\\").c_str();
	pluginsDir = stringReplaceAll(pluginsDir, "/", "\\");
	std::string simDllPath = pluginsDir + "otSim.dll";
	//SetDllDirectory();

	//coidlog_info("", "otSimWrapper()");

	loadSimPlugin(simDllPath);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///Destructor
otSimWrapper::~otSimWrapper()
{
	if (simPlugin)
		unloadSimPlugin();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///Update simulation (global)
void otSimWrapper::updateSimulation(float dt)
{
	if (simPlugin) {
		simPlugin->updateSimulation(dt);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///Update physics (global)
void otSimWrapper::updatePhysics(float dt)
{
	if (simPlugin) {
		simPlugin->updatePhysics(dt);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSimWrapper::set_time_of_day(double timeOfDay)
{
	m_env->set_time(m_env->get_day_of_year(), timeOfDay);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSimWrapper::set_timeflow_multiplier(float timeAccel)
{
	m_env->set_time(m_env->get_day_of_year(), m_env->get_time_of_day(), timeAccel);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSimWrapper::set_day_of_year(int dayOfYear)
{
	m_env->set_time(static_cast<coid::int64>(dayOfYear), m_env->get_time_of_day());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double otSimWrapper::get_time_of_day()
{
	return m_env->get_time_of_day();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

float otSimWrapper::get_timeflow_multiplier()
{
	return m_env->get_timeflow_multiplier();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int otSimWrapper::get_day_of_year()
{
	return static_cast<int>(m_env->get_day_of_year());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otSimWrapper::loadSimPlugin(const std::string& pluginName)
{
	if (simPlugin)
		unloadSimPlugin();

	std::string pluginNameStr = stringReplaceAll(pluginName, "/", "\\");

	if (getFileExtension(pluginNameStr) == "") {
		pluginNameStr = addExtension(pluginNameStr, ".dll");
	}

	// Try to load the plugin library
	HMODULE hModule = LoadLibrary(pluginNameStr.c_str());
	if (hModule)
	{
		fnCreatePlugin CreatePlugin = (fnCreatePlugin)GetProcAddress(hModule, "CreatePlugin");
		if (CreatePlugin)
		{
			// Invoke the function to get the plugin from the DLL.
			simPlugin = CreatePlugin();

			if (simPlugin)
			{
				simPlugin->initialize(pluginsDir.c_str());

				simPluginName = pluginNameStr;
				simPluginHMODULE = hModule;
			}
			else
			{
				//Core::FSLog::error("Could not load plugin from %s", pluginNameStr.c_str());
				// Unload the library.
				FreeLibrary(hModule);
			}
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

void otSimWrapper::unloadSimPlugin()
{
	if (simPlugin)
	{
		fnDestroyPlugin DestroyPlugin = (fnDestroyPlugin)GetProcAddress(simPluginHMODULE, "DestroyPlugin");
		if (DestroyPlugin)
		{
			DestroyPlugin();
		}
		else
		{
			//Core::FSLog::error("Unable to find symbol \"DestroyPlugin\" in library: %s", simPluginName.c_str());
			//Core::FSLog::warning("Memory leaks may occur.");
		}
		// Unload the library
		FreeLibrary(simPluginHMODULE);
		// NULL the plugin
		simPlugin = nullptr;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%