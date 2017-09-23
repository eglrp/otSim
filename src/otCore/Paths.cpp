/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Paths.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Path utility functions

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Paths.h"

#include <shlobj.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "StringUtility.h"

#ifdef WIN32
#ifndef S_ISDIR
#define S_ISDIR(mode)  (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif
#endif

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

#ifdef WIN32
std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string UsersPath() {
	char buffer[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, buffer);
	return std::string(buffer);
	//std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
	//return std::string( buffer ).substr( 0, pos);
}
#endif

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Paths::pathExists(const std::string &path)
{
	struct stat s;
	int err = stat(path.c_str(), &s);
	if (-1 == err) {
		if (ENOENT == errno) {
			/* does not exist */
			return false;
		}
		else {
			perror("stat");
			exit(1);
		}
	}
	else {
		if (S_ISDIR(s.st_mode)) {
			/* it's a dir */
			return true;
		}
		else {
			/* exists but is no dir */
			return false;
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Paths::fileExists(const std::string &filePath)
{
	struct stat buf;
	if (stat(filePath.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getBinDir() {
	return ExePath();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getPluginsDir()
{
	return (ExePath() += "\\plugins");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getPackagesDir()
{
	return (ExePath() += "\\packages");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getBaseDir() {
	return (getPackagesDir() += "\\otSim");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getDataDir() {
	return (getBaseDir() += "\\data");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getSavedGamesDir() {
	return (UsersPath() += "\\Saved Games");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getGamePreferencesDir() {
	return (getSavedGamesDir() += "\\otSim");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getAddonsDir() {
	return (getBaseDir() += "\\addons");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getCustomAddonsDir() {
	return (getGamePreferencesDir() += "\\addons");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getConfigDir() {
	return (getBaseDir() + "\\config");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getCustomConfigDir() {
	return (getGamePreferencesDir() += "\\config");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getInputDir() {
	return (getConfigDir() + "\\input");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getCustomInputDir() {
	return (getCustomConfigDir() + "\\input");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getScenariosDir() {
	return (getBaseDir() + "\\scenarios");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getCustomScenariosDir() {
	return (getGamePreferencesDir() + "\\scenarios");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string Paths::getLogsDir() {
	return (getGamePreferencesDir() + "\\logs");
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//std::string Paths::getScreenshotsDir() {
//	return (getGamePreferencesDir() + "\\screenshots");
//}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> Paths::findFoldersInFolder(const std::string& folder, bool returnAbsolutePath)
{
	std::vector<std::string> folders;

	std::string searchFolder = StringUtility::getFolder(folder);

#ifdef _WIN32

	std::string filter = searchFolder + "*";

	WIN32_FIND_DATA findData;
	HANDLE hFind = FindFirstFile(filter.c_str(), &findData);

	if (hFind != 0 && hFind != INVALID_HANDLE_VALUE) {
		do
		{
			std::string folder(findData.cFileName);

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && folder != "." && folder != "..")
			{
				//std::string folder(findData.cFileName);
				if (returnAbsolutePath)
					folder = searchFolder + folder;

				folders.push_back(folder);
			}

		} while (FindNextFile(hFind, &findData));
	}
#else
	//TODO: Finish this
	/*
	DIR *dir;
	class dirent *ent;
	class stat st;
	dir = opendir(searchFolder);
	while ((ent = readdir(dir))) {
	const std::string file_name = ent->d_name;
	const std::string full_file_name = searchFolder + file_name;

	if (file_name[0] == '.')
	continue;

	if (stat(full_file_name.c_str(), &st) == -1)
	continue;

	const bool is_directory = (st.st_mode & S_IFDIR) != 0;

	if (is_directory)
	continue;

	for (size_t i = 0; i < fileExtensions.size(); i++)
	{
	if (fileExtensions.at(i) == getFileExtension(file_name))
	{
	if (returnAbsolutePath)
	files.push_back(full_file_name);
	else
	files.push_back(file_name);
	}
	}
	}
	closedir(dir);
	*/
#endif

	return folders;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> Paths::findFilesInFolder(const std::string& folder, const std::string& extensions, bool returnAbsolutePath)
{
	std::vector<std::string> files;

	std::vector<std::string> fileExtensions = StringUtility::tokenizeString(extensions, ',');
	std::string searchFolder = StringUtility::getFolder(folder);

#ifdef _WIN32

	for (size_t i = 0; i < fileExtensions.size(); i++)
	{
		std::string filter = searchFolder + "*." + fileExtensions.at(i);

		WIN32_FIND_DATA findData;
		HANDLE hFind = FindFirstFile(filter.c_str(), &findData);

		if (hFind != 0 && hFind != INVALID_HANDLE_VALUE)
			do
			{
				std::string file(findData.cFileName);
				if (returnAbsolutePath)
					file = searchFolder + file;

				files.push_back(file);
			} while (FindNextFile(hFind, &findData));
	}
#else
	DIR *dir;
	class dirent *ent;
	class stat st;
	dir = opendir(searchFolder);
	while ((ent = readdir(dir))) {
		const std::string file_name = ent->d_name;
		const std::string full_file_name = searchFolder + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		for (size_t i = 0; i < fileExtensions.size(); i++)
		{
			if (fileExtensions.at(i) == StringUtility::getFileExtension(file_name))
			{
				if (returnAbsolutePath)
					files.push_back(full_file_name);
				else
					files.push_back(file_name);
			}
		}
	}
	closedir(dir);
#endif

	return files;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%