/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       GUID.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Allows parsing of GUIDs.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "GUID.h"
#include <ObjBase.h>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otCore {

std::string GUIDToString(const GUID &guid) {
	//OLECHAR* bstrCLSID;
	wchar_t* guid_;
	HRESULT hr = StringFromCLSID(guid, &guid_);
	if (hr != S_OK) {
		// bad GUID ...
		return "{ERRORERR-ERRO-ERRO-ERRO-ERRORERROR}";
	}
	std::wstring guid_wstr(guid_);
	std::string output(guid_wstr.begin(), guid_wstr.end());
	return output;
}

GUID stringToGUID(const std::string& guid_str) {
	GUID output;
	std::wstring guid_wstr = std::wstring(guid_str.begin(), guid_str.end());
	const wchar_t* guid_ = guid_wstr.c_str();
	HRESULT hr = CLSIDFromString(guid_, &output);
	if (hr != S_OK) {
		// bad GUID string...
		return GUID_NULL;
	}
	return output;
}

} //namespace otCore

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%