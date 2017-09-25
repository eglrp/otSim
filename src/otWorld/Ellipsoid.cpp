/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Ellipsoid.cpp
Author:       Patrick Cozzi and Deron Ohlarik & Cory Parks
Date started: 09/2017

------ Copyright (C) 2009,2015  Patrick Cozzi, Deron Ohlarik, Cory Parks ------

C# code distributed under the MIT License.
https://github.com/virtualglobebook/OpenGlobe
http://www.opensource.org/licenses/mit-license.php.
For C++ converted version see LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Ellipsoid class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Ellipsoid.h"

#include <cmath>
#include <algorithm>

#include "GeographicLib\Geodesic.hpp"
#include "GeographicLib\Geocentric.hpp"

#include "Conversions.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

// Hidden (private) implementation details
class Ellipsoid::Impl
{
public:

	Vector3 _radii;
	Vector3 _radiiSquared;
	Vector3 _radiiToTheFourth;
	Vector3 _oneOverRadiiSquared;

	GeographicLib::Geodesic *geodesicObj = nullptr;
	GeographicLib::Geocentric *geocentricObj = nullptr;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ellipsoid::Ellipsoid(void) : mImpl(new Impl())
{
	mImpl->_radii = mImpl->_radiiSquared = mImpl->_radiiToTheFourth = mImpl->_oneOverRadiiSquared = Vector3(1.0, 1.0, 1.0);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ellipsoid::Ellipsoid(double a, double f) : mImpl(new Impl())
{
	if ((a > 0.0))
	{
		Initialize(a, f);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ellipsoid::Ellipsoid(const Vector3& radii) : mImpl(new Impl())
{
	if ((radii[0] > 0.0) || (radii[1] > 0.0) || (radii[2] > 0.0))
	{
		Initialize(radii);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ellipsoid::~Ellipsoid()
{
	if (mImpl->geodesicObj) delete mImpl->geodesicObj;
	if (mImpl->geocentricObj) delete mImpl->geocentricObj;
	delete mImpl;
	mImpl = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::Initialize(const Vector3& radii)
{
	mImpl->_radii = radii;
	mImpl->_radiiSquared = Vector3(
		radii[0] * radii[0],
		radii[1] * radii[1],
		radii[2] * radii[2]);
	mImpl->_radiiToTheFourth = Vector3(
		mImpl->_radiiSquared[0] * mImpl->_radiiSquared[0],
		mImpl->_radiiSquared[1] * mImpl->_radiiSquared[1],
		mImpl->_radiiSquared[2] * mImpl->_radiiSquared[2]);
	mImpl->_oneOverRadiiSquared = Vector3(
		1.0 / (radii[0] * radii[0]),
		1.0 / (radii[1] * radii[1]),
		1.0 / (radii[2] * radii[2]));

	// geodesic class only works on the assumption that radiusX = radiusY (semimajor)
	// if this isn't true, don't create the class object
	if (radii[0] == radii[1])
	{
		double f = (radii[0] - radii[2]) / radii[0];
		mImpl->geodesicObj = new GeographicLib::Geodesic(radii[0], f);
		mImpl->geocentricObj = new GeographicLib::Geocentric(radii[0], f);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::Initialize(double a, double f)
{
	mImpl->_radii[0] = a;
	mImpl->_radii[1] = a;
	mImpl->_radii[2] = mImpl->_radii[0] * (1.0 - f);

	mImpl->_radiiSquared = Vector3(
		mImpl->_radii[0] * mImpl->_radii[0],
		mImpl->_radii[1] * mImpl->_radii[1],
		mImpl->_radii[2] * mImpl->_radii[2]);
	mImpl->_radiiToTheFourth = Vector3(
		mImpl->_radiiSquared[0] * mImpl->_radiiSquared[0],
		mImpl->_radiiSquared[1] * mImpl->_radiiSquared[1],
		mImpl->_radiiSquared[2] * mImpl->_radiiSquared[2]);
	mImpl->_oneOverRadiiSquared = Vector3(
		1.0 / (mImpl->_radii[0] * mImpl->_radii[0]),
		1.0 / (mImpl->_radii[1] * mImpl->_radii[1]),
		1.0 / (mImpl->_radii[2] * mImpl->_radii[2]));

	mImpl->geodesicObj = new GeographicLib::Geodesic(a, f);
	mImpl->geocentricObj = new GeographicLib::Geocentric(a, f);

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define CALL_MEMBER_FUNCTION(object,ptrToMember) ((object).*(ptrToMember))
typedef void (GeographicLib::Geodesic::*InverseGeodesicFunction)(double, double, double, double, double&, double&, double&) const;
typedef void (GeographicLib::Geodesic::*DirectGeodesicFunction)(double, double, double&, double&, double, double, double&) const;

void getDistanceAndBearingArbitrary(double lat1, double lon1, double lat2, double lon2,
	double& distance, double& initialBearing, double& finalBearing,
	GeographicLib::Geodesic& geodesicObject,
	InverseGeodesicFunction function)
{
	CALL_MEMBER_FUNCTION(geodesicObject, function)(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (initialBearing < 0) initialBearing += 360.0;
	if (finalBearing < 0) finalBearing += 360.0;

	initialBearing = DEGtoRAD(initialBearing);
	finalBearing = DEGtoRAD(finalBearing);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void getDestinationPointArbitrary(double lat1, double lon1, double& lat2, double& lon2,
	double distance, double initialBearing, double& finalBearing,
	GeographicLib::Geodesic& geodesicObject,
	DirectGeodesicFunction function)
{
	CALL_MEMBER_FUNCTION(geodesicObject, function)(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (finalBearing < 0) finalBearing += 360.0;

	finalBearing = DEGtoRAD(finalBearing);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void PreprocessDistanceData(const Geodetic3& origin, const Geodetic3& destination,
	double& lat1, double& lon1, double& lat2, double& lon2,
	double& distance, double& initialBearing, double& finalBearing)
{
	distance = 0.0;
	initialBearing = 0.0;
	finalBearing = 0.0;
	lat1 = RADtoDEG(origin.getLatitude());
	lon1 = RADtoDEG(origin.getLongitude());
	lat2 = RADtoDEG(destination.getLatitude());
	lon2 = RADtoDEG(destination.getLongitude());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getDestinationPoint(const Geodetic3& origin, Geodetic3& destination,
	double distance, double initialBearing, double& finalBearing) const
{
	DirectGeodesicFunction destinationFunction = &GeographicLib::Geodesic::VincentyDirect;

	double lat1, lon1, lat2, lon2;

	PreprocessDistanceData(origin, destination, lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (mImpl->geodesicObj) {
		getDestinationPointArbitrary(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing, *mImpl->geodesicObj, destinationFunction);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getEstimatedDestinationPoint(const Geodetic3& origin, Geodetic3& destination,
	double distance, double initialBearing, double& finalBearing) const
{
	DirectGeodesicFunction destinationFunction = &GeographicLib::Geodesic::HaversineDirect;

	double lat1, lon1, lat2, lon2;

	PreprocessDistanceData(origin, destination, lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (mImpl->geodesicObj) {
		getDestinationPointArbitrary(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing, *mImpl->geodesicObj, destinationFunction);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getExactDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
	double& distance, double& initialBearing, double& finalBearing) const
{
	InverseGeodesicFunction distanceFunction = &GeographicLib::Geodesic::KarneyInverse;

	double lat1, lon1, lat2, lon2;

	PreprocessDistanceData(origin, destination, lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (mImpl->geodesicObj) {
		getDistanceAndBearingArbitrary(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing, *mImpl->geodesicObj, distanceFunction);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
	double& distance, double& initialBearing, double& finalBearing) const
{
	InverseGeodesicFunction distanceFunction = &GeographicLib::Geodesic::VincentyInverse;

	double lat1, lon1, lat2, lon2;

	PreprocessDistanceData(origin, destination, lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (mImpl->geodesicObj) {
		getDistanceAndBearingArbitrary(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing, *mImpl->geodesicObj, distanceFunction);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getEstimatedDistanceAndBearing(const Geodetic3& origin, const Geodetic3& destination,
	double& distance, double& initialBearing, double& finalBearing) const
{
	InverseGeodesicFunction distanceFunction = &GeographicLib::Geodesic::HaversineInverse;

	double lat1, lon1, lat2, lon2;

	PreprocessDistanceData(origin, destination, lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing);

	if (mImpl->geodesicObj) {
		getDistanceAndBearingArbitrary(lat1, lon1, lat2, lon2, distance, initialBearing, finalBearing, *mImpl->geodesicObj, distanceFunction);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::getGeodeticSurfaceNormal(const Vector3& positionOnEllipsoid) const
{
	return (positionOnEllipsoid.multiplyComponents(mImpl->_oneOverRadiiSquared)).unitVector();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::getGeodeticSurfaceNormal(const Geodetic3& geodetic) const
{
	double cosLatitude = cos(geodetic.getLatitude());

	return Vector3(
		cosLatitude * cos(geodetic.getLongitude()),
		cosLatitude * sin(geodetic.getLongitude()),
		sin(geodetic.getLatitude()));
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double Ellipsoid::getMinimumRadius(void) const
{
	return std::min(mImpl->_radii[0], std::min(mImpl->_radii[1], mImpl->_radii[2]));
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double Ellipsoid::getMaximumRadius(void) const
{
	return std::max(mImpl->_radii[0], std::max(mImpl->_radii[1], mImpl->_radii[2]));
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void Ellipsoid::getIntersections(const Vector3& origin, Vector3 direction, double* roots) const
{
	//double *root = new double[2];

	direction.normalize();

	// By laborious algebraic manipulation....
	double a = direction[0] * direction[0] * mImpl->_oneOverRadiiSquared[0] +
		direction[1] * direction[1] * mImpl->_oneOverRadiiSquared[1] +
		direction[2] * direction[2] * mImpl->_oneOverRadiiSquared[2];
	double b = 2.0 *
		(origin[0] * direction[0] * mImpl->_oneOverRadiiSquared[0] +
			origin[1] * direction[1] * mImpl->_oneOverRadiiSquared[1] +
			origin[2] * direction[2] * mImpl->_oneOverRadiiSquared[2]);
	double c = origin[0] * origin[0] * mImpl->_oneOverRadiiSquared[0] +
		origin[1] * origin[1] * mImpl->_oneOverRadiiSquared[1] +
		origin[2] * origin[2] * mImpl->_oneOverRadiiSquared[2] - 1.0;

	// Solve the quadratic equation: ax^2 + bx + c = 0.
	// Algorithm is from Wikipedia's "Quadratic equation" topic, and Wikipedia credits
	// Numerical Recipes in C, section 5.6: "Quadratic and Cubic Equations"
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0)
	{
		// no intersections
		roots[0] = 0;
		roots[1] = 0;
		return;
	}
	else if (discriminant == 0.0)
	{
		// one intersection at a tangent point
		double intersection = -0.5 * b / a;
		roots[0] = intersection;
		roots[1] = intersection;
		return;
	}

	double t = -0.5 * (b + (b > 0.0 ? 1.0 : -1.0) * sqrt(discriminant));
	double root1 = t / a;
	double root2 = c / t;

	// Two intersections - return the smallest first.
	if (root1 < root2)
	{
		roots[0] = root1;
		roots[1] = root2;
		return;
	}
	else
	{
		roots[0] = root2;
		roots[1] = root1;
		return;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::toECEF(const Geodetic3& geodetic) const
{
	Vector3 n = getGeodeticSurfaceNormal(geodetic);
	Vector3 k = mImpl->_radiiSquared.multiplyComponents(n);
	double gamma = sqrt(
		(k[0] * n[0]) +
		(k[1] * n[1]) +
		(k[2] * n[2]));

	Vector3 rSurface = k / gamma;
	return rSurface + (n * geodetic.getHeight());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::toECEF(const Geodetic2& geodetic) const
{
	return (toECEF(Geodetic3(geodetic.getLatitude(), geodetic.getLongitude(), 0)));
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Geodetic3 Ellipsoid::toGeodetic3(const Vector3& position) const
{
	double latitude = 0.0;
	double longitude = 0.0;
	double height = 0.0;

	if (!position.isNull()) {
		Vector3 positionOnEllipsoid = ScaleToGeodeticSurface(position);
		Vector3 n = getGeodeticSurfaceNormal(positionOnEllipsoid);
		longitude = atan2(n[1], n[0]);
		//latitude = asin(n[2] / n.magnitude());  //remove - n is already a unit vector
		latitude = asin(n[2]);

		Vector3 h = position - positionOnEllipsoid;
		height = (h.dot(position) >= 0.0 ? 1.0 : -1.0) * h.magnitude();
	}

	return Geodetic3(latitude, longitude, height);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Geodetic2 Ellipsoid::toGeodetic2(const Vector3& position) const
{
	Geodetic3 geod3 = toGeodetic3(position);

	return Geodetic2(geod3.getLatitude(), geod3.getLongitude());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::ScaleToGeodeticSurface(const Vector3& position) const
{
	double beta = 1.0 / sqrt(
		(position[0] * position[0]) * mImpl->_oneOverRadiiSquared[0] +
		(position[1] * position[1]) * mImpl->_oneOverRadiiSquared[1] +
		(position[2] * position[2]) * mImpl->_oneOverRadiiSquared[2]);
	double n = Vector3(
		beta * position[0] * mImpl->_oneOverRadiiSquared[0],
		beta * position[1] * mImpl->_oneOverRadiiSquared[1],
		beta * position[2] * mImpl->_oneOverRadiiSquared[2]).magnitude();

	double alpha = (1.0 - beta) * (position.magnitude() / n);

	double x2 = position[0] * position[0];
	double y2 = position[1] * position[1];
	double z2 = position[2] * position[2];

	double da = 0.0;
	double db = 0.0;
	double dc = 0.0;

	double s = 0.0;
	double dSdA = 1.0;

	do
	{
		alpha -= (s / dSdA);

		da = 1.0 + (alpha * mImpl->_oneOverRadiiSquared[0]);
		db = 1.0 + (alpha * mImpl->_oneOverRadiiSquared[1]);
		dc = 1.0 + (alpha * mImpl->_oneOverRadiiSquared[2]);

		double da2 = da * da;
		double db2 = db * db;
		double dc2 = dc * dc;

		double da3 = da * da2;
		double db3 = db * db2;
		double dc3 = dc * dc2;

		s = x2 / (mImpl->_radiiSquared[0] * da2) +
			y2 / (mImpl->_radiiSquared[1] * db2) +
			z2 / (mImpl->_radiiSquared[2] * dc2) - 1.0;

		dSdA = -2.0 *
			(x2 / (mImpl->_radiiToTheFourth[0] * da3) +
				y2 / (mImpl->_radiiToTheFourth[1] * db3) +
				z2 / (mImpl->_radiiToTheFourth[2] * dc3));
	} while (abs(s) > 1e-10);

	return Vector3(
		position[0] / da,
		position[1] / db,
		position[2] / dc);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::ScaleToGeocentricSurface(const Vector3& position) const
{
	double beta = 1.0 / sqrt(
		(position[0] * position[0]) * mImpl->_oneOverRadiiSquared[0] +
		(position[1] * position[1]) * mImpl->_oneOverRadiiSquared[1] +
		(position[2] * position[2]) * mImpl->_oneOverRadiiSquared[2]);

	return (position * beta);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::toSphericalECEF(const Vector3& position, double radius) const
{
	return toSphericalECEF(toGeodetic3(position), radius);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 Ellipsoid::toSphericalECEF(const Geodetic3& geodetic, double radius) const
{
	if (radius <= 0) radius = mImpl->_radii[0];

	Vector3 n = getGeodeticSurfaceNormal(geodetic);
	return n * (radius + geodetic.getHeight());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Geodetic3 Ellipsoid::sphericalECEFToGeodetic(const Vector3& position, double radius) const
{
	if (radius <= 0) radius = mImpl->_radii[0];

	double latitude = 0.0;
	double longitude = 0.0;
	double height = 0.0;

	if (!position.isNull()) {
		longitude = atan2(position[1], position[0]);
		double s = sqrt(position[0] * position[0] + position[1] * position[1]);
		latitude = atan2(position[2], s);

		height = s*cos(latitude) + position[2]*sin(latitude) - radius;
	}

	return Geodetic3(latitude, longitude, height);
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Vector3& Ellipsoid::getRadii(void) const
{
	return mImpl->_radii;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Vector3& Ellipsoid::getRadiiSquared(void) const
{
	return mImpl->_radiiSquared;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Vector3& Ellipsoid::getOneOverRadiiSquared(void) const
{
	return mImpl->_oneOverRadiiSquared;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

GeographicLib::Geocentric* Ellipsoid::getGeocentricObj() const
{
	return mImpl->geocentricObj;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otWorld

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%