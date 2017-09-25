/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Ellipsoid.h
Author:       Patrick Cozzi and Deron Ohlarik & Cory Parks
Date started: 09/2017

------ Copyright (C) 2009,2015  Patrick Cozzi, Deron Ohlarik, Cory Parks ------

C# code distributed under the MIT License.
https://github.com/virtualglobebook/OpenGlobe
http://www.opensource.org/licenses/mit-license.php.
For C++ converted version see LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Ellipsoid_H
#define Ellipsoid_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Geodetic3.h"
#include "Geodetic2.h"
#include "otMath.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace GeographicLib {
class Geocentric;
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

/** Models the Ellipsoid class.

@author Patrick Cozzi and Deron Ohlarik & Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class WORLD_API Ellipsoid
{
public:
	/// Constructor
	Ellipsoid(void);
	/// Constructor given semi-major axis radius (a) and flattening (f)
	Ellipsoid(double a, double f);
	/// Constructor given radii of ellipsoid
	Ellipsoid(const Vector3& radii);

	/// Destructor
	~Ellipsoid();

	/// Get the estimated destination geodetic coordinates and final bearing along the surface of the ellipsoid.
	/// Fairly intensive iterative function that is usually quite accurate..
	/// Only use this function when a good destination and/or final bearing value is necessary.
	/// Use getEstimatedDestinationPoint function instead to get a quicker estimate.
	void getDestinationPoint(const Geodetic3& origin, Geodetic3& destination,
		double distance, double initialBearing, double& finalBearing) const;

	/// Get the estimated destination geodetic coordinates and final bearing along the surface of the sphere.
	/// Because this function assumes a spherical shape, it is very quick to solve, but the error can be up to
	/// about 0.5%.
	/// Only use this function when a rough estimate destination and/or final bearing value is necessary.
	/// Use getDestinationPoint function instead to get a better estimate.
	void getEstimatedDestinationPoint(const Geodetic3& origin, Geodetic3& destination,
		double distance, double initialBearing, double& finalBearing) const;

	/// Get the distance and bearing between two geodetic coordinates along the surface of the ellipsoid.
	/// Fairly intensive iterative function that is usually quite accurate (10E-6 m).
	/// May have convergence problems near antipodal points, in which case, the getExactDistanceAndBearing
	/// function should be used instead.
	/// Only use this function when a good distance and/or bearing value is necessary.
	/// Use getEstimatedDistanceAndBearing function instead to get a quicker estimate.
	void getDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
		double& distance, double& initialBearing, double& finalBearing) const;

	/// Get the exact distance and bearing between two geodetic coordinates along the surface of the ellipsoid.
	/// Very intensive function that is accurate to a few nanometers (10E-8).
	/// Only use this function when an exact distance and/or bearing is necessary.
	/// Use getDistanceAndBearing or getEstimatedDistanceAndBearing function instead to get a quicker estimate.
	void getExactDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
		double& distance, double& initialBearing, double& finalBearing) const;

	/// Get the estimated distance and bearing between two geodetic coordinates along the surface of the sphere.
	/// Because this function assumes a spherical shape, it is very quick to solve, but the error can be up to
	/// about 0.5%.
	/// Only use this function when a rough estimate distance and/or bearing value is necessary.
	/// Use getDistanceAndBearing function instead to get a better estimate.
	void getEstimatedDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
		double& distance, double& initialBearing, double& finalBearing) const;

	/// Return the normal vector in ECEF coordinates for a given position on the Ellipsoid that travels through the center of the ellipsoid
	/// This is effectively the unit vector of the position
	static Vector3 getCentricSurfaceNormal(const Vector3& positionOnEllipsoid)
	{
		return positionOnEllipsoid.unitVector();
	}

	/// Return the surface normal vector in ECEF coordinates for a given position on the ellipsoid also given in ECEF coordinates.
	/// The given position MUST be at exactly zero altitude above the local horizontal frame.
	/// Use the other function that takes the geodetic coordinates (lat,lon) if this isn't true.
	Vector3 getGeodeticSurfaceNormal(const Vector3& positionOnEllipsoid) const;

	/// Return the surface normal vector in ECEF coordinates for a given position on the ellipsoid given in geodetic coordinates (lat,lon)
	Vector3 getGeodeticSurfaceNormal(const Geodetic3& geodetic) const;

	/// Return the radii Vector3 for each axis of the ellipsoid
	const Vector3& getRadii(void) const;

	/// Return the radii squared Vector3 for each axis of the ellipsoid
	const Vector3& getRadiiSquared(void) const;

	/// Return the Vector3 for one over the radii squared for each axis of the ellipsoid
	const Vector3& getOneOverRadiiSquared(void) const;

	/// Return the value of the minimum radius between each axis of the ellipsoid
	double getMinimumRadius(void) const;

	/// Return the value of the maxium radius between each axis of the ellipsoid
	double getMaximumRadius(void) const;

	/// Returns the roots (one or two) of the intersection distance to the surface of the ellipsoid,
	/// given the location (relative to the ellipsoid) and the direction vector.
	/// Pass a pointer to a two sized double array for the roots to be returned.
	/// Values of zero will indicate no solutions found.  If two roots are found, the smallest is reported first.
	/// These returned values can be used to find the exact intersection point by multiplying the
	/// distance by the normalized direction vector and adding it to the current location.
	/// Ex: intersectionPoint = origin + direction.normalize() * intersectionDistance;
	void getIntersections(const Vector3& origin, Vector3 direction, double* roots) const;

	/// Convert the given geodetic coordinates (lat,lon,alt) to ECEF coordinates (X,Y,Z) and return the Vector3
	Vector3 toECEF(const Geodetic3& geodetic) const;

	/// Convert the given geodetic coordinates (lat,lon) to ECEF coordinates (X,Y,Z) and return the Vector3 (altitude is assumed 0)
	Vector3 toECEF(const Geodetic2& geodetic) const;

	/// Convert the given ECEF coordinates (X,Y,Z) to geodetic coordinates (lat,lon,alt) the Geodetic3 structure
	Geodetic3 toGeodetic3(const Vector3& position) const;

	/// Convert the given ECEF coordinates (X,Y,Z) to geodetic coordinates (lat,lon) the Geodetic2 structure
	Geodetic2 toGeodetic2(const Vector3& position) const;

	/// Scale the given position Vector3 down to the geodetic surface to find the position on the surface of the ellipsoid
	Vector3 ScaleToGeodeticSurface(const Vector3& position) const;

	/// ???
	Vector3 ScaleToGeocentricSurface(const Vector3& position) const;

	/// Convert the given ECEF coordinates (X,Y,Z) to spherical ECEF coordinates (X,Y,Z) with the
	/// given sphere radius.  If radius is not given, the ellipsoid's X axis radius will be used.
	Vector3 toSphericalECEF(const Vector3& position, double radius = 0) const;

	/// Convert the geodetic coordinates (lat,lon,alt) to spherical ECEF coordinates (X,Y,Z) with the
	/// given sphere radius.  If radius is not given, the ellipsoid's X axis radius will be used.
	Vector3 toSphericalECEF(const Geodetic3& geodetic, double radius = 0) const;

	Geodetic3 sphericalECEFToGeodetic(const Vector3& position, double radius) const;

	///Returns the GeographicLib Geocentric Object Pointer
	GeographicLib::Geocentric* getGeocentricObj() const;

private:
	/// Make this object be noncopyable because it holds a pointer
	Ellipsoid(const Ellipsoid& ellipsoid);
	const Ellipsoid &operator =(const Ellipsoid &);

	void Initialize(const Vector3& radii);
	void Initialize(double a, double f);

	class Impl;
	Impl* mImpl;
};

} //namespace otWorld

#endif //Ellipsoid_H