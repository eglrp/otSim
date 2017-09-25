/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       WorldTypes.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef WorldTypes_H
#define WorldTypes_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <string>
#include "otMath.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/// Enumeration list for Celestial Body types
enum CelestialBodyTypes
{
	SPECIAL_POINT = 0,
	STAR,
	PLANET,
	MOON,
	DWARF_PLANET,
	ASTEROID,
	COMET,

	NumberOfCelestialBodyTypes
};

const std::string CELESTIAL_BODY_TYPE_STRINGS[NumberOfCelestialBodyTypes] =
{
	"Special_Point",
	"Star",
	"Planet",
	"Moon",
	"Dwarf_Planet",
	"Asteroid",
	"Comet",
};

inline int getCelestialBodyTypeFromString(const std::string& strType)
{
	for (int i = 0; i < (NumberOfCelestialBodyTypes - 1); i++) {
		if (strType == CELESTIAL_BODY_TYPE_STRINGS[i])
			return i;
	}
	return -1;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/// Enumeration list for Atmosphere types
enum AtmosphereTypes {
	NO_ATMOSPHERE = 0,
	US_STANDARD,
	MARS_ATMOSPHERE,

	NumberofAtmosphereTypes,
};

const std::string ATMOSPHERE_TYPE_STRINGS[NumberofAtmosphereTypes] =
{
	"No_Atmosphere",
	"US_Standard",
	"Mars",
};

inline int getAtmosphereTypeFromString(const std::string& strType)
{
	for (int i = 0; i < (NumberofAtmosphereTypes); i++) {
		if (strType == ATMOSPHERE_TYPE_STRINGS[i])
			return i;
	}
	return -1;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/// Enumeration list for Magnetic Model types
enum MagneticModelTypes
{
	NO_MAGNETIC_MODEL = 0,
	WMM2010,
	WMM2015,
	IGRF11,
	IGRF12,
	EMM2010,
	EMM2015,
	EMM2017,

	NumberOfMagneticModelTypes
};

const std::string MAGNETIC_MODEL_TYPE_STRINGS[NumberOfMagneticModelTypes] =
{
	"None",
	"wmm2010",
	"wmm2015",
	"igrf11",
	"igrf12",
	"emm2010",
	"emm2015",
	"emm2017",
};

inline int getMagneticModelTypeFromString(const std::string& strType)
{
	for (int i = 0; i < (NumberOfMagneticModelTypes - 1); i++) {
		if (strType == MAGNETIC_MODEL_TYPE_STRINGS[i])
			return i;
	}
	return -1;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/// Enumeration list for Geoid Model types
enum GeoidModelTypes
{
	NO_GEOID_MODEL = 0,
	EGM84_30,
	EGM84_15,
	EGM96_15,
	EGM96_5,
	EGM2008_5,
	EGM2008_2_5,
	EGM2008_1,

	NumberOfGeoidModelTypes
};

const std::string GEOID_MODEL_TYPE_STRINGS[NumberOfGeoidModelTypes] =
{
	"None",
	"egm84-30",
	"egm84-15",
	"egm96-15",
	"egm96-5",
	"egm2008-5",
	"egm2008-2_5",
	"egm2008-1",
};

inline int getGeoidModelTypeFromString(const std::string& strType)
{
	for (int i = 0; i < (NumberOfGeoidModelTypes - 1); i++) {
		if (strType == GEOID_MODEL_TYPE_STRINGS[i])
			return i;
	}
	return -1;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/// Enumeration list for Gravity Model types
enum GravityModelTypes
{
	SIMPLE_GRAVITY = 0,
	ELLIPSOID_GRAVITY,
	EGM84,
	EGM96,
	EGM2008,

	NumberOfGravityModelTypes
};

const std::string GRAVITY_MODEL_TYPE_STRINGS[NumberOfGravityModelTypes] =
{
	"Simple",
	"Ellipsoid",
	"egm84",
	"egm96",
	"egm2008",
};

inline int getGravityModelTypeFromString(const std::string& strType)
{
	for (int i = 0; i < (NumberOfGravityModelTypes - 1); i++) {
		if (strType == GRAVITY_MODEL_TYPE_STRINGS[i])
			return i;
	}
	return -1;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Physical Properties of a Celestial Body

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

struct CelestialBodyPhysicalProperties
{
	CelestialBodyPhysicalProperties()
	{
		rotationAxis = Vector3(0, 0, 1); //Rotation axis assumed to be +Z
	}

	double GM = 1.0E3;					//Gravitational parameter (m^3/s^2)
	double J2 = 0;						//Second zonal harmonic coefficient (derived dynamic form factor) (Default = 0)
	double rateRotation = 0;			//rad/s (Default = 0)
	double semimajorRadius = 1000.0;	//Equatorial radius (m)
	double semiminorRadius = 1000.0;	//Polar radius (m)  (Default = semimajorRadius, used when inverseFlattening == 0)
	double inverseFlattening = 0;		//Inverse Flattening (0 represents usage of semiminorRadius)
	double geometricAlbedo = 0.1;		//Fraction of solar energy reflected from the body (Default = 0.1)
	double solarConstant = 0;			//Solar radiation per area at 1 AU distance (W/m^2)
	Vector3 rotationAxis;				//unit vector (Default = {0.0, 0.0, 1.0})

};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Orbital elements of a Celestial Body

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//The orbital elements contain two values, with the second being the change in the value per century.
//http://ssd.jpl.nasa.gov/txt/aprx_pos_planets.pdf
struct CelestialBodyOrbitalElements
{
	CelestialBodyOrbitalElements()
	{
		for (int i = 0; i<2; i++) {
			eccentricity[i] = 0.0;
			inclination[i] = 0.0;
			longitudeOfAscendingNode[i] = 0.0;
			longitudeOfPeriapsis[i] = 0.0;
			meanLongitude[i] = 0.0;
		}
		semimajorAxis[0] = 1.0;
		semimajorAxis[1] = 0.0;
	}

	double ephemerisDate = J2000EphemerisDate; //date for when orbital elements were defined

	double semimajorAxis[2]; //a0 (AU), a(dot) (AU/century)
	double eccentricity[2]; //e0 () ,e(dot) (/century)
	double inclination[2]; //I0 (deg), I(dot) (deg/century)
	double meanLongitude[2]; //L0 (deg), L(dot) (deg/century)
	double longitudeOfPeriapsis[2]; //ω(bar)0 (deg), ω(bar-dot) (deg/century)
	double longitudeOfAscendingNode[2]; //Ω0 (deg), Ω(dot) (deg/century)
										//additional terms necessary for computation of the mean Anomaly (M) for some celestial bodies
	double b = 0;
	double c = 0;
	double s = 0;
	double f = 0;
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Local atmospheric state properties

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

struct AtmosphereState
{
	/// Altitude above sea level in m
	double altitudeASL;
	/// Temperature in Kelvin
	double temperature;
	/// Pressure in Pa
	double pressure;
	/// Density in kg/m^3
	double density;
	/// Ratio of specific heats
	double gamma;
	/// Speed of Sound in m/s.
	double speedSound;
	/// Pressure Altitude in m
	double pressureAltitude;
	/// Density Altitude in m
	double densityAltitude;
	/// Absolute Viscosity (dynamic) in Pa-s
	double viscosity;
	/// Kinematic Viscosity in ???
	double kinematicViscosity;
};


} //namespace otWorld

#endif //WorldTypes_H