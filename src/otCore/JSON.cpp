/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       JSON.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
JSON handling.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "JSON.h"
#include <fstream>
#include <iostream>
//#include "Strings.h"
//#include "Paths.h"

#include "json\json.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

class JSON::Impl
{
public:
	Impl()
	{
		jObjectRoot.clear();
	}

	//Utility functions
	static void tokenizeString(const std::string &s, const char token, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, token)) {
			elems.push_back(item);
		}
	}
	static Json::Value* getJsonValue(const std::string &object, Json::Value *root, bool &successful);
	static bool removeJsonValue(const std::string &object, Json::Value *root);


	//Members
	Json::Value jObjectRoot;

	bool getValue(const std::string &object);
	bool setValue(const std::string &object);

	bool initialized = false;
	bool changed = false;

	ResultType resultType = R_INT;

	int iResult = 0;
	unsigned int uiResult = 0;
	long long i64Result = 0;
	unsigned long long ui64Result = 0;

	bool bResult = 0;
	float fResult = 0;
	double dResult = 0;
	std::string sResult = "";
	std::vector<int> iaResult;
	std::vector<double> naResult;
	std::vector<std::string> saResult;
	std::vector<std::vector<double>> aaResult;
	std::vector<JSON*> oaResult;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

JSON::JSON(void) : mImpl(new JSON::Impl())
{
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

JSON::~JSON(void)
{
	delete mImpl;
	mImpl = 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::readFile(const std::string &filePath)
{
	std::ifstream jsontest(filePath, std::ifstream::binary);

	Json::Features features;
	features.allowDroppedNullPlaceholders_ = true;
	Json::Reader reader(features);

	if (!mImpl->jObjectRoot.empty())
		mImpl->jObjectRoot.clear();

	bool success = reader.parse(jsontest, mImpl->jObjectRoot, true);

	if (!success)
	{
		// report to the user the failure and their locations in the document.
		// TODO: ERROR message
		//LOGGER->Warning(reader.getFormattedErrorMessages().c_str());
		std::string error = reader.getFormattedErrorMessages().c_str();
	}

	mImpl->initialized = success;
	mImpl->changed = false;
	return success;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::writeFile(const std::string &filePath)
{
	if (mImpl->initialized) {
		Json::StyledWriter writer;

		std::string output = writer.write(mImpl->jObjectRoot);

		std::ofstream ofile;
		ofile.open(filePath, std::ios::out | std::ios::trunc);
		if (ofile.is_open()) {
			ofile << output;
			ofile.close();
			mImpl->changed = false;
			return true;
		}
	}
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int JSON::getValue(const std::string &object, int defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_INT)
				return mImpl->iResult;
			else if (mImpl->resultType == R_UINT)
				return int(mImpl->uiResult);
			else if (mImpl->resultType == R_INT64)
				return int(mImpl->i64Result);
			else if (mImpl->resultType == R_UINT64)
				return int(mImpl->ui64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned int JSON::getValue(const std::string &object, unsigned int defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_UINT)
				return mImpl->uiResult;
			else if (mImpl->resultType == R_INT)
				return uint32_t(mImpl->iResult);
			else if (mImpl->resultType == R_INT64)
				return uint32_t(mImpl->i64Result);
			else if (mImpl->resultType == R_UINT64)
				return uint32_t(mImpl->ui64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

long long JSON::getValue(const std::string &object, long long defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_INT64)
				return mImpl->i64Result;
			else if (mImpl->resultType == R_INT)
				return int64_t(mImpl->iResult);
			else if (mImpl->resultType == R_UINT)
				return int64_t(mImpl->uiResult);
			else if (mImpl->resultType == R_UINT64)
				return int64_t(mImpl->ui64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

unsigned long long JSON::getValue(const std::string &object, unsigned long long defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_UINT64)
				return mImpl->ui64Result;
			else if (mImpl->resultType == R_INT)
				return uint64_t(mImpl->iResult);
			else if (mImpl->resultType == R_UINT)
				return uint64_t(mImpl->uiResult);
			else if (mImpl->resultType == R_INT64)
				return uint64_t(mImpl->i64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::getValue(const std::string &object, bool defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_BOOL)
				return mImpl->bResult;
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

float JSON::getValue(const std::string &object, float defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_FLOAT)
				return mImpl->fResult;
			else if (mImpl->resultType == R_DOUBLE)
				return float(mImpl->dResult);
			else if (mImpl->resultType == R_INT)
				return float(mImpl->iResult);
			else if (mImpl->resultType == R_UINT)
				return float(mImpl->uiResult);
			else if (mImpl->resultType == R_INT64)
				return float(mImpl->i64Result);
			else if (mImpl->resultType == R_UINT64)
				return float(mImpl->ui64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

double JSON::getValue(const std::string &object, double defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_DOUBLE)
				return mImpl->dResult;
			else if (mImpl->resultType == R_FLOAT)
				return mImpl->fResult;
			else if (mImpl->resultType == R_INT)
				return double(mImpl->iResult);
			else if (mImpl->resultType == R_UINT)
				return double(mImpl->uiResult);
			else if (mImpl->resultType == R_INT64)
				return double(mImpl->i64Result);
			else if (mImpl->resultType == R_UINT64)
				return double(mImpl->ui64Result);
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string JSON::getValue(const std::string &object, std::string defaultValue)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_STRING)
				return mImpl->sResult;
		}
	}
	return defaultValue;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<int> JSON::getValueIntegerArray(const std::string &object)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_INTARRAY)
				return mImpl->iaResult;
			else if (mImpl->resultType == R_NUMARRAY)
			{
				std::vector<int> returnArray;
				for (unsigned int i = 0; i < mImpl->naResult.size(); i++)
				{
					returnArray.push_back((int)mImpl->naResult.at(i));
				}

				return returnArray;
			}
		}
	}
	return mImpl->iaResult;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<double> JSON::getValueNumericArray(const std::string &object)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_NUMARRAY)
				return mImpl->naResult;
			else if (mImpl->resultType == R_INTARRAY)
			{
				std::vector<double> returnArray;
				for (unsigned int i = 0; i < mImpl->iaResult.size(); i++)
				{
					returnArray.push_back((double)mImpl->iaResult.at(i));
				}

				return returnArray;
			}
		}
	}
	return mImpl->naResult;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> JSON::getValueStringArray(const std::string &object)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_STRARRAY)
				return mImpl->saResult;
		}
	}
	return mImpl->saResult;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<JSON*> JSON::getObjectArray(const std::string &object)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_OPTARRAY)
				return mImpl->oaResult;
		}
	}
	return mImpl->oaResult;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::vector<double>> JSON::getValueArrayNumericArray(const std::string &object)
{
	if (mImpl->initialized) {
		if (mImpl->getValue(object)) {
			if (mImpl->resultType == R_ARRARRAY)
				return mImpl->aaResult;
		}
	}
	return mImpl->aaResult;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string JSON::advancedGetValueString(
	const std::string& object,
	std::string defaultValue,
	size_t maxStringLength,
	const std::string& comment)
{
	std::string value;
	if (hasObject(object)) {
		value = getValue(object, defaultValue);
		if (value.length() > maxStringLength) {
			value.substr(0, maxStringLength);
			setValue(object, value);
		}
	}
	else {
		value = defaultValue;
		if (value.length() > maxStringLength) {
			value.substr(0, maxStringLength);
		}
		setValue(object, value);
		if (!comment.empty())
			setComment(object, comment);
	}
	return value;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::string JSON::advancedGetValueStringWithList(
	const std::string& object,
	std::string defaultValue,
	const std::string& listValues,
	const std::string& comment)
{
	std::vector<std::string> vec;
	mImpl->tokenizeString(listValues, ',', vec);

	if (vec.empty()) //if empty given list, just use other string get function
		return advancedGetValueString(object, defaultValue, 5000, comment);

	std::string value;
	if (hasObject(object)) {
		value = getValue(object, defaultValue);
		bool matches = false;
		for (unsigned int i = 0; i < vec.size(); i++) {
			if (value == vec.at(i)) {
				matches = true;
				break;
			}
		}
		if (!matches) {
			value = vec.at(0);
			setValue(object, value);
		}
	}
	else {
		value = defaultValue;
		bool matches = false;
		for (unsigned int i = 0; i < vec.size(); i++) {
			if (value == vec.at(i)) {
				matches = true;
				break;
			}
		}
		if (!matches) {
			value = vec.at(0);
		}
		setValue(object, value);
		if (!comment.empty())
			setComment(object, comment);
	}
	return value;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, int value)
{
	mImpl->resultType = R_INT;
	mImpl->iResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, unsigned int value)
{
	mImpl->resultType = R_UINT;
	mImpl->uiResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, long long value)
{
	mImpl->resultType = R_INT64;
	mImpl->i64Result = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, unsigned long long value)
{
	mImpl->resultType = R_UINT64;
	mImpl->ui64Result = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, bool value)
{
	mImpl->resultType = R_BOOL;
	mImpl->bResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, float value)
{
	mImpl->resultType = R_FLOAT;
	mImpl->fResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, double value)
{
	mImpl->resultType = R_DOUBLE;
	mImpl->dResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string &object, const std::string& value)
{
	mImpl->resultType = R_STRING;
	mImpl->sResult = value;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string& object, const std::vector<int>& vector)
{
	mImpl->resultType = R_INTARRAY;
	mImpl->iaResult = vector;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string& object, const std::vector<double>& vector)
{
	mImpl->resultType = R_NUMARRAY;
	mImpl->naResult = vector;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setValue(const std::string& object, const std::vector<std::string>& vector)
{
	mImpl->resultType = R_STRARRAY;
	mImpl->saResult = vector;
	return mImpl->setValue(object);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::Impl::setValue(const std::string &object)
{
	std::vector<std::string> vec;
	tokenizeString(object, '.', vec);

	Json::Value value;

	switch (resultType) {
		//case R_CHAR:
		//	value = cResult;
		//	break;
	case R_INT:
		value = iResult;
		break;
	case R_UINT:
		value = uiResult;
		break;
	case R_INT64:
		value = i64Result;
		break;
	case R_UINT64:
		value = ui64Result;
		break;
	case R_BOOL:
		value = bResult;
		break;
	case R_FLOAT:
		value = fResult;
		break;
	case R_DOUBLE:
		value = dResult;
		break;
	case R_STRING:
		value = sResult;
		break;
	case R_INTARRAY:
		for (unsigned int i = 0; i < iaResult.size(); i++) {
			value[i] = iaResult.at(i);
		}
		break;
	case R_NUMARRAY:
		for (unsigned int i = 0; i < naResult.size(); i++) {
			value[i] = naResult.at(i);
		}
		break;
	case R_STRARRAY:
		for (unsigned int i = 0; i < saResult.size(); i++) {
			value[i] = saResult.at(i);
		}
		break;
	default:
		return false;
	}

	//only support 10 levels deep
	if (vec.size() == 1)
		jObjectRoot[vec.at(0)] = value;
	else if (vec.size() == 2)
		jObjectRoot[vec.at(0)][vec.at(1)] = value;
	else if (vec.size() == 3)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)] = value;
	else if (vec.size() == 4)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)] = value;
	else if (vec.size() == 5)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)] = value;
	else if (vec.size() == 6)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)][vec.at(5)] = value;
	else if (vec.size() == 7)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)][vec.at(5)][vec.at(6)] = value;
	else if (vec.size() == 8)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)][vec.at(5)][vec.at(6)][vec.at(7)] = value;
	else if (vec.size() == 9)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)][vec.at(5)][vec.at(6)][vec.at(7)][vec.at(8)] = value;
	else if (vec.size() == 10)
		jObjectRoot[vec.at(0)][vec.at(1)][vec.at(2)][vec.at(3)][vec.at(4)][vec.at(5)][vec.at(6)][vec.at(7)][vec.at(8)][vec.at(9)] = value;
	else
		return false;

	if (!initialized) initialized = true;

	changed = true;
	return true;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Json::Value* JSON::Impl::getJsonValue(const std::string &object, Json::Value *root, bool &successful)
{
	std::vector<std::string> optionsPathVec;

	tokenizeString(object, '.', optionsPathVec);

	successful = false;

	Json::Value* value = root;

	for (unsigned int i = 0; i<optionsPathVec.size(); i++)
	{
		//get next sub-category
		if (value->isMember(optionsPathVec.at(i))) {
			value = &(*value)[optionsPathVec.at(i)];
		}
		else
			return nullptr;
	}

	successful = true;
	return value;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::Impl::removeJsonValue(const std::string &object, Json::Value *root)
{
	std::vector<std::string> optionsPathVec;

	tokenizeString(object, '.', optionsPathVec);

	Json::Value* value = root;

	for (unsigned int i = 0; i < optionsPathVec.size(); i++)
	{
		if (i != (optionsPathVec.size() - 1)) {
			//not the last object token, get next sub-category
			if (value->isMember(optionsPathVec.at(i))) {
				value = &(*value)[optionsPathVec.at(i)];
			}
			else
				return false;
		}
		else {
			//last object token, remove here
			if (value->isMember(optionsPathVec.at(i))) {
				Json::Value oldValue = value->removeMember(optionsPathVec.at(i));
				if (!oldValue.isNull())
					return true;
				break;
			}
			else
				return false;
		}
	}

	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::removeObject(const std::string& object)
{
	if (mImpl->removeJsonValue(object, &mImpl->jObjectRoot)) {
		mImpl->changed = true;
		return true;
	}

	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::hasObject(const std::string &object)
{
	bool successful;
	Json::Value *optionValue = mImpl->getJsonValue(object, &mImpl->jObjectRoot, successful);
	return successful;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::hasChanged() const
{
	return mImpl->changed;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> JSON::getObjectList()
{
	return mImpl->jObjectRoot.getMemberNames();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

std::vector<std::string> JSON::getObjectList(const std::string& object)
{
	std::vector<std::string> optionList;
	bool successful;
	Json::Value *optionValue = mImpl->getJsonValue(object, &mImpl->jObjectRoot, successful);
	if (successful)
		optionList = optionValue->getMemberNames();
	return optionList;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::setComment(const std::string &object, const std::string& comment)
{
	bool successful;
	Json::Value *optionValue = mImpl->getJsonValue(object, &mImpl->jObjectRoot, successful);
	if (successful) {
		optionValue->setComment("//" + comment, Json::commentAfterOnSameLine);
		mImpl->changed = true;
		return true;
	}

	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::getObject(const std::string& object, JSON* returnedObject)
{
	if (mImpl->initialized) {
		bool successful;

		Json::Value *result = mImpl->getJsonValue(object, &mImpl->jObjectRoot, successful);
		if (successful)
		{
			if (result->isObject()) {
				returnedObject->mImpl->jObjectRoot = (*result);
				returnedObject->mImpl->initialized = true;
				return true;
			}
		}
	}
	return false;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool JSON::Impl::getValue(const std::string &object)
{
	iaResult.clear();
	naResult.clear();
	saResult.clear();
	aaResult.clear();

	for (unsigned int i = 0; i<oaResult.size(); i++)
	{
		delete oaResult.at(i);
	}
	oaResult.clear();

	if (initialized)
	{
		bool successful;

		Json::Value *result = getJsonValue(object, &jObjectRoot, successful);
		if (successful)
		{
			if (result->isInt())
				resultType = R_INT;
			else if (result->isUInt())
				resultType = R_UINT;
			else if (result->isInt64())
				resultType = R_INT64;
			else if (result->isUInt64())
				resultType = R_UINT64;
			else if (result->isBool())
				resultType = R_BOOL;
			else if (result->isDouble())
				resultType = R_DOUBLE;
			else if (result->isString())
				resultType = R_STRING;
			else if (result->isNumeric())
				resultType = R_FLOAT;
			else if (result->isArray())
			{
				bool isInteger = true;
				bool isNumeric = true;
				bool isString = true;
				bool isArray = true;
				bool isObject = true;
				for (unsigned int i = 0; i<result->size(); i++)
				{
					//for(Json::Value::iterator it = result->begin(); it !=result->end(); ++it)
					//{
					//check if last item and is null
					//if ( result->end() - it == 1 && (*it).isNull())
					//	break;
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;

					if (!(*result)[i].isInt())
						isInteger = false;

					if (!(*result)[i].isNumeric())
						isNumeric = false;

					if (!(*result)[i].isString())
						isString = false;

					if (!(*result)[i].isArray())
						isArray = false;

					if (!(*result)[i].isObject())
						isObject = false;

					if (!isObject && !isInteger && !isNumeric && !isString && !isArray)
						return false;
				}
				if (isInteger)
					resultType = R_INTARRAY;
				else if (isNumeric)
					resultType = R_NUMARRAY;
				else if (isString)
					resultType = R_STRARRAY;
				else if (isArray)
					resultType = R_ARRARRAY;
				else if (isObject)
					resultType = R_OPTARRAY;
			}
			else
				return false;

			switch (resultType)
			{
				//case R_CHAR:
				//	cResult = result->asCString();
				//	return true;
			case R_INT:
				iResult = result->asInt();
				return true;
			case R_UINT:
				uiResult = result->asUInt();
				return true;
			case R_INT64:
				i64Result = result->asInt64();
				return true;
			case R_UINT64:
				ui64Result = result->asUInt64();
				return true;
			case R_BOOL:
				bResult = result->asBool();
				return true;
			case R_FLOAT:
				fResult = result->asFloat();
				return true;
			case R_DOUBLE:
				dResult = result->asDouble();
				return true;
			case R_STRING:
				sResult = result->asString();
				return true;
			case R_INTARRAY:
			{
				for (unsigned int i = 0; i<result->size(); i++) {
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;
					iaResult.push_back((*result)[i].asInt());
				}
				return true;
			}
			case R_NUMARRAY:
			{
				for (unsigned int i = 0; i<result->size(); i++) {
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;
					naResult.push_back((*result)[i].asDouble());
				}
				return true;
			}
			case R_STRARRAY:
			{
				for (unsigned int i = 0; i<result->size(); i++) {
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;
					saResult.push_back((*result)[i].asString());
				}
				return true;
			}
			case R_ARRARRAY:
			{
				for (unsigned int i = 0; i<result->size(); i++)
				{
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;

					std::vector<double> innerArray;
					Json::Value *result2 = &(*result)[i];
					for (unsigned int j = 0; j < result2->size(); j++) {
						if (j == result2->size() - 1 && (*result2)[j].isNull())
							break;

						if (!(*result2)[j].isNumeric())
							return false;

						innerArray.push_back((*result2)[j].asDouble());
					}

					aaResult.push_back(innerArray);
				}
				return true;
			}
			case R_OPTARRAY:
			{
				for (unsigned int i = 0; i<result->size(); i++)
				{
					if (i == result->size() - 1 && (*result)[i].isNull())
						break;

					JSON* newObject = new JSON();

					newObject->mImpl->jObjectRoot = (*result)[i];
					newObject->mImpl->initialized = true;
					oaResult.push_back(newObject);
				}
				return true;
			}
			default:
				break;
			}
		}
	}

	return false;
}

void JSON::debugPrint()
{
	if (mImpl->initialized) {
		Json::StyledWriter writer;

		std::string output = writer.write(mImpl->jObjectRoot);
		// TODO: Output message
		//LOGGER->Info(output);
	}
}

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%