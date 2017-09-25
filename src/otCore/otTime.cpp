/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       otTime.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Time class for the simulator.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "otTime.h"

#include "otSim.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <ctime>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


namespace otCore
{

ITime* globalTime = nullptr;

static const int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void TimeInitilizer::initialize()
{
	globalTime = &otCore::otTime::getInstance();
}

otTime::otTime()
{

}

otTime::~otTime()
{
	globalTime = nullptr;
}

void otTime::update()
{
	//NOTE: Need ot::world interface to properly pause simulation

	//NOTE: OT does not yet support years, so fixed at 2017 for now
	timeAcceleration = otsim::otSim::getInstance().getTimeAcceleration();
	simTimeOfDay = otsim::otSim::getInstance().getTimeOfDay() / 1000.0;
	simDate.dayOfYear = otsim::otSim::getInstance().getDayOfYear();

	int month, day;
	TimeUtility::calculateMonthDayFromDayOfYear(simDate.dayOfYear, simDate.year, month, day);
	simDate.month = (unsigned int)month;
	simDate.day = (unsigned int)day;

	double tmp;
	simDate.hour = (unsigned int)(floor(simTimeOfDay / 3600.0));
	tmp = simTimeOfDay - simDate.hour*3600.0;
	simDate.minute = (unsigned int)(floor(tmp / 60.0));
	tmp -= simDate.minute*60.0;
	simDate.second = (unsigned int)(tmp);
	simDate.seconds = (float)tmp;

	simJulianDate = TimeUtility::getJulianDateFromUTCTime(simDate);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otTime::setTimeAcceleration(float timeAccel)
{
	if (timeAccel > 0.00001f && timeAccel <= 10000.0) {
		otsim::otSim::getInstance().setTimeAcceleration(timeAccel);
	}
}

void otTime::setSimTimeOfDay(double simTimeDay)
{
	if (simTimeDay >= 0 && simTimeDay < 86400)
	{
		otsim::otSim::getInstance().setTimeOfDay(simTimeDay*1000.0);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void otTime::setSimDate(const Date& simDate_)
{
	simDate = simDate_;

	simDate.seconds = (simDate.seconds < 0) ? 0 : (simDate.seconds >= 60.f) ? 60.f : simDate.seconds;
	simDate.second = (simDate.second < 0) ? 0 : (simDate.second > 59) ? 59 : simDate.second;
	simDate.minute = (simDate.minute < 0) ? 0 : (simDate.minute > 59) ? 59 : simDate.minute;
	simDate.hour = (simDate.hour < 0) ? 0 : (simDate.hour > 23) ? 23 : simDate.hour;
	simDate.month = (simDate.month < 1) ? 1 : (simDate.month > 12) ? 12 : simDate.month;

	unsigned int daysInMonth = (unsigned int)TimeUtility::getNumberOfDaysInMonth(simDate.year, simDate.month);

	simDate.day = (simDate.day < 1U) ? 1U : (simDate.day > daysInMonth) ? daysInMonth : simDate.day;

	simTimeOfDay =
		simDate.hour*3600.0 +
		simDate.minute*60.0 +
		simDate.seconds;

	simDate.dayOfYear = TimeUtility::calculateDayOfYear(simDate.year, simDate.month, simDate.day);
	otsim::otSim::getInstance().setDayOfYear(simDate.dayOfYear);
	otsim::otSim::getInstance().setTimeOfDay(simTimeOfDay);

	simJulianDate = TimeUtility::getJulianDateFromUTCTime(simDate);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

float otTime::getTimeAcceleration(void) const
{
	return timeAcceleration;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Date& otTime::getSimDate(void) const
{
	return simDate;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double otTime::getSimJulianDate(void) const
{
	return simJulianDate;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double otTime::getSimTimeOfDay(void) const
{
	return simTimeOfDay;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Date& otTime::getRealDate(void)
{
	time_t rawTime;
	struct tm * localTime;
	//int tm_sec;    seconds after the minute - [0,59]
	//int tm_min;    minutes after the hour - [0,59]
	//int tm_hour;   hours since midnight - [0,23]
	//int tm_mday;   day of the month - [1,31]
	//int tm_mon;    months since January - [0,11]
	//int tm_year;   years since 1900
	//int tm_wday;   days since Sunday - [0,6]
	//int tm_yday;   days since January 1 - [0,365]
	//int tm_isdst;  daylight savings time flag

	time(&rawTime);
	localTime = localtime(&rawTime);
	//zulu_timeinfo = gmtime ( &rawTime );

	realLocalDate.day = localTime->tm_mday;
	//realLocalDate.dayOfYear = localTime->tm_yday + 1;
	realLocalDate.hour = localTime->tm_hour;
	realLocalDate.minute = localTime->tm_min;
	realLocalDate.month = localTime->tm_mon + 1;
	realLocalDate.second = localTime->tm_sec;
	realLocalDate.year = localTime->tm_year + 1900;

	realLocalDate.seconds = (float)realLocalDate.second;
	return realLocalDate;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool TimeUtility::isLeapYear(int year)
{
	bool is_a_leap_year = false;

	if ((year % 4) == 0)
	{
		is_a_leap_year = true;
		if ((year % 100) == 0)
		{
			if ((year % 400) == 0)
			{
				is_a_leap_year = true;
			}
			else
			{
				is_a_leap_year = false;
			}
		}
	}

	return is_a_leap_year;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double TimeUtility::getJulianDateFromUTCTime(const Date& date)
{
	double y = 0;
	double m = 0;
	if (date.month <= 2) {
		y = date.year - 1;
		m = date.month + 12;
	}
	else {
		y = date.year;
		m = date.month;
	}

	double JD = (int)(365.25*y) + (int)(30.6001*(m + 1.0)) + (double)date.day + date.hour / 24.0 + date.minute / 1440.0 + date.seconds / 86400.0 + 1720981.5;

	return JD;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int TimeUtility::getNumberOfDaysInMonth(int year, int month)
{
	int days_in_month = 0;

	month = month < 1 ? 1 : month > 12 ? 12 : month;
	int monthIndx = month - 1;

	if (month == 2 && isLeapYear(year)) {
		days_in_month = daysInMonth[monthIndx] + 1;
	}
	else {
		days_in_month = daysInMonth[monthIndx];
	}

	return days_in_month;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int TimeUtility::getNumberOfDaysInYear(int year)
{
	if (isLeapYear(year)) {
		return 366;
	}

	return 365;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int TimeUtility::calculateDayOfYear(int year, int month, int dayOfMonth)
{
	int monthIndx = (month < 1 ? 1 : month > 12 ? 12 : month) - 1;
	int daysBeforeMonth = 0;
	while (monthIndx > 0) {
		int prevMonthIndx = monthIndx - 1;
		if (prevMonthIndx == 2) {
			daysBeforeMonth += getNumberOfDaysInMonth(year, prevMonthIndx);
		}
		else {
			daysBeforeMonth += daysInMonth[prevMonthIndx];
		}

		monthIndx--;
	}

	return (daysBeforeMonth + dayOfMonth);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void TimeUtility::calculateMonthDayFromDayOfYear(int dayOfYear, int year, int& month, int& day)
{
	int daysInYear = isLeapYear(year) ? 366 : 365;

	dayOfYear = dayOfYear < 1 ? 1 : dayOfYear > daysInYear ? daysInYear : dayOfYear;

	int daysBeforeMonth = 0;
	int daysAfterMonth = 0;
	for (int monthIdx = 0; monthIdx < 12; monthIdx++)
	{
		int thisMonth = monthIdx + 1;
		if (thisMonth == 2) {
			daysAfterMonth += getNumberOfDaysInMonth(year, thisMonth);
		}
		else {
			daysAfterMonth += daysInMonth[monthIdx];
		}

		if (dayOfYear <= daysAfterMonth)
		{
			month = monthIdx + 1;
			day = dayOfYear - daysBeforeMonth;
			break;
		}
		daysBeforeMonth = daysAfterMonth;
	}
}

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%