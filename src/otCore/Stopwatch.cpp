/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Stopwatch.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Stopwatch class for measuring real time.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Stopwatch.h"
#include <time.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

class Stopwatch::Impl
{
public:
	Impl::Impl()
	{

	}

	clock_t startTime = 0;
	clock_t currentTime = 0;
	clock_t elapsedTime = 0;
	bool stopped = true;
	bool started = false;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Stopwatch::Stopwatch(void) : mImpl(new Stopwatch::Impl())
{

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Stopwatch::~Stopwatch()
{
	delete mImpl;
	mImpl = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Stopwatch::start()
{
	mImpl->startTime = clock();
	mImpl->started = true;
	mImpl->stopped = false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Stopwatch::stop()
{
	//only stop it once and only if it has been started
	if (mImpl->started && !mImpl->stopped) {
		//make sure time is saved when stopping
		mImpl->currentTime = clock();
		mImpl->elapsedTime = mImpl->currentTime - mImpl->startTime;
		mImpl->stopped = true;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double Stopwatch::getElapsedTime(TimeUnits tu)
{
	//make sure stopwatch was started
	if (mImpl->started) {
		//if stopped, don't calculate new elapsed time
		if (mImpl->started && !mImpl->stopped) {
			mImpl->currentTime = clock();
			mImpl->elapsedTime = mImpl->currentTime - mImpl->startTime;
		}

		switch (tu) {
		case TimeUnits::MILLISECONDS:
			return double(mImpl->elapsedTime);
			break;
		case TimeUnits::SECONDS:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC;
			break;
		case TimeUnits::MINUTES:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC / 60.0;
			break;
		case TimeUnits::HOURS:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC / 3600.0;
			break;
		case TimeUnits::DAYS:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC / 86400.0;
			break;
		case TimeUnits::WEEKS:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC / 86400.0 / 7.0;
			break;
		case TimeUnits::YEARS:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC / 86400.0 / 365.25;
			break;
		default:
			return double(mImpl->elapsedTime) / CLOCKS_PER_SEC;
			break;
		};
	}
	else return 0;
}

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%