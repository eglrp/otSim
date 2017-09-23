/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       JSON.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef JSON_H
#define JSON_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include <string>
#include <vector>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

namespace otCore {

enum ResultType {
	//R_CHAR = 0,
	R_INT,
	R_UINT,
	R_INT64,
	R_UINT64,
	R_BOOL,
	R_FLOAT,
	R_DOUBLE,
	R_STRING,
	R_INTARRAY,
	R_NUMARRAY,
	R_STRARRAY,
	R_ARRARRAY,
	R_OPTARRAY,
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** JSON handling.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class CORE_API JSON {
public:
	/// Constructor
	JSON(void);
	/// Destructor
	~JSON(void);

	///Read the JSON file at the given file path
	bool readFile(const std::string &filePath);

	///Write the JSON file to the given file path
	bool writeFile(const std::string &filePath);

	///Returns true if any objects or comments were changed by any setValue()/setComment() functions.
	bool hasChanged() const;

	///Returns true if the given JSON object was found, false if it was not
	bool hasObject(const std::string &object);

	///Returns the object from the given object name
	///If not found, the return boolean will be false.
	bool getObject(const std::string& object, JSON* returnedObject);

	///Returns a string vector of object names available in the current object.
	///Vector size will be 0 if the current object is not a category.
	std::vector<std::string> getObjectList();

	///Returns a string vector of object names available in the given object.
	///Vector size will be 0 if given object is not found, or if the object is not a category.
	std::vector<std::string> getObjectList(const std::string& object);

	///Will get an object value if it exists, else it will set AND return a default value.
	///If the object does not exist, it will also write the provided comment, if given.
	template <typename T>
	T advancedGetValue(
		const std::string& object,
		T defaultValue,
		const std::string& comment = "")
	{
		T value;
		if (hasObject(object)) {
			value = getValue(object, defaultValue);
		}
		else {
			value = defaultValue;
			setValue(object, value);
			if (!comment.empty())
				setComment(object, comment);
		}
		return value;
	}

	///Will get an object value if it exists, else it will set AND return a default value.
	///If the object exists, the value will be checked for limits and will be set AND returned within those limits.
	///If the object does not exist, it will also write the provided comment, if given.
	///NOTE: If the default value is not within the limits, the value set and returned will be changed.
	template <typename T>
	T advancedGetValueWithLimits(
		const std::string& object,
		T minValue, T maxValue, T defaultValue,
		const std::string& comment = "")
	{
		T value;
		if (hasObject(object)) {
			value = getValue(object, defaultValue);
			if (value < minValue || value > maxValue) {
				clamp(value, minValue, maxValue);
				setValue(object, value);
			}
		}
		else {
			value = clampR(defaultValue, minValue, maxValue);
			setValue(object, value);
			if (!comment.empty())
				setComment(object, comment);
		}
		return value;
	}

	///Will get a string object value if it exists, else it will set AND return a default string.
	///If the object exists, the string value will be checked for string length limits and will truncate any longer string.
	///If the object does not exist, it will also write the provided comment, if given.
	///NOTE: If the default string length is not within the string length limit,
	///the string set and returned will be changed.
	std::string advancedGetValueString(
		const std::string& object,
		std::string defaultValue,
		size_t maxStringLength,
		const std::string& comment = "");

	///Will get a string object value if it exists, else it will set AND return a default string.
	///If the object exists, the string value will be checked for equivalency to any of the given possible string
	///values in the listValues string.  Possible values should be separated by a comma within the string.
	///If the object does not exist, it will also write the provided comment, if given.
	///NOTE: If the string value or default string is not one of the given listValues,
	///the string set and returned will be the first string value in listValues.
	std::string advancedGetValueStringWithList(
		const std::string& object,
		std::string defaultValue,
		const std::string& listValues,
		const std::string& comment = "");

	///Return the integer object value.
	///If not found, the default value is returned.
	int getValue(const std::string &object, int defaultValue);

	///Return the unsigned integer object value.
	///If not found, the default value is returned.
	unsigned int getValue(const std::string &object, unsigned int defaultValue);

	///Return the integer object value.
	///If not found, the default value is returned.
	long long getValue(const std::string &object, long long defaultValue);

	///Return the unsigned integer64 object value.
	///If not found, the default value is returned.
	unsigned long long getValue(const std::string &object, unsigned long long defaultValue);

	///Return the boolean object value.
	///If not found, the default value is returned.
	bool getValue(const std::string &object, bool defaultValue);

	///Return the float object value.
	///If not found, the default value is returned.
	float getValue(const std::string &object, float defaultValue);

	///Return the double object value.
	///If not found, the default value is returned.
	double getValue(const std::string &object, double defaultValue);

	///Return the string object value.
	///If not found, the default value is returned.
	std::string getValue(const std::string &object, std::string defaultValue);

	///Returns an array vector of integers if the object is found.
	///If not found, the vector will be empty.
	std::vector<int> getValueIntegerArray(const std::string &object);

	///Returns an array vector of doubles if the object is found.
	///If not found, the vector will be empty.
	std::vector<double> getValueNumericArray(const std::string &object);

	///Returns an array vector of strings if the object is found.
	///If not found, the vector will be empty.
	std::vector<std::string> getValueStringArray(const std::string &object);

	///Returns an array vector of arrays of doubles if the object is found.
	///If not found, the vector will be empty.
	std::vector<std::vector<double>> getValueArrayNumericArray(const std::string &object);

	///Returns an array vector of JSON object pointers if the object is found.
	///If not found, the vector will be empty.
	///IMPORTANT: Caller must free memory for JSON objects when finished with processing
	std::vector<JSON*> getObjectArray(const std::string &object);


	bool setValue(const std::string &object, int value);

	bool setValue(const std::string &object, unsigned int value);

	bool setValue(const std::string &object, long long value);

	bool setValue(const std::string &object, unsigned long long value);

	bool setValue(const std::string &object, bool value);

	bool setValue(const std::string &object, float value);

	bool setValue(const std::string &object, double value);

	bool setValue(const std::string &object, const std::string& value);

	bool setValue(const std::string& object, const std::vector<int>& vector);

	bool setValue(const std::string& object, const std::vector<double>& vector);

	bool setValue(const std::string& object, const std::vector<std::string>& vector);

	///Removes an object permanently. Returns true if successful, false if object did not exist.
	bool removeObject(const std::string& object);

	///Set the comment string on the given object.
	///Returns true if successful in finding the object, otherwise false.
	bool setComment(const std::string &object, const std::string& comment);

	void debugPrint();

private:
	// Make this object be noncopyable
	JSON(const JSON& json);
	const JSON &operator =(const JSON &);

	// Pointer to implementation
	class Impl;
	Impl* mImpl;
};

} //namespace otCore

#endif //JSON_H