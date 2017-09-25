/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       ICelestialBody.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef ICelestialBody_H
#define ICelestialBody_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "WorldConstants.h"
#include "WorldTypes.h"
//#include "IAtmosphere.h"
#include "GUID.h"
#include "otMath.h"
#include "Table.h"
#include "Ellipsoid.h"

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

/** Interface class for Celestial Body objects

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class WORLD_API ICelestialBody
{
public:

	virtual ~ICelestialBody() {}


	/////////////////////
	//  Set Functions  //
	/////////////////////

	/// Sets the type of the celestial body (STAR, PLANET, MOON, etc)
	virtual void setCelestialBodyType(CelestialBodyTypes celestialBodyType) = 0;

	/// Sets the central body around which the celestial body orbits
	virtual void setCentralBody(ICelestialBody* parentCentralBody) = 0;

	/// Sets the central body around which the celestial body orbits using the GUID
	virtual void setCentralBody(const GUID& parentCentralBodyGUID) = 0;

	/// Sets the physical properties for the celestial body
	virtual void setPhysicalProperties(const CelestialBodyPhysicalProperties& physicalProperties) = 0;

	/// Sets the orbital elements for the celestial body
	virtual void setOrbitalElements(const CelestialBodyOrbitalElements& orbitalElements) = 0;

	/// Sets the atmosphere for the celestial body (Default = NoAtmosphere, if not set)
	/// Deletes the existing atmosphere and assigns the new one
	//virtual void setAtmosphere(Weather::IAtmosphere* atmosphere) = 0;

	/// Sets the magnetic model for the celestial body (Default = NO_MAGNETIC_MODEL, if not set)
	virtual void setMagneticModel(MagneticModelTypes magneticModelType) = 0;

	/// Sets the gravity model for the celestial body (Default = ELLIPSOID, if not set)
	virtual void setGravityModel(GravityModelTypes gravityModelType) = 0;

	/// Set a 1-D table specifying the fraction of surface gravity as you travel toward the center of the celestial body
	/// Index is the ratio current radii to the semiminor (smaller) radius of the celestial body
	/// Value is the fraction of the surface gravity at that semiminor radius
	virtual void setInternalGravityFactorTable(const dTable& table) = 0;


	/////////////////////
	//  Get Functions  //
	/////////////////////

	/// Retrieves the type of the celestial body (STAR, PLANET, MOON, etc)
	virtual CelestialBodyTypes getCelestialBodyType(void) const = 0;

	/// Returns the unique identifier (GUID) of the celestial body
	virtual const GUID& getGUID(void) const = 0;

	/// Returns the central body around which the celestial body orbits
	virtual ICelestialBody* getCentralBody(void) const = 0;

	/// Gets the GUID of the central body around which the celestial body orbits
	virtual const GUID& getCentralBodyGUID(void) const = 0;

	/// Returns the atmosphere for the celestial body (Default = NoAtmosphere, if not set)
	//virtual Weather::IAtmosphere* getAtmosphere(void) const = 0;

	/// Returns the Ellipsoidal shape of the celestial body
	virtual Ellipsoid* getShape(void) = 0;

	/// Returns the gravitational acceleration vector in the ECEF coordinate system
	virtual Vector3 getGravitationalAccelerationECEF(const Vector3& posECEF) const = 0;

	/// Returns the gravity value at the given radius from the center of the celestial body - m/s^2
	virtual double getGravity(double radius) const = 0;

	/// Returns the angle of rotation of the planet in its cycle
	virtual double getAngleRotation(void) const = 0;

	/// Returns the Semimajor (equatorial) axis radius in meters
	virtual double getSemimajorRadius(void) const = 0;

	/// Returns the Semiminor axis radius in meters
	virtual double getSemiminorRadius(void) const = 0;

	/// Returns the rate of rotation (spin) of the celestial body around its axis - rad/s
	virtual double getRateRotation(void) const = 0;

	/// Returns the structure containing the physical properties of the celestial body
	virtual const CelestialBodyPhysicalProperties& getPhysicalProperties(void) const = 0;

	/// Returns the structure containing the orbital elements for the celestial body
	virtual const CelestialBodyOrbitalElements& getOrbitalElements(void) const = 0;

	/// Returns the mass the celestial body in kg
	virtual double getMass(void) const = 0;

	/// Returns the volume of the celestial body in m^3
	virtual double getVolume(void) const = 0;

	/// Returns the density of the celestial body in kg/m^3
	virtual double getDensity(void) const = 0;

	/// Returns the rotational rate vector of the celestial body in the ECEF coordinate system
	virtual const Vector3& getRotationWorld(void) const = 0;

	/// Returns the ECI to ECEF Transformation matrix for the celestial body
	virtual const Matrix33 &getECI2ECEFTransform(void) const = 0;

	/// Returns the ECEF to ECI Transformation matrix for the celestial body
	virtual const Matrix33 &getECEF2ECITransform(void) const = 0;

	/// Returns the magnetic field measurement in nanoTeslas (nT) within an NED vector, along with the
	/// declination and inclination in degrees, given the geodetic location (Lat, Lon, HAE) and time in years (decimal).
	/// If no magnetic model is loaded, all values will return zero.
	virtual void getMagneticField(const Geodetic3& geodetic, double time_years, Vector3& NED_nT, double& decl_deg, double& incl_deg) const = 0;


};

} //namespace otWorld

#endif //ICelestialBody_H