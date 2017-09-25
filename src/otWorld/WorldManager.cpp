/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       WorldManager.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
World Manager class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "WorldManager.h"

#include <vector>

#include "CelestialBody.h"
#include "CelestialBodyFactory.h"
#include "GUID.h"
#include "JSON.h"
#include "Table.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otWorld {

class WorldManager::Impl
{
public:
	Impl() : numberCelestialBodies(0),
		solarSystemChanged(true)
	{

	}

	~Impl()
	{
		for (auto body : celestialBodies) {
			delete body;
		}
		celestialBodies.clear();
	}

	unsigned int numberCelestialBodies;
	bool solarSystemChanged;
	std::vector<ICelestialBody*> celestialBodies;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

WorldManager::WorldManager(void)
{
	mImpl = new WorldManager::Impl();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

WorldManager::~WorldManager(void)
{
	delete mImpl;
	mImpl = 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int WorldManager::getNumberCelestialBodies() const
{
	return mImpl->numberCelestialBodies;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool WorldManager::addCelestialBody(ICelestialBody* celestialBody)
{
	if (!celestialBody) return false;

	//don't allow duplicates
	const GUID& guid = celestialBody->getGUID();
	for (auto body : mImpl->celestialBodies) {
		if (body->getGUID() == guid)
			return false;
	}

	//Add the body to the main physics object list
	mImpl->celestialBodies.push_back(celestialBody);
	mImpl->numberCelestialBodies++;
	mImpl->solarSystemChanged = true;

	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool WorldManager::removeCelestialBody(ICelestialBody* celestialBody)
{
	if (!celestialBody) return false;

	for (unsigned int i = 0; i < mImpl->celestialBodies.size(); i++) {
		if (mImpl->celestialBodies.at(i) == celestialBody) {
			mImpl->celestialBodies.erase(mImpl->celestialBodies.begin() + i);
			break;
		}
	}

	mImpl->numberCelestialBodies--;
	mImpl->solarSystemChanged = true;
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ICelestialBody* WorldManager::getCelestialBody(const GUID& guid) const
{
	for (auto body : mImpl->celestialBodies) {
		if (body->getGUID() == guid)
			return body;
	}
	return nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void WorldManager::update()
{

	if (mImpl->solarSystemChanged)
		updateSolarSystemHierarchy();

	for (auto body : mImpl->celestialBodies) {
		if (body) {
			static_cast<CelestialBody*>(body)->update();
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void WorldManager::updateSolarSystemHierarchy()
{
	//set up hierarchy
	int i = 0;
	bool found = false;
	for (auto body : mImpl->celestialBodies) {
		if (body) {
			GUID centralBodyGUID = body->getCentralBodyGUID();
			int j = 0;
			for (auto otherBody : mImpl->celestialBodies) {
				if ((i != j) && otherBody && (otherBody->getGUID() == centralBodyGUID)) {
					body->setCentralBody(otherBody);
					found = true;
					break;
				}
				j++;
			}
			// if body wasn't found with GUID, set central body to itself
			if (!found) body->setCentralBody(body);
		}
		i++;
	}

	mImpl->solarSystemChanged = false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool WorldManager::parseCelestialBodyConfig(const std::string& file)
{
	otCore::JSON json;

	if (json.readFile(file))
	{
		//define required fields
		std::vector<std::string> requiredFields;
		requiredFields.push_back("internalName");
		requiredFields.push_back("GUID");
		requiredFields.push_back("objectType");
		requiredFields.push_back("celestialType");

		bool validFile = true;
		for (auto &field : requiredFields) {
			if (!json.hasObject(field)) {
				validFile = false;
				break;
			}
		}

		if (validFile)
		{
			GUID guid = otCore::GUID_NULL;
			guid = otCore::stringToGUID(json.getValue("GUID", otCore::GUID_NULL_STR));
			if (guid != otCore::GUID_NULL)
			{
				ICelestialBody* body = CelestialBodyFactory::CreateCelestialBody(guid);
				if (body)
				{
					GUID centralBodyGUID = otCore::stringToGUID(json.getValue("centralBody", otCore::GUID_NULL_STR));
					if (centralBodyGUID != otCore::GUID_NULL) {
						body->setCentralBody(centralBodyGUID);
					}
					int celestialType = getCelestialBodyTypeFromString(json.getValue("celestialType", std::string("")));
					if (celestialType >= 0) {
						body->setCelestialBodyType(static_cast<CelestialBodyTypes>(celestialType));
					}

					if (json.hasObject("orbitalElements"))
					{
						CelestialBodyOrbitalElements orbElm;
						orbElm.ephemerisDate = json.getValue("orbitalElements.ephemerisDate", 2451545.0);

						std::vector<double> orbElmVec1 = json.getValueNumericArray("orbitalElements.semimajorAxis");
						if (orbElmVec1.size() > 0) {
							orbElm.semimajorAxis[0] = orbElmVec1.at(0);
							if (orbElmVec1.size() > 1) orbElm.semimajorAxis[1] = orbElmVec1.at(1);
						}

						std::vector<double> orbElmVec2 = json.getValueNumericArray("orbitalElements.eccentricity");
						if (orbElmVec2.size() > 0) {
							orbElm.eccentricity[0] = orbElmVec2.at(0);
							if (orbElmVec2.size() > 1) orbElm.eccentricity[1] = orbElmVec2.at(1);
						}

						std::vector<double> orbElmVec3 = json.getValueNumericArray("orbitalElements.inclination");
						if (orbElmVec3.size() > 0) {
							orbElm.inclination[0] = orbElmVec3.at(0);
							if (orbElmVec3.size() > 1) orbElm.inclination[1] = orbElmVec3.at(1);
						}

						std::vector<double> orbElmVec4 = json.getValueNumericArray("orbitalElements.meanLongitude");
						if (orbElmVec4.size() > 0) {
							orbElm.meanLongitude[0] = orbElmVec4.at(0);
							if (orbElmVec4.size() > 1) orbElm.meanLongitude[1] = orbElmVec4.at(1);
						}

						std::vector<double> orbElmVec5 = json.getValueNumericArray("orbitalElements.longitudeOfPeriapsis");
						if (orbElmVec5.size() > 0) {
							orbElm.longitudeOfPeriapsis[0] = orbElmVec5.at(0);
							if (orbElmVec5.size() > 1) orbElm.longitudeOfPeriapsis[1] = orbElmVec5.at(1);
						}

						std::vector<double> orbElmVec6 = json.getValueNumericArray("orbitalElements.longitudeOfAscendingNode");
						if (orbElmVec6.size() > 0) {
							orbElm.longitudeOfAscendingNode[0] = orbElmVec6.at(0);
							if (orbElmVec6.size() > 1) orbElm.longitudeOfAscendingNode[1] = orbElmVec6.at(1);
						}

						orbElm.b = json.getValue("orbitalElements.b", 0.0);
						orbElm.c = json.getValue("orbitalElements.c", 0.0);
						orbElm.s = json.getValue("orbitalElements.s", 0.0);
						orbElm.f = json.getValue("orbitalElements.f", 0.0);

						body->setOrbitalElements(orbElm);
					}

					if (json.hasObject("physicalProperties"))
					{
						CelestialBodyPhysicalProperties physProp;
						physProp.GM = json.getValue("physicalProperties.GM", 1.0E3);
						physProp.J2 = json.getValue("physicalProperties.J2", 0.0);
						physProp.rateRotation = json.getValue("physicalProperties.rateRotation", 0.0);
						physProp.semimajorRadius = json.getValue("physicalProperties.semimajorRadius", 1000.0);
						physProp.semiminorRadius = json.getValue("physicalProperties.semiminorRadius", 1000.0);
						physProp.inverseFlattening = json.getValue("physicalProperties.inverseFlattening", 0.0);
						physProp.geometricAlbedo = json.getValue("physicalProperties.geometricAlbedo", 0.1);
						physProp.solarConstant = json.getValue("physicalProperties.solarConstant", 0.0);
						std::vector<double> rotationAxisJsonVector = json.getValueNumericArray("physicalProperties.rotationAxis");
						if (rotationAxisJsonVector.size() == 3)
						{
							physProp.rotationAxis.x = rotationAxisJsonVector.at(0);
							physProp.rotationAxis.y = rotationAxisJsonVector.at(1);
							physProp.rotationAxis.z = rotationAxisJsonVector.at(2);
						}

						body->setPhysicalProperties(physProp);
					}

					int atmosphereType = getAtmosphereTypeFromString(json.getValue("atmosphere", std::string("No_Atmosphere")));
					//TODO: Add Atmosphere Models
					//if (atmosphereType > 0)
					//	body->setAtmosphere(static_cast<AtmosphereTypes>(atmosphereType));

					int magneticModelType = getMagneticModelTypeFromString(json.getValue("magneticModel", std::string("None")));
					if (magneticModelType >= 0) {
						body->setMagneticModel(static_cast<MagneticModelTypes>(magneticModelType));
					}

					int gravityModelType = getGravityModelTypeFromString(json.getValue("gravityModel", std::string("Ellipsoid")));
					if (gravityModelType >= 0) {
						body->setGravityModel(static_cast<GravityModelTypes>(gravityModelType));
					}


					if (json.hasObject("internalGravityFactorTable.radiusFraction") && json.hasObject("internalGravityFactorTable.gravityFraction"))
					{
						std::vector<double> radFracVec = json.getValueNumericArray("internalGravityFactorTable.radiusFraction");
						std::vector<double> gravFracVec = json.getValueNumericArray("internalGravityFactorTable.gravityFraction");
						//if (radFracVec.size() != gravFracVec.size())
						//	Core::FSLog::warning("Json arrays sizes for x and y do not match, using smallest array size for table.");

						size_t sizeArray = std::min(radFracVec.size(), gravFracVec.size());

						dTable table(sizeArray);

						for (unsigned int j = 0; j < sizeArray; j++) {
							//		 Radius Frac			Gravity Frac
							//		 -----------			-----------
							table << radFracVec.at(j) << gravFracVec.at(j);
						}

						body->setInternalGravityFactorTable(table);
					}


					return true;
				}
			}

		}
	}
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otWorld

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%