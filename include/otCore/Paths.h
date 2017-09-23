/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Paths.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Paths_H
#define Paths_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <string>
#include <vector>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

namespace otCore {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Path utility functions

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class CORE_API Paths
{
public:
	///////////////////
	//Paths based in the install directory
	///////////////////

	/// Returns the path to the install directory
	static std::string getBinDir();

	/// Returns the path to the plugins directory
	static std::string getPluginsDir();

	/// Returns the path to the packages directory
	static std::string getPackagesDir();


	///////////////////
	//Paths based in the package directory
	///////////////////

	/// Returns the path to the package base directory
	static std::string getBaseDir();

	/// Returns the path to the base data directory
	static std::string getDataDir();

	/// Returns the path to the base addons directory
	static std::string getAddonsDir();

	/// Returns the path to the base configuration directory
	static std::string getConfigDir();

	/// Returns the path to the base inputs directory
	static std::string getInputDir();

	/// Returns the path to the base scenarios directory
	static std::string getScenariosDir();


	///////////////////
	//Paths based on the custom preferences directory in Saved Games
	///////////////////

	/// Returns the path to the user's Saved Games directory
	static std::string getSavedGamesDir();

	/// Returns the path to the game preferences directory
	static std::string getGamePreferencesDir();

	/// Returns the path to the custom addons
	static std::string getCustomAddonsDir();

	/// Returns the path to the custom configuration directory
	static std::string getCustomConfigDir();

	/// Returns the path to the custom inputs directory
	static std::string getCustomInputDir();

	/// Returns the path to the custom scenarios directory
	static std::string getCustomScenariosDir();

	/// Returns the path to the logs directory
	static std::string getLogsDir();

	/// Returns the path to the screenshots directory
	//static std::string getScreenshotsDir();


	///////////////////
	//Utility functions
	///////////////////

	/// Returns boolean based on whether the given directory path exists
	static bool pathExists(const std::string &path);

	/// Returns boolean based on whether the given file path exists
	static bool fileExists(const std::string &filePath);

	///Return a vector list of all folders in the given folder.
	///Set the returnAbsolutePath flag depending on whether absolute paths are needed for the returned folder list folders.
	static std::vector<std::string> findFoldersInFolder(const std::string& folder, bool returnAbsolutePath);

	///Return a vector list of all files in the given folder with the given extension (no dot).
	///Multiple extensions can be defined with comma separators. Ex: "json,cfg".
	///Set the returnAbsolutePath flag depending on whether absolute paths are needed for the returned file list files.
	static std::vector<std::string> findFilesInFolder(const std::string& folder, const std::string& extensionsFilter, bool returnAbsolutePath = false);

};


} //namespace otCore

#endif //Paths_H