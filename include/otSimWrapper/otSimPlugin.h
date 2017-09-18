/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       otSimPlugin.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef otSimPlugin_H
#define otSimPlugin_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef IGC_PLUGIN_EXPORTS
#define OTSIMWRAPPER_API __declspec(dllexport)
#else
#define OTSIMWRAPPER_API __declspec(dllimport)
#endif

namespace otsim_wrapper {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** otSimPlugin base class.

In order to use this base class, you must write a plugin class that derives from
otSimPlugin and then export two functions, CreatePlugin and DestroyPlugin as
shown below:

class YourPluginClass : public otsim_wrapper::otSimPlugin
{
... implement virtual functions here ...
};

YourPluginClass* instance = nullptr;

extern "C" __declspec(dllexport) otsim_wrapper::otSimPlugin* CreatePlugin(void)
{
instance = new YourPluginClass();
return instance;
}

extern "C" __declspec(dllexport) void DestroyPlugin(void)
{
if (instance)
delete instance;
instance = nullptr;
}

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class OTSIMWRAPPER_API otSimPlugin
{
public:

	//////////////////////////////////////////////////////////
	// VIRTUAL FUNCTIONS - MUST BE DEFINED BY DERIVED CLASS //
	//////////////////////////////////////////////////////////

	/// Initialization function called before the first iteration
	virtual void initialize(const char* pluginsDir) = 0;

	/// Main loop function called on each graphics frame iteration
	/// @param dt Time delta for graphics (s)
	virtual void updateSimulation(float dt) = 0;

	/// Main loop physics update function called on each iteration
	/// @param dt Time delta for physics simulation (s)
	virtual void updatePhysics(float dt) = 0;

	////////////////////
	// TIME FUNCTIONS //
	////////////////////

	void setTimeOfDay(double timeOfDay);
	void setTimeAcceleration(float timeAccel = 1.f);
	void setDayOfYear(int dayOfYear);

	double getTimeOfDay();
	float getTimeAcceleration();
	int getDayOfYear();



	///////////////////////////
	// NOT USED BY 3RD PARTY //
	///////////////////////////

	/// Constructor
	otSimPlugin(void) {}

	/// Destructor
	virtual ~otSimPlugin(void) {}


private:
	// Make this object be noncopyable
	otSimPlugin(const otSimPlugin&);
	const otSimPlugin &operator =(const otSimPlugin &);
};


} //namespace otsim_wrapper

#endif //otSimPlugin_H