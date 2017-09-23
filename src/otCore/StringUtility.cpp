/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       StringUtility.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
String utility functions

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "StringUtility.h"

#include <algorithm>
#include <shlobj.h>
#include <atlstr.h>
#include <cstdint>
#include <iomanip>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

std::string whitespaces(" \t\f\v\n\r");

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
char* StringUtility::getNonConstString(const std::string& s)
{
	//return non-constant copy of s.c_str()

	char *pTemp = new char[s.size()];
	strcpy(pTemp, &s[0]);
	return pTemp;
	//std::vector<char> char_array(s.begin(),s.end());
	//char_array.push_back('\0');
	//*charStr = &char_array[0];
}
#ifdef _MSC_VER
#pragma warning( pop )
#endif
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::stringReplaceAll(std::string s, const std::string &what, const std::string &with)
{
	size_t pos;
	do
	{
		pos = s.find(what);
		if (pos != -1)  s.replace(pos, what.length(), with);
	} while (pos != -1);
	return s;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void StringUtility::stringEraseAll(std::string &s, const char &what)
{
	s.erase(std::remove(s.begin(), s.end(), what), s.end());
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void StringUtility::stringTrim(std::string& s)
{
	size_t p = s.find_first_not_of(whitespaces);
	s.erase(0, p);

	p = s.find_last_not_of(whitespaces);
	if (std::string::npos != p)
		s.erase(p + 1);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::stringToLower(const std::string& s)
{
	std::string lowerStr = s;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::stringToUpper(const std::string& s)
{
	std::string upperStr = s;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
	return upperStr;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> &StringUtility::tokenizeString(const std::string &s, const char token, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, token)) {
		elems.push_back(item);
	}
	return elems;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> StringUtility::tokenizeString(const std::string &s, const char token) {
	std::vector<std::string> elems;
	tokenizeString(s, token, elems);
	return elems;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::addExtension(const std::string& file, const std::string& extension)
{
	if (extension.length() > 0) {
		if (extension[0] == '.')
			return (file + extension);
		else
			return (file + "." + extension);
	}
	else
		return file;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getFileName(const std::string& filePath)
{
	size_t lastslash = filePath.find_last_of("/\\");

	if (lastslash == std::string::npos)
		return filePath;
	else if (lastslash == (filePath.length() - 1))
		return "";
	else
		return filePath.substr(lastslash + 1);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getFileNameWithoutExtension(const std::string& filePath)
{
	return removeFileExtension(getFileName(filePath));
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getFileExtension(const std::string& filePath)
{
	size_t lastdot = filePath.find_last_of(".");

	//if a dot was not found OR it was the last character, fail
	if (lastdot == std::string::npos || lastdot == (filePath.length() - 1))
		return "";

	//make sure the dot wasn't related to "../" or "./" relative directory commands
	if (filePath.at(lastdot - 1) == '.' || filePath.at(lastdot + 1) == '/')
		return "";

	return filePath.substr(lastdot + 1);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getFolder(const std::string& filePath)
{
	std::string folderName = "";
	size_t lastslash = filePath.find_last_of("/\\");

	if (getFileExtension(filePath) != "") {
		//has a file extension, so path is likely ending with a file name, not a folder
		if (lastslash == std::string::npos)
			return filePath;
		else
			folderName = filePath.substr(0, lastslash + 1);
	}
	else {
		//no file extension, so path is likely ending with a folder name, not a file
		if (lastslash == std::string::npos)
			folderName = addTrailingSlash(filePath);
		else {
			if (filePath.at(lastslash) == '\\')
				folderName = addTrailingSlash(filePath, true);
			else if (filePath.at(lastslash) == '/')
				folderName = addTrailingSlash(filePath);
		}
	}
	return folderName;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::removeFileExtension(const std::string& filePath)
{
	size_t lastdot = filePath.find_last_of(".");
	if (lastdot == std::string::npos)
		return filePath;

	//make sure the dot wasn't related to "../" relative directory command
	if (filePath.at(lastdot - 1) == '.')
		return filePath;

	//make sure the dot wasn't related to  "./" current directory command
	if (lastdot != (filePath.length() - 1) && filePath.at(lastdot + 1) == '/')
		return filePath;

	return filePath.substr(0, lastdot);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::addTrailingSlash(const std::string& path, bool backslash)
{
	if (path.length() > 0)
	{
		if (path.back() == '\\' || path.back() == '/')
			return path;
		else
		{
			if (!backslash)
				return path + "/";
			else
				return path + "\\";
		}
	}
	else
		return path;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

///Removes trailing slash from the and of the given path, if it exists
std::string StringUtility::removeTrailingSlash(const std::string& path)
{
	if (path.length() > 0)
	{
		if (path.back() == '\\' || path.back() == '/')
			return path.substr(0, path.size() - 1);
		else
			return path;
	}
	else
		return path;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool StringUtility::toBool(const std::string& s, bool& bVal)
{
	std::string s_lower = stringToLower(s);

	if (s_lower == "t" || s == "true" || s == "1") {
		bVal = true;
		return true;
	}
	else if (s == "f" || s == "false" || s == "0") {
		bVal = false;
		return true;
	}
	else {
		int numVal;
		if (toNumber(s, numVal)) {
			if (numVal > 0) {
				bVal = true;
				return true;
			}
			else {
				bVal = false;
				return true;
			}
		}
		else {
			bVal = false;
			return false;
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getLocalAppDataPath()
{
	LPWSTR localAppDataPath = nullptr;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppDataPath);

	if (SUCCEEDED(hr)) {
		std::string localAppDataPathString = CW2A(localAppDataPath);
		return localAppDataPathString;
	}

	return "";
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getTimeString(uint64_t timeMS, TimeStringFormats format)
{
	std::ostringstream o;

	uint64_t iSec = timeMS / 1000;
	uint64_t iMin = timeMS / 60000;
	uint64_t iHr = timeMS / 3600000;

	switch (format) {
	case TimeStringFormats::S:
	{
		o << iSec;
		break;
	}
	case TimeStringFormats::S_FLOAT1:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(1);
		o << (double)iSec + (double)(timeMS - iSec * 1000) / 1000.0;
		break;
	}
	case TimeStringFormats::S_FLOAT2:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (double)iSec + (double)(timeMS - iSec * 1000) / 1000.0;
		break;
	}
	case TimeStringFormats::S_FLOAT3:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(3);
		o << (double)iSec + (double)(timeMS - iSec * 1000) / 1000.0;
		break;
	}
	case TimeStringFormats::MMSS:
	{
		o << iMin << ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	case TimeStringFormats::MMMSS:
	{
		o << std::setfill('0') << std::setw(3) << iMin
			<< ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	case TimeStringFormats::HHMMSS:
	{
		o << iHr << ":" << std::setfill('0') << std::setw(2) << (iMin - iHr * 60)
			<< ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	case TimeStringFormats::CLOCK:
	{
		uint64_t iHr_24 = iHr;
		uint64_t min = 0ULL;
		uint64_t max = 24ULL;
		uint64_t range = max - min;
		double normpos = (iHr - min) / double(max - min);
		if (iHr > max)
			iHr_24 = uint64_t((normpos - (double)int(normpos)) * range) + min;
		else if (iHr < min)
			iHr_24 = uint64_t((normpos - (double)int(normpos)) * range) + max;

		o << std::setfill('0') << std::setw(2) << iHr_24
			<< ":" << std::setfill('0') << std::setw(2) << (iMin - iHr * 60)
			<< ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	case TimeStringFormats::DHHMMSS:
	{
		uint64_t iDay = timeMS / 86400000;
		if (iDay > 0)
			o << iDay << "d ";

		o << std::setfill('0') << std::setw(2) << (iHr - iDay * 24)
			<< ":" << std::setfill('0') << std::setw(2) << (iMin - iHr * 60)
			<< ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	case TimeStringFormats::YDHHMMSS:
	{
		uint64_t iDay = timeMS / 86400000;
		uint64_t iYr = timeMS / 86400000 / 365;
		if (iYr > 0) {
			o << iYr << "y " << (iDay - iYr * 365) << "d ";
		}
		else {
			if (iDay > 0)
				o << iDay << "d ";
		}

		o << std::setfill('0') << std::setw(2) << (iHr - iDay * 24)
			<< ":" << std::setfill('0') << std::setw(2) << (iMin - iHr * 60)
			<< ":" << std::setfill('0') << std::setw(2) << (iSec - iMin * 60);
		break;
	}
	default:

		break;
	}

	return o.str();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::getBandwidthString(uint64_t bytes, BandwidthStringFormats format)
{
	std::ostringstream o;

	switch (format) {
	case BandwidthStringFormats::bps:
	{
		o << bytes * 8 << " bps";
		break;
	}
	case BandwidthStringFormats::kbps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 128.0) << " kbps";
		break;
	}
	case BandwidthStringFormats::mbps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 131072.0) << " mbps";
		break;
	}
	case BandwidthStringFormats::gbps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 134217728.0) << " gbps";
		break;
	}
	case BandwidthStringFormats::Bps:
	{
		o << bytes << " bytes/s";
		break;
	}
	case BandwidthStringFormats::KBps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1024.0) << " KB/s";
		break;
	}
	case BandwidthStringFormats::MBps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1048576.0) << " MB/s";
		break;
	}
	case BandwidthStringFormats::GBps:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1073741824.0) << " GB/s";
		break;
	}
	case BandwidthStringFormats::bytes:
	{
		o << bytes << " bytes";
		break;
	}
	case BandwidthStringFormats::KB:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1024.0) << " KB";
		break;
	}
	case BandwidthStringFormats::MB:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1048576.0) << " MB";
		break;
	}
	case BandwidthStringFormats::GB:
	{
		o.setf(std::ios_base::fixed, std::ios_base::floatfield);
		o.precision(2);
		o << (bytes / 1073741824.0) << " GB";
		break;
	}
	case BandwidthStringFormats::rateAdaptive:
	{
		uint64_t bits = bytes * 8;
		if (bits < 1024)
			o << bits << " bps";
		else {
			o.setf(std::ios_base::fixed, std::ios_base::floatfield);
			o.precision(2);

			if (bits < 1048576)
				o << (bits / 1024.0) << " kbps";
			else if (bits < 1073741824)
				o << (bits / 1048576.0) << " mbps";
			else
				o << (bits / 1073741824.0) << " gbps";
		}

		break;
	}
	case BandwidthStringFormats::totalAdaptive:
	{
		if (bytes < 1024)
			o << bytes << " bytes";
		else {
			o.setf(std::ios_base::fixed, std::ios_base::floatfield);
			o.precision(2);

			if (bytes < 1048576)
				o << (bytes / 1024.0) << " KB";
			else if (bytes < 1073741824)
				o << (bytes / 1048576.0) << " MB";
			else
				o << (bytes / 1073741824.0) << " GB";
		}

		break;
	}
	default:

		break;
	}

	return o.str();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string StringUtility::fillStringR(const std::string& str, int width, char fillChar) {
	std::ostringstream o;
	o << std::setfill(fillChar) << std::setw(width) << str;

	return o.str();
}

std::string StringUtility::fillStringR(const std::stringstream& ss, int width, char fillChar) {
	return fillStringR(ss.str(), width, fillChar);
}

std::string StringUtility::fillStringR(const char* str, int width, char fillChar) {
	return fillStringR(std::string(str), width, fillChar);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%