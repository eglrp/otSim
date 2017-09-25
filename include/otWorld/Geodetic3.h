/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Geodetic3.h
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

#ifndef Geodetic3_H
#define Geodetic3_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

}

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

/** Models the Geodetic container (latitude, longitude, height).

@author Patrick Cozzi and Deron Ohlarik and Kevin Ring & adapted by Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class WORLD_API Geodetic3
{
public:
	/// Constructor
	Geodetic3(void)
	{
		_latitude = 0.0;
		_longitude = 0.0;
		_height = 0.0;
	}

	/// Constructor
	Geodetic3(double latitude, double longitude, double height)
	{
		_latitude = latitude;
		_longitude = longitude;
		_height = height;
	}

	/// Constructor
	Geodetic3(double latitude, double longitude)
	{
		_latitude = latitude;
		_longitude = longitude;
		_height = 0.0;
	}

	/// Copy Constructor
	Geodetic3(const Geodetic3& geodetic)
	{
		_latitude = geodetic.getLatitude();
		_longitude = geodetic.getLongitude();
		_height = geodetic.getHeight();
	}

	/// Destructor
	~Geodetic3() {}

	/// Initialization (zero)
	void Init(void)
	{
		_longitude = 0.0;
		_latitude = 0.0;
		_height = 0.0;
	}

	/// Assignment operator
	Geodetic3& operator=(const Geodetic3& geodetic)
	{
		_latitude = geodetic.getLatitude();
		_longitude = geodetic.getLongitude();
		_height = geodetic.getHeight();
		return *this;
	}

	/// Return the longitude coordinate in radians
	double getLongitude(void) const { return _longitude; }

	/// Return the latitude coordinate in radians
	double getLatitude(void) const { return _latitude; }

	/// Return the height (altitude) value in meters
	double getHeight(void) const { return _height; }

	///  Comparison function
	bool Equals(const Geodetic3& other)
	{
		return _longitude == other._longitude && _latitude == other._latitude && _height == other._height;
	}

	///  Comparison operator
	bool operator==(const Geodetic3& right)
	{
		return this->Equals(right);
	}

	///  Comparison operator
	bool operator!=(const Geodetic3& right)
	{
		return !this->Equals(right);
	}


private:
	double _latitude;
	double _longitude;
	double _height;
};


} //namespace otWorld

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif //Geodetic3_H