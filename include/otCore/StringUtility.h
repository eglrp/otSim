/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       StringUtility.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef StringUtility_H
#define StringUtility_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <string>
#include <istream>
#include <sstream>
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

enum class TimeStringFormats {
	S = 0,		//seconds only - always increasing
	MMSS,		//mm::ss - minutes can go higher than 59
	HHMMSS,		//HH:mm::ss - hours can go higher than 23
	DHHMMSS,	//d HH:mm:ss - days increment at 24 hours and always increasing
	YDHHMMSS,	//y d HH:mm::ss - years increment at 365 days

	S_FLOAT1,	//seconds "X.Y"
	S_FLOAT2,	//seconds "X.YY"
	S_FLOAT3,	//seconds "X.YYY"

				//Special
				MMMSS,		//mmm::ss - exactly 3 digits on minutes (Max 999 minutes, 59 seconds)
				CLOCK,		//HH:mm::ss - maximum time is 23:59:59
};
enum class BandwidthStringFormats {
	bps = 0,	//bits/sec
	kbps,		//kilobits/sec
	mbps,		//megabits/sec
	gbps,		//gigabits/sec
	Bps,		//bytes/sec
	KBps,		//Kilobytes/sec
	MBps,		//Megabytes/sec
	GBps,		//Gigabytes/sec
	bytes,		//bytes
	KB,			//Kilobytes
	MB,			//Megabytes
	GB,			//Gigabytes
	rateAdaptive, //bps, kbps, mbps, or gbps depending on rate
	totalAdaptive, //bytes, KB, MB, or GB depending on amount
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** String utility functions

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class CORE_API StringUtility
{
public:
	///Returns a non constant c string from a given standard string
	static char* getNonConstString(const std::string& s);

	///Replace all instances of substring 'what' within string 's' with string 'with'
	static std::string stringReplaceAll(std::string s, const std::string &what, const std::string &with);

	///Erase all 'what' characters in string 's'
	static void stringEraseAll(std::string &s, const char &what);

	///Trim string 's' of all whitespace
	static void stringTrim(std::string& s);

	///Convert string to lower case
	static std::string stringToLower(const std::string& s);

	///Convert string to upper case
	static std::string stringToUpper(const std::string& s);

	///Tokenize the given string by the given token character
	///Uses a pre-constructed vector
	static std::vector<std::string> &tokenizeString(const std::string &s, const char token, std::vector<std::string> &elems);

	///Tokenize the given string by the given token character
	///Returns a new vector
	static std::vector<std::string> tokenizeString(const std::string &s, const char token);


	///////////////////////////
	//  File/Path Functions  //
	///////////////////////////

	///Returns file name with extension from the given full path
	static std::string getFileName(const std::string& filePath);

	///Returns file name without extension from the given full path
	static std::string getFileNameWithoutExtension(const std::string& filePath);

	///Returns file extension without dot from the given full path
	static std::string getFileExtension(const std::string& filePath);

	///Returns folder name path from the given full path
	static std::string getFolder(const std::string& filePath);

	///Removes the file extension from the given full path
	static std::string removeFileExtension(const std::string& filePath);

	///Adds the given file extension to the given file
	static std::string addExtension(const std::string& file, const std::string& extension);

	///Adds trailing slash to the end of the given path, if it is missing
	static std::string addTrailingSlash(const std::string& path, bool backslash = false);

	///Removes trailing slash from the and of the given path, if it exists
	static std::string removeTrailingSlash(const std::string& path);

	///Returns the user's appdata/local/ folder
	static std::string getLocalAppDataPath();


	/////////////////////////////////
	//  Number/String Conversions  //
	/////////////////////////////////

	///Get a boolean value 'bVal' from the given string 's'
	///Function returns false if the string could not be converted
	static bool toBool(const std::string& s, bool& bVal);

	///Get a time string formatted as desired given a time in milliseconds
	static std::string getTimeString(uint64_t timeMS, TimeStringFormats format = TimeStringFormats::DHHMMSS);

	///Get a bandwidth string formatted as desired given a bandwidth in bytes
	static std::string getBandwidthString(uint64_t bytes, BandwidthStringFormats format = BandwidthStringFormats::rateAdaptive);

	///Covert value 'val' to a string
	template<typename T>
	inline static std::string toString(const T& val) {
		std::ostringstream o;
		o << val;
		return o.str();
	}

	///Covert value 'val' to a string with given precision
	template<typename T>
	static std::string toString(const T& val, int precision, int width = 0, char fillChar = '0') {
		std::ostringstream o;
		if (width > 0) {
			if (precision > 0) {
				o << std::setfill(fillChar) << std::setw(width) << std::fixed
					<< std::setprecision(precision) << val;
			}
			else {
				o << std::setfill(fillChar) << setw(width) << val;
			}
		}
		else {
			if (precision > 0) {
				o << std::fixed << std::setprecision(precision) << val;
			}
			else {
				o << int(val);
			}

		}

		return o.str();
	}

	///Covert string 'str' to a string with given width (right aligned)
	static std::string fillStringR(const std::string& str, int width, char fillChar = ' ');

	static std::string fillStringR(const std::stringstream& ss, int width, char fillChar = ' ');

	static std::string fillStringR(const char* str, int width, char fillChar = ' ');

	///Convert string 's' to a value of type 'T'.  Returns false if failed.
	template< typename T >
	static bool toNumber(const std::string& s, T& val)
	{
		std::istringstream iss(s);

		iss >> std::ws >> val >> std::ws;

		if (!iss.eof())
			return false;

		return true;
	}

	//TODO: Test which method is faster
	template< typename T >
	inline static bool toNumber2(const std::string& s, T& val)
	{
		std::istringstream iss(s);
		char c;
		if (!(iss >> val) || (iss.get(c)))
			return false;

		return true;
	}

	//static std::wstring s2ws(const std::string& s)
	//{
	//	int len;
	//	int slength = (int)s.length() + 1;
	//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	//	wchar_t* buf = new wchar_t[len];
	//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	//	std::wstring r(buf);
	//	delete[] buf;
	//	return r;
	//}
};


} //namespace otCore

#endif //StringUtility_H