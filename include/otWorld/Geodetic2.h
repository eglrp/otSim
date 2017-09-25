/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Geodetic2.h
Author:       Patrick Cozzi and Deron Ohlarik and Kevin Ring & adapted by Cory Parks
Date started: 09/2017

Copyright (C) 2009,2017  Patrick Cozzi, Deron Ohlarik, Kevin Ring, Cory Parks

C# code distributed under the MIT License.
https://github.com/virtualglobebook/OpenGlobe
http://www.opensource.org/licenses/mit-license.php.
For C++ converted version see LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Geodetic2_H
#define Geodetic2_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <cmath>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef WORLD_EXPORTS
#define WORLD_API __declspec(dllexport)
#else
#define WORLD_API __declspec(dllimport)
#endif

namespace otWorld {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Models the Geodetic container (latitude, longitude).

@author Patrick Cozzi and Deron Ohlarik and Kevin Ring & adapted by Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class WORLD_API Geodetic2
{
public:
	/// Constructor
	Geodetic2(void)
	{
		_latitude = 0.0;
		_longitude = 0.0;
	}

	/// Constructor
	Geodetic2(double latitude, double longitude)
	{
		_latitude = latitude;
		_longitude = longitude;
	}

	/// Copy Constructor
	Geodetic2(const Geodetic2& geodetic)
	{
		_latitude = geodetic.getLatitude();
		_longitude = geodetic.getLongitude();
	}

	/// Destructor
	~Geodetic2() {}

	/// Initialization (zero)
	void Init(void)
	{
		_longitude = 0.0;
		_latitude = 0.0;
	}

	/// Assignment operator
	Geodetic2& operator=(const Geodetic2& geodetic)
	{
		_latitude = geodetic.getLatitude();
		_longitude = geodetic.getLongitude();
		return *this;
	}

	/// Return the longitude coordinate in radians
	double getLongitude(void) const { return _longitude; }

	/// Return the latitude coordinate in radians
	double getLatitude(void) const { return _latitude; }

	///  Comparison function with epsilon
	bool equalsEpsilon(const Geodetic2& other, double epsilon)
	{
		return (fabs(_longitude - other._longitude) <= epsilon) &&
			(fabs(_latitude - other._latitude) <= epsilon);
	}

	///  Comparison function
	bool equals(const Geodetic2& other)
	{
		return _longitude == other._longitude && _latitude == other._latitude;
	}

	///  Comparison operator
	bool operator==(const Geodetic2& right)
	{
		return this->equals(right);
	}

	///  Comparison operator
	bool operator!=(const Geodetic2& right)
	{
		return !this->equals(right);
	}


private:
	double _latitude;
	double _longitude;
};


} //namespace otWorld

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif //Geodetic2_H