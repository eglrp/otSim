/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       ITime.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef ITime_H
#define ITime_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

namespace otCore {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Date structure

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

struct CORE_API Date
{
	void reset() {
		second = minute = hour = 0;
		day = dayOfYear = month = 1;
		year = 2017;
		seconds = 0.f;
	}
	unsigned int second = 0;
	unsigned int minute = 0;
	unsigned int hour = 0;
	unsigned int day = 1;
	unsigned int dayOfYear = 1;
	unsigned int month = 1;
	unsigned int year = 2017;

	float seconds = 0.f;
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Interface class for time.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class ITime
{
public:

	/////////////////////
	//  Get Functions  //
	/////////////////////

	///Returns the current simulation time acceleration multiplier
	virtual float getTimeAcceleration(void) const = 0;

	///Returns the current simulation time in seconds since simulation start.
	///Pause & time acceleration affect this time value.
	//double getSimTime(void) const;

	///Returns the current simulation time of day in seconds from midnight
	virtual double getSimTimeOfDay(void) const = 0;

	///Returns the current simulation date in the FSDate structure, which
	///includes the Year, Month, Day, Hour, Minute, and Second.
	virtual const Date& getSimDate(void) const = 0;

	///Returns the Julian date representing the number of days since noon
	///Universal Time on January 1, 4713 BC
	virtual double getSimJulianDate(void) const = 0;

	/////Returns the current real world date (not simulated) in the FSDate structure,
	/////which includes the Year, Month, Day, Hour, Minute, and Second.
	virtual const Date& getRealDate(void) = 0;

	/////////////////////
	//  Set Functions  //
	/////////////////////

	/////Set the current simulation time acceleration multiplier
	virtual void setTimeAcceleration(float timeAccel = 1.f) = 0;

	/////Set the current simulation date in the FSDate structure, which
	/////includes the Year, Month, Day, Hour, Minute, and Second.
	virtual void setSimDate(const Date& simDate) = 0;

	/////Set the current simulation time of day in seconds
	virtual void setSimTimeOfDay(double simTimeDay) = 0;

	///////////////////
	//  DO NOT CALL  //
	///////////////////

	virtual void update() = 0;

	virtual ~ITime() {}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Utility Time Class

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class CORE_API TimeUtility
{
public:

	/////////////////////////
	//  Utility Functions  //
	/////////////////////////

	///Returns the day of the year given the year, month, and day of the month
	static int calculateDayOfYear(int year, int month, int dayOfMonth);

	///Given the day of the year and the year, returns by value the month and day of the month
	static void calculateMonthDayFromDayOfYear(int dayOfYear, int year, int& month, int& day);

	///Returns the number of days in the year given the year
	static int getNumberOfDaysInYear(int year);

	///Returns the number of days in the month given the year and month
	static int getNumberOfDaysInMonth(int year, int month);

	///Returns boolean of given year being a leap year
	static bool isLeapYear(int year);

	///Computes the Julian date from the given UTC time
	static double getJulianDateFromUTCTime(const Date& date);

};


class CORE_API TimeInitilizer
{
public:
	static void initialize();
};

extern CORE_API ITime* globalTime;

} //namespace otCore

#endif //ITime_H