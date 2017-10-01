/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       AddonManager.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Addon Manager class

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "AddonManager.h"
#include "Paths.h"
#include "JSON.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otMain {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class AddonManager::Impl
{
public:
	//Constructor
	Impl(void)
	{
	}
	~Impl(void)
	{
		//Must delete addons manually
		for (unsigned int i = 0; i<addonList.size(); i++) {
			delete addonList.at(i);
		}
		addonList.clear();
	}

	std::vector<Addon*> addonList;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

AddonManager::AddonManager() : mImpl(new Impl())
{

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

AddonManager::~AddonManager()
{
	delete mImpl;
	mImpl = nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void AddonManager::enumerateAddons()
{
	//Core::FSLog::message("Enumerating addons ...");

	std::string baseAddonsDir = otCore::Paths::getAddonsDir();
	std::string customAddonsDir = otCore::Paths::getCustomAddonsDir();

	std::vector<std::string> foldersToOpen = otCore::Paths::findFoldersInFolder(baseAddonsDir, true);
	std::vector<std::string> customFoldersToOpen = otCore::Paths::findFoldersInFolder(customAddonsDir, true);
	foldersToOpen.insert(foldersToOpen.end(), customFoldersToOpen.begin(), customFoldersToOpen.end());

	for (unsigned int i = 0; i<foldersToOpen.size(); i++)
	{
		std::string addonFile = foldersToOpen.at(i) + "\\addon.json";
		if (otCore::Paths::fileExists(addonFile)) {
			otCore::JSON addonJson;
			if (addonJson.readFile(addonFile)) {
				std::string addon_name = addonJson.getValue("addon_name", std::string(""));
				if (!addon_name.empty())
				{
					if (!mImpl->addonList.empty())
					{
						//check existing addons for duplicates before adding the new one
						for (unsigned int j = 0; j<mImpl->addonList.size(); j++) {
							if (mImpl->addonList.at(j)->getAddonName() == addon_name) {
								return;
							}
						}
					}
					Addon* newAddon = new Addon(addon_name);

					newAddon->setPath(foldersToOpen.at(i));

					newAddon->setAuthor(addonJson.getValue("author", std::string("")));

					newAddon->setVersion(addonJson.getValue("version", std::string("")));

					newAddon->setDescription(addonJson.getValue("description", std::string("")));

					std::vector<std::string> objects;

					objects = addonJson.getValueStringArray("objects");

					for (unsigned int j = 0; j < objects.size(); j++) {
						if (!objects.at(j).empty())
							newAddon->addObject(objects.at(j));
					}

					mImpl->addonList.push_back(newAddon);
				}
				else {
					//Core::FSLog::error("addon_name cannot be determined: %s", addonFile.c_str());
				}
			}
			else {
				//Core::FSLog::error("Addon configuration file cannot be read: %s", addonFile.c_str());
			}
		}
		else {
			//Core::FSLog::warning("Addon configuration file cannot be found, skipping: %s", addonFile.c_str());
		}
	}
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int AddonManager::getNumberAddons() const
{
	return mImpl->addonList.size();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Addon* AddonManager::getAddonByName(const std::string& addon_name) const
{
	for (unsigned int i = 0; i<mImpl->addonList.size(); i++) {
		if (mImpl->addonList.at(i)->getAddonName() == addon_name) {
			return mImpl->addonList.at(i);
		}
	}
	return nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Addon* AddonManager::getAddonByIndex(unsigned int addonIndex) const
{
	if (addonIndex < mImpl->addonList.size()) {
		return mImpl->addonList.at(addonIndex);
	}
	return nullptr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otMain

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%