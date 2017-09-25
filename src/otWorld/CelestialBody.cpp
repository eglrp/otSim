/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       CelestialBody.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Celestial Body class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "CelestialBody.h"

#include "GeographicLib\MagneticModel.hpp"
#include "GeographicLib\GravityModel.hpp"
#include "GeographicLib\Geocentric.hpp"

#include "ITime.h"
#include "Conversions.h"
#include "Paths.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

class CelestialBody::Impl
{
public:
	Impl(const GUID& _guid) : guid(_guid), centralBodyGUID(_guid)
	{
		//initially create an empty atmosphere and assign it to the celestial body
		//atmosphere = Weather::AtmosphereFactory::CreateAtmosphere(Weather::NO_ATMOSPHERE);

		internalGravityFactorTable = new dTable(2);

		//							 Radius Frac    Gravity Frac
		//							 -----------    -----------
		*internalGravityFactorTable << 0.0000000 << 0.0000000
									<< 1.0000000 << 1.0000000;
	}

	~Impl() {
		//clean up atmosphere if it still exists
		//if (atmosphere)
		//	Weather::AtmosphereFactory::DestroyAtmosphere(atmosphere);

		if (magneticModel) delete magneticModel;

		if (shape) delete shape;
		shape = nullptr;

		if (internalGravityFactorTable) delete internalGravityFactorTable;
		internalGravityFactorTable = nullptr;
	}

	bool loadMagneticModel(MagneticModelTypes model);
	bool loadGravityModel(GravityModelTypes model);

	ICelestialBody* centralBody = nullptr;  //parent celestial body
	Ellipsoid* shape = nullptr;

	MagneticModelTypes magneticModelType = NO_MAGNETIC_MODEL;
	GravityModelTypes gravityModelType = ELLIPSOID_GRAVITY;

	//Weather::Atmosphere* atmosphere = nullptr;
	GeographicLib::MagneticModel* magneticModel = nullptr;
	GeographicLib::GravityModel* gravityModel = nullptr;
	GeographicLib::NormalGravity* normalGravityModel = nullptr;

	CelestialBodyTypes celestialBodyType = SPECIAL_POINT;

	GUID guid = otCore::GUID_NULL;
	GUID centralBodyGUID = otCore::GUID_NULL;

	bool initialized = false;

	double semimajorAxis; //a (AU)
	double eccentricity; //e
	double inclination; //I (deg)
	double meanLongitude; //L (deg)
	double longitudeOfPeriapsis; //ω(bar) (deg)
	double longitudeOfAscendingNode; //Ω (deg)

	double argumentOfPeriapsis; //ω (deg)
	double meanAnomaly; //M (deg)
	double eccentricAnomaly; //E (deg)

	Vector3 orbitalPlanePosition;
	Vector3 eclipticPlanePosition;

	double mass; //kg
	double volume; //m^3
	double density; //kg/m^3
	double angleRotation = 0; //rad
	double rotationRateScalar;
	Vector3 worldRotation; //celestial body rotational vector

	Matrix33 ECI2ECEFTransform;
	Matrix33 ECEF2ECITransform;

	dTable* internalGravityFactorTable = nullptr; //Fraction of surface gravity as you travel toward the center of the body

};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool CelestialBody::Impl::loadMagneticModel(MagneticModelTypes model)
{
	bool modelLoaded = false;
	//if (model >= 0 && model < NumberOfMagneticModelTypes)
	//{
	//	std::string magModelStr = MAGNETIC_MODEL_TYPE_STRINGS[model];
	//
	//	std::string magFolder = otCore::Paths::getDataDir() + "\\magnetic";
	//	std::string metadatalFile = magFolder + "\\" + magModelStr + ".wmm";
	//	std::string coeffFile = metadatalFile + ".cof";
	//	if (!magFolder.empty() && !magModelStr.empty() && otCore::Paths::fileExists(metadatalFile) && otCore::Paths::fileExists(coeffFile))
	//	{
	//		if (magneticModel) delete magneticModel;
	//		magneticModel = nullptr;
	//
	//		if (shape && shape->getGeocentricObj()) {
	//			magneticModel = new GeographicLib::MagneticModel(magModelStr, magFolder, *shape->getGeocentricObj());
	//		}
	//		else {
	//			magneticModel = new GeographicLib::MagneticModel(magModelStr, magFolder, GeographicLib::Geocentric::WGS84());
	//		}
	//
	//		if (magneticModel) {
	//			modelLoaded = true;
	//			magneticModelType = model;
	//		}
	//	}
	//}
	//
	//if (!modelLoaded) {
	//	if (magneticModel) delete magneticModel;
	//	magneticModel = nullptr;
	//	magneticModelType = NO_MAGNETIC_MODEL;
	//}

	return modelLoaded;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool CelestialBody::Impl::loadGravityModel(GravityModelTypes model)
{
	bool modelLoaded = false;
	//if (model >= 0 && model < NumberOfGravityModelTypes)
	//{
	//	std::string gravModelStr = GRAVITY_MODEL_TYPE_STRINGS[model];
	//
	//	std::string gravityFolder = otCore::Paths::getDataDir() + "\\gravity";
	//	std::string metadatalFile = gravityFolder + "\\" + gravModelStr + ".egm";
	//	std::string coeffFile = metadatalFile + ".cof";
	//	if (!gravityFolder.empty() && !gravModelStr.empty() && otCore::Paths::fileExists(metadatalFile) && otCore::Paths::fileExists(coeffFile))
	//	{
	//		if (gravityModel) delete gravityModel;
	//		gravityModel = nullptr;
	//
	//		gravityModel = new GeographicLib::GravityModel(gravModelStr, gravityFolder);
	//
	//
	//		if (gravityModel) {
	//			modelLoaded = true;
	//			gravityModelType = model;
	//		}
	//	}
	//}
	//
	//if (!modelLoaded) {
	//	if (gravityModel) delete gravityModel;
	//	gravityModel = nullptr;
	//	gravityModelType = ELLIPSOID_GRAVITY;
	//}

	return modelLoaded;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CelestialBody::CelestialBody(const GUID& guid) : mImpl(new CelestialBody::Impl(guid))
{
	mImpl->centralBody = this; //initialize the central body to itself
	mImpl->rotationRateScalar = 1.0;

	//mImpl->shape = new Ellipsoid(semimajorRadius, semimajorRadius, semiminorRadius);
	mImpl->mass = physicalProperties.GM / gravitationalConstant;
	mImpl->volume = (4.0 / 3.0) * PI * physicalProperties.semimajorRadius * physicalProperties.semimajorRadius * physicalProperties.semiminorRadius;
	mImpl->density = mImpl->mass / mImpl->volume;

	mImpl->ECI2ECEFTransform.null();
	mImpl->ECEF2ECITransform.null();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CelestialBody::~CelestialBody()
{
	delete mImpl;
	mImpl = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::Initialize()
{
	if (!mImpl->initialized)
	{
		Propagate();
		double a = physicalProperties.semimajorRadius;
		double omega = physicalProperties.rateRotation;
		double GM = physicalProperties.GM;
		double f = 0;

		mImpl->worldRotation = physicalProperties.rotationAxis * omega;

		if (mImpl->shape) delete mImpl->shape;
		mImpl->shape = nullptr;

		if (physicalProperties.inverseFlattening == 0)
		{
			mImpl->shape = new Ellipsoid(Vector3(a, a, physicalProperties.semiminorRadius));
			if (a != physicalProperties.semiminorRadius) {
				physicalProperties.inverseFlattening = a / (a - physicalProperties.semiminorRadius);
				f = 1.0 / physicalProperties.inverseFlattening;
			}
		}
		else
		{
			mImpl->shape = new Ellipsoid(a, 1.0 / physicalProperties.inverseFlattening);
			f = 1.0 / physicalProperties.inverseFlattening;
			physicalProperties.semiminorRadius = (1.0 - f) * a;
		}

		mImpl->mass = GM / gravitationalConstant;
		mImpl->volume = (4.0 / 3.0) * PI * a * a * physicalProperties.semiminorRadius;
		mImpl->density = mImpl->mass / mImpl->volume;


		if (mImpl->normalGravityModel) delete mImpl->normalGravityModel;
		mImpl->normalGravityModel = nullptr;
		if (physicalProperties.J2 != 0.0) {
			mImpl->normalGravityModel = new GeographicLib::NormalGravity(a, GM, omega, physicalProperties.J2, false);
		}
		else {
			mImpl->normalGravityModel = new GeographicLib::NormalGravity(a, GM, omega, f, true);
		}

		mImpl->initialized = true;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setCelestialBodyType(CelestialBodyTypes celestialBodyType)
{
	mImpl->celestialBodyType = celestialBodyType;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setCentralBody(ICelestialBody* parentCentralBody)
{
	if (parentCentralBody && mImpl->centralBody != parentCentralBody) {
		mImpl->centralBody = parentCentralBody;
		mImpl->centralBodyGUID = mImpl->centralBody->getGUID();
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setCentralBody(const GUID& parentCentralBodyGUID)
{
	mImpl->centralBodyGUID = parentCentralBodyGUID;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Delete existing atmosphere and assign new one
//void CelestialBody::setAtmosphere(Weather::IAtmosphere* atmosphere)
//{
//	if (atmosphere && mImpl->atmosphere != atmosphere) {
//		if (mImpl->atmosphere) {
//			Weather::AtmosphereFactory::DestroyAtmosphere(mImpl->atmosphere.get());
//		}
//		mImpl->atmosphere = atmosphere;
//	}
//}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setMagneticModel(MagneticModelTypes magneticModelType)
{
	if (mImpl->magneticModelType != magneticModelType) {
		mImpl->loadMagneticModel(magneticModelType);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setGravityModel(GravityModelTypes gravityModelType)
{
	if (mImpl->gravityModelType != gravityModelType) {
		mImpl->loadGravityModel(gravityModelType);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::UpdateRotationMatrix()
{
	// Calculate the inertial to ECEF and transpose matrices using the rotation angle of the celestial body
	double cos_epa = cos(mImpl->angleRotation);
	double sin_epa = sin(mImpl->angleRotation);
	mImpl->ECI2ECEFTransform = Matrix33(cos_epa, sin_epa, 0.0,
		-sin_epa, cos_epa, 0.0,
		0.0, 0.0, 1.0);

	mImpl->ECEF2ECITransform = mImpl->ECI2ECEFTransform.transpose();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::update()
{
	if (mImpl->initialized)
		Propagate();
	else
		Initialize();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::computeOrbitalParameters(double julianDate)
{
	double T = (julianDate - orbitalElements.ephemerisDate) / julianCentury; //centuries

	mImpl->semimajorAxis =
		orbitalElements.semimajorAxis[0] +
		orbitalElements.semimajorAxis[1] * T;

	mImpl->eccentricity =
		orbitalElements.eccentricity[0] +
		orbitalElements.eccentricity[1] * T;

	mImpl->inclination =
		orbitalElements.inclination[0] +
		orbitalElements.inclination[1] * T;

	mImpl->meanLongitude =
		orbitalElements.meanLongitude[0] +
		orbitalElements.meanLongitude[1] * T;

	mImpl->longitudeOfPeriapsis =
		orbitalElements.longitudeOfPeriapsis[0] +
		orbitalElements.longitudeOfPeriapsis[1] * T;

	mImpl->longitudeOfAscendingNode =
		orbitalElements.longitudeOfAscendingNode[0] +
		orbitalElements.longitudeOfAscendingNode[1] * T;


	mImpl->argumentOfPeriapsis =
		mImpl->longitudeOfPeriapsis - mImpl->longitudeOfAscendingNode;

	mImpl->meanAnomaly =
		mImpl->meanLongitude - mImpl->longitudeOfPeriapsis +
		(orbitalElements.b * T*T) + (orbitalElements.c * cos(orbitalElements.f * T)) +
		(orbitalElements.s * sin(orbitalElements.f * T));

	while (mImpl->meanAnomaly < -180.0) mImpl->meanAnomaly += 360.0;
	while (mImpl->meanAnomaly > 180.0) mImpl->meanAnomaly -= 360.0;

	//aproximate eccentric anomaly
	double E0 = mImpl->meanAnomaly + (180.0 / PI) * mImpl->eccentricity *
		sin(DEGtoRAD(mImpl->meanAnomaly)) * (1.0 + mImpl->eccentricity *
			cos(DEGtoRAD(mImpl->meanAnomaly)));
	double E1 = 9999;
	double error = 9999;
	int iter = 1;
	int maxIt = 10;
	while (error > 1E-6) {
		E1 = E0 - (E0 - (180.0 / PI) * mImpl->eccentricity * sin(DEGtoRAD(E0)) -
			mImpl->meanAnomaly) / (1.0 - mImpl->eccentricity * cos(DEGtoRAD(E0)));
		iter++;
		error = abs(E1 - E0);
		E0 = E1;
		if (iter >= maxIt)
			break;
	}
	mImpl->eccentricAnomaly = E1;

	//compute the heliocentric coordinates in the orbital plane (r') with the x-axis aligned
	//from the focus to the perihelion
	mImpl->orbitalPlanePosition.x =
		mImpl->semimajorAxis * (cos(DEGtoRAD(mImpl->eccentricAnomaly)) - mImpl->eccentricity);

	mImpl->orbitalPlanePosition.y =
		mImpl->semimajorAxis * sqrt(1.0 - mImpl->eccentricity*mImpl->eccentricity) *
		sin(DEGtoRAD(mImpl->eccentricAnomaly));

	mImpl->orbitalPlanePosition.z = 0;

	//compute the coordinates in the J2000 ecliptic plane (recl), with the x-axis aligned toward
	//the equinox:   recl = Rz(-Ω)Rx(-I)Rz(-ω)r'
	double w = DEGtoRAD(mImpl->argumentOfPeriapsis);
	double O = DEGtoRAD(mImpl->longitudeOfAscendingNode);
	double I = DEGtoRAD(mImpl->inclination);

	//Matrix33 transform = Matrix33(
	//mImpl->J2000EclipticPlanePosition = mImpl->heliocentricOrbitalPlanePosition;

	mImpl->eclipticPlanePosition.x =
		(cos(w)*cos(O) - sin(w)*sin(O)*cos(I)) * mImpl->orbitalPlanePosition.x +
		(-sin(w)*cos(O) - cos(w)*sin(O)*cos(I)) * mImpl->orbitalPlanePosition.y;

	mImpl->eclipticPlanePosition.y =
		(cos(w)*sin(O) + sin(w)*cos(O)*cos(I)) * mImpl->orbitalPlanePosition.x +
		(-sin(w)*sin(O) + cos(w)*cos(O)*cos(I)) * mImpl->orbitalPlanePosition.y;

	mImpl->eclipticPlanePosition.z =
		(sin(w)*sin(I)) * mImpl->orbitalPlanePosition.x +
		(cos(w)*sin(I)) * mImpl->orbitalPlanePosition.y;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::Propagate()
{
	//allow this rotation vector to change with time
	double effectiveRateRotation = mImpl->rotationRateScalar * physicalProperties.rateRotation;
	mImpl->worldRotation = physicalProperties.rotationAxis * effectiveRateRotation;

	double julianDate = otCore::globalTime->getSimJulianDate();

	double rotationT = (julianDate - J2000EphemerisDate)*julianDay; //seconds

	setAngleRotation(rotationT*effectiveRateRotation);

	//if a body doesn't orbit another, don't calculate its orbital parameters
	if (mImpl->centralBody && mImpl->centralBody != this)
		computeOrbitalParameters(julianDate);

	if (mImpl->initialized) {
		getGravitationalAccelerationECEF(mImpl->shape->toECEF(Geodetic3(DEGtoRAD(16.321), DEGtoRAD(-73.45), 12000.0)));
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setAngleRotation(double angle)
{
	mImpl->angleRotation = angle;

	double tmp;
	if (mImpl->angleRotation >= 2.0*PI) {
		tmp = floor(mImpl->angleRotation / (2.0*PI));
		mImpl->angleRotation -= tmp*2.0*PI;
	}
	else if (mImpl->angleRotation < 0) {
		tmp = floor(mImpl->angleRotation / (2.0*PI));
		mImpl->angleRotation += tmp*2.0*PI;
	}

	UpdateRotationMatrix();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setPhysicalProperties(const CelestialBodyPhysicalProperties& _physicalProperties)
{
	physicalProperties = _physicalProperties;
	mImpl->initialized = false;
	Initialize();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setOrbitalElements(const CelestialBodyOrbitalElements& _orbitalElements)
{
	orbitalElements = _orbitalElements;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setRateRotationScalar(double scalar)
{
	mImpl->rotationRateScalar = scalar;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::setInternalGravityFactorTable(const dTable& table)
{
	if (table.getNumRows() > 1 && table.getNumColumns() == 1)
	{
		//delete mImpl->internalGravityFactorTable;

		//mImpl->internalGravityFactorTable = new dTable(table.getNumRows());

		*mImpl->internalGravityFactorTable = table;
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getMass(void) const
{
	return mImpl->mass;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getVolume(void) const
{
	return mImpl->volume;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getDensity(void) const
{
	return mImpl->density;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Vector3 CelestialBody::getGravitationalAccelerationECEF(const Vector3& ecef) const
{
	// Gravitation accel
	Vector3 J2Gravity;
	double radius = ecef.magnitude();

	if (physicalProperties.J2 > 0) {
		// use spherical celestial body latitude
		double latitude = 0.0;
		double radiusXY = sqrt(ecef[0]*ecef[0] + ecef[1]*ecef[1]);
		if (radiusXY != 0.0 || ecef[2] != 0.0)
			latitude = atan2(ecef[2], radiusXY);

		double sinLat = sin(latitude);

		double xy = 1.0 - 5.0*(sinLat*sinLat);
		double z = 3.0 - 5.0*(sinLat*sinLat);

		double adivr, preCommon, GMOverr2;

		if (radius >= physicalProperties.semiminorRadius) {
			adivr = physicalProperties.semimajorRadius / radius;
			preCommon = 1.5*physicalProperties.J2*adivr*adivr;
			GMOverr2 = physicalProperties.GM / (radius*radius);

			J2Gravity[0] = -GMOverr2 * ((1.0 + (preCommon * xy)) * ecef[0] / radius);
			J2Gravity[1] = -GMOverr2 * ((1.0 + (preCommon * xy)) * ecef[1] / radius);
			J2Gravity[2] = -GMOverr2 * ((1.0 + (preCommon *  z)) * ecef[2] / radius);
		}
		else {
			//below the surface, adjust the gravity value
			double internalGravityFactor = mImpl->internalGravityFactorTable->interp(radius / physicalProperties.semiminorRadius);

			adivr = physicalProperties.semimajorRadius / physicalProperties.semiminorRadius;
			preCommon = 1.5*physicalProperties.J2*adivr*adivr;
			GMOverr2 = physicalProperties.GM / (physicalProperties.semiminorRadius*physicalProperties.semiminorRadius);

			J2Gravity[0] = (-GMOverr2 * ((1.0 + (preCommon * xy)) * ecef[0] / radius)) * internalGravityFactor;
			J2Gravity[1] = (-GMOverr2 * ((1.0 + (preCommon * xy)) * ecef[1] / radius)) * internalGravityFactor;
			J2Gravity[2] = (-GMOverr2 * ((1.0 + (preCommon *  z)) * ecef[2] / radius)) * internalGravityFactor;
		}
	}
	else {
		J2Gravity = -(ecef * (getGravity(radius) / radius));
	}

	//if (mImpl->shape)
	//{
	//	Geodetic3 geod = mImpl->shape->toGeodetic3(ecef);
	//	double g_East = 0;
	//	double g_North = 0;
	//	double g_Up = 0;
	//	double w = 0;
	//	double u = 0;
	//	if (mImpl->gravityModel && mImpl->gravityModelType > GravityModelTypes::ELLIPSOID_GRAVITY) {
	//		w = mImpl->gravityModel->Gravity(RADtoDEG(geod.getLatitude()), RADtoDEG(geod.getLongitude()), geod.getHeight(), g_East, g_North, g_Up);
	//		Vector3 NED1_mps2 = Vector3(g_North, g_East, -g_Up);
	//
	//		u = mImpl->normalGravityModel->Gravity(RADtoDEG(geod.getLatitude()), geod.getHeight(), g_North, g_Up);
	//
	//		Vector3 NED2_mps2 = Vector3(g_North, 0, -g_Up);
	//
	//		double x = 0;
	//	}
	//	else if (mImpl->normalGravityModel && mImpl->gravityModelType == GravityModelTypes::ELLIPSOID_GRAVITY) {
	//		u = mImpl->normalGravityModel->Gravity(RADtoDEG(geod.getLatitude()), geod.getHeight(), g_North, g_Up);
	//
	//	}
	//	//TODO: Simple Model
	//}

	return J2Gravity;

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CelestialBody::getMagneticField(const Geodetic3& geodetic, double time_years, Vector3& NED_nT, double& decl_deg, double& incl_deg) const
{
	decl_deg = incl_deg = 0;
	NED_nT.init();
	double B_east = 0;
	double B_north = 0;
	double B_up = 0;
	//make sure magnetic model is loaded
	if (mImpl->magneticModel && mImpl->magneticModelType != MagneticModelTypes::NO_MAGNETIC_MODEL) {
		//next, get NUE magnetic field values
		mImpl->magneticModel->operator()(time_years, RADtoDEG(geodetic.getLatitude()), RADtoDEG(geodetic.getLongitude()), geodetic.getHeight(), B_east, B_north, B_up);
		//finally, calculate magnetic declination and inclination
		double H, F;
		mImpl->magneticModel->FieldComponents(B_east, B_north, B_up, H, F, decl_deg, incl_deg);
	}
	NED_nT = Vector3(B_north, B_east, -B_up);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getGravity(double radius) const
{
	if (radius >= physicalProperties.semiminorRadius) {
		return (physicalProperties.GM / (radius*radius));
	}
	else {
		//below the surface of the celestial body, use an adjustment factor to prevent the gravity value from rising
		double internalGravityFactor = mImpl->internalGravityFactorTable->interp(radius / physicalProperties.semiminorRadius);
		return (physicalProperties.GM / (physicalProperties.semiminorRadius*physicalProperties.semiminorRadius) * internalGravityFactor);
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getSLGravity(void) const
{
	return physicalProperties.GM / (physicalProperties.semimajorRadius*physicalProperties.semimajorRadius);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double CelestialBody::getAngleRotation() const
{
	return mImpl->angleRotation;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Vector3& CelestialBody::getRotationWorld() const
{
	return mImpl->worldRotation;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Matrix33& CelestialBody::getECI2ECEFTransform() const
{
	return mImpl->ECI2ECEFTransform;

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const Matrix33& CelestialBody::getECEF2ECITransform() const
{
	return mImpl->ECEF2ECITransform;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const GUID& CelestialBody::getGUID(void) const
{
	return mImpl->guid;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CelestialBodyTypes CelestialBody::getCelestialBodyType() const
{
	return mImpl->celestialBodyType;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ICelestialBody* CelestialBody::getCentralBody() const
{
	return mImpl->centralBody;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

const GUID& CelestialBody::getCentralBodyGUID(void) const
{
	return mImpl->centralBodyGUID;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//Weather::IAtmosphere* CelestialBody::getAtmosphere() const
//{
//	return mImpl->atmosphere;
//}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ellipsoid* CelestialBody::getShape(void)
{
	if (!mImpl->initialized)
		Initialize();
	return mImpl->shape;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otWorld

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%