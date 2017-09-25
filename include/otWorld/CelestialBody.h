/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       CelestialBody.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef CelestialBody_H
#define CelestialBody_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "ICelestialBody.h"

#include <algorithm>

#include "Geodetic2.h"
#include "Geodetic3.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Celestial Body Class

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class CelestialBody : public ICelestialBody
{
	friend class WorldManager;
public:
	/// Constructor
	CelestialBody(const GUID& guid);

	/// Destructor
	virtual ~CelestialBody();


	/////////////////////////
	//  Virtual Functions  //
	/////////////////////////

	/// Returns the gravitational acceleration vector in the ECEF coordinate system
	virtual Vector3 getGravitationalAccelerationECEF(const Vector3& posECEF) const;

	/// Returns the gravity value at the given radius from the center of the celestial body - m/s^2
	virtual double getGravity(double radius) const;

	/// Returns the gravity value at sea level for the celestial body (uses semimajor radius) - m/s^2
	virtual double getSLGravity(void) const;


	/////////////////////
	//  Set Functions  //
	/////////////////////

	/// Sets the type of the celestial body (STAR, PLANET, MOON, etc)
	void setCelestialBodyType(CelestialBodyTypes celestialBodyType);

	/// Sets the central body around which the celestial body orbits
	void setCentralBody(ICelestialBody* parentCentralBody);

	/// Sets the central body around which the celestial body orbits using the GUID
	void setCentralBody(const GUID& parentCentralBodyGUID);

	/// Sets the physical properties for the celestial body
	void setPhysicalProperties(const CelestialBodyPhysicalProperties& physicalProperties);

	/// Sets the orbital elements for the celestial body
	void setOrbitalElements(const CelestialBodyOrbitalElements& orbitalElements);

	/// Sets the atmosphere for the celestial body (Default = NoAtmosphere, if not set)
	/// Deletes the existing atmosphere and assigns the new one
	//void setAtmosphere(Weather::IAtmosphere* atmosphere);

	/// Sets the magnetic model for the celestial body (Default = NO_MAGNETIC_MODEL, if not set)
	void setMagneticModel(MagneticModelTypes magneticModelType);

	/// Sets the gravity model for the celestial body (Default = ELLIPSOID, if not set)
	void setGravityModel(GravityModelTypes gravityModelType);

	/// Sets the angle of rotation of the planet in its cycle
	void setAngleRotation(double angle);

	/// Changes the rotational rate for the planet with a scalar (Default = 1.0)
	void setRateRotationScalar(double scalar);

	/// Set a 1-D table specifying the fraction of surface gravity as you travel toward the center of the celestial body
	/// Index is the ratio current radii to the semiminor (smaller) radius of the celestial body
	/// Value is the fraction of the surface gravity at that semiminor radius
	void setInternalGravityFactorTable(const dTable& table);


	/////////////////////
	//  Get Functions  //
	/////////////////////

	/// Returns the type of the celestial body (STAR, PLANET, MOON, etc)
	CelestialBodyTypes getCelestialBodyType(void) const;

	/// Returns the unique identifier (GUID) of the celestial body
	const GUID& getGUID(void) const;

	/// Returns the central body around which the celestial body orbits
	ICelestialBody* getCentralBody(void) const;

	/// Gets the GUID of the central body around which the celestial body orbits
	const GUID& getCentralBodyGUID(void) const;

	/// Returns the atmosphere for the celestial body (Default = NoAtmosphere, if not set)
	//Weather::IAtmosphere* getAtmosphere(void) const;

	/// Returns the Ellipsoidal shape of the celestial body
	Ellipsoid* getShape(void);

	/// Returns the angle of rotation of the planet in its cycle
	double getAngleRotation(void) const;

	/// Returns the Semimajor (equatorial) axis radius in meters
	double getSemimajorRadius(void) const { return std::max(physicalProperties.semimajorRadius, 0.001); }

	/// Returns the Semiminor axis radius in meters
	double getSemiminorRadius(void) const { return std::max(physicalProperties.semiminorRadius, 0.001); }

	/// Returns the rate of rotation (spin) of the celestial body around its axis - rad/s
	double getRateRotation(void) const { return physicalProperties.rateRotation; }

	/// Returns the structure containing the physical properties of the celestial body
	const CelestialBodyPhysicalProperties& getPhysicalProperties(void) const { return physicalProperties; }

	/// Returns the structure containing the orbital elements for the celestial body
	const CelestialBodyOrbitalElements& getOrbitalElements(void) const { return orbitalElements; }

	/// Returns the mass the celestial body in kg
	double getMass(void) const;

	/// Returns the volume of the celestial body in m^3
	double getVolume(void) const;

	/// Returns the density of the celestial body in kg/m^3
	double getDensity(void) const;

	/// Returns the rotational rate vector of the planet in the ECEF coordinate system
	const Vector3& getRotationWorld(void) const;

	/// Returns the ECI to ECEF Transformation matrix for the celestial body
	const Matrix33& getECI2ECEFTransform(void) const;

	/// Returns the ECEF to ECI Transformation matrix for the celestial body
	const Matrix33& getECEF2ECITransform(void) const;

	/// Returns the magnetic field measurement in nanoTeslas (nT) within an NED vector, along with the
	/// declination and inclination in degrees, given the geodetic location (Lat, Lon, HAE) and time in years (decimal).
	/// If no magnetic model is loaded, all values will return zero.
	void getMagneticField(const Geodetic3& geodetic, double time_years, Vector3& NED_nT, double& decl_deg, double& incl_deg) const;

protected:

	//Must define these in derived class
	CelestialBodyPhysicalProperties physicalProperties;
	CelestialBodyOrbitalElements orbitalElements;//Orbital parameters describing a unique orbit in the solar system


private:
	// Make this object be noncopyable because it contains a pointer
	CelestialBody(const CelestialBody&);
	const CelestialBody &operator =(const CelestialBody &);

	/// Initialize the derived celestial body properties after setting the physical properties.
	/// Called automatically before update() if not already called.
	void Initialize();

	/// Run the simulation of the celestial body
	/// Called by PhysicsManager::update()
	void update();

	void computeOrbitalParameters(double julianDate);

	/// Performs the rotation of the planet around its own spin axis and translation around its parent body in its orbit.
	void Propagate();

	void UpdateRotationMatrix();

	//pointer to implementation class
	class Impl;
	Impl* mImpl;
};


} //namespace otWorld

#endif //CelestialBody_H