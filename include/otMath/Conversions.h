/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Conversion.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Conversion_H
#define Conversion_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef MATH_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif

#define PI 3.14159265358979323846264338327950288419716939937511

static const double DEG2RAD = PI / 180.0;
static const double RAD2DEG = 180.0 / PI;

enum class UnitTypes {
	DISTANCE = 0,
	SPEED,
	MASS,
	FORCE,
	AREA,
	PRESSURE,
};

////////////////////////
//DISTANCE CONVERSIONS//
////////////////////////

/// Distance Units
enum class DistanceUnits {
	M = 0,
	KM,
	FT,
	NM,
	SM,
	INCH,
	CM,
	MM,
	DISTANCE_UNITS,
};

/////////////////////
//SPEED CONVERSIONS//
/////////////////////

/// Speed Units
enum class SpeedUnits {
	MPS = 0,
	KMH,
	FPS,
	KTS,
	MPH,
	FPM,
	SPEED_UNITS,
};

///////////////////////////
//MASS/WEIGHT CONVERSIONS//
///////////////////////////

/// Mass/Weight Units
enum class MassUnits {
	KG = 0,
	LB,
	SLUG,
	MASS_UNITS,
};

/////////////////////
//FORCE CONVERSIONS//
/////////////////////

/// Force Units
enum class ForceUnits {
	N = 0,
	KN,
	LBF,
	KGF,
	FORCE_UNITS,
};

////////////////////
//AREA CONVERSIONS//
////////////////////

/// Area Units
enum class AreaUnits {
	M2 = 0,
	FT2,
	IN2,
	CM2,
	AREA_UNITS,
};

////////////////////////
//PRESSURE CONVERSIONS//
////////////////////////

/// Pressure Units
enum class PressureUnits {
	ATM = 0,
	BAR,
	INHG,
	MMHG,
	PA,
	PSI,
	PSF,
	KPCM2,
	PRESSURE_UNITS,
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Unit conversion functions

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

template < typename T >
T MATH_API convert(const T val, const DistanceUnits from, const DistanceUnits to);

template < typename T >
T MATH_API convert(const T val, const SpeedUnits from, const SpeedUnits to);

template < typename T >
T MATH_API convert(const T val, const MassUnits from, const MassUnits to);

template < typename T >
T MATH_API convert(const T val, const ForceUnits from, const ForceUnits to);

template < typename T >
T MATH_API convert(const T val, const AreaUnits from, const AreaUnits to);

template < typename T >
T MATH_API convert(const T val, const PressureUnits from, const PressureUnits to);

////////////////////////////////////////////////////////
//TEMPERATURE CONVERSIONS (ABSOLUTE, NOT DIFFERENTIAL)//
////////////////////////////////////////////////////////

//  *************************************************************************
/// @name Temperature Conversion functions.
// @{

template < typename T >
inline T CtoF(T tempC)
{
	return (9.0 / 5.0)*tempC + 32.0;
}

template < typename T >
inline T CtoK(T tempC)
{
	return tempC + 273.15;
}

template < typename T >
inline T CtoR(T tempC)
{
	//local tempR = ((9.0/5.0)*tempC + 32.0) + 459.67
	return (tempC + 273.15)*(9.0 / 5.0);
}

template < typename T >
inline T FtoC(T tempF)
{
	return (tempF - 32.0)*(5.0 / 9.0);
}

template < typename T >
inline T FtoK(T tempF)
{
	return (tempF - 32.0)*(5.0 / 9.0) + 273.15;
}

template < typename T >
inline T FtoR(T tempF)
{
	return tempF + 459.67;
}

template < typename T >
inline T KtoF(T tempK)
{
	//tempF = (tempK - 273.15)*(9.0/5.0) + 32
	return tempK*(9.0 / 5.0) - 459.67;
}

template < typename T >
inline T KtoC(T tempK)
{
	return tempK - 273.15;
}

template < typename T >
inline T KtoR(T tempK)
{
	return tempK*(9.0 / 5.0);
}

template < typename T >
inline T RtoF(T tempR)
{
	return tempR - 459.67;
}

template < typename T >
inline T RtoC(T tempR)
{
	return ((tempR - 459.67) - 32)*(5.0 / 9.0);
}

template < typename T >
inline T RtoK(T tempR)
{
	return tempR*(5.0 / 9.0);
}
//@}

////////////////////////////
//ACCELERATION CONVERSIONS//
////////////////////////////

//  *************************************************************************
/// @name Acceleration Conversion functions.
// @{

template < typename T >
inline T MPS2toFPS2(T mps2)
{
	return mps2*3.28083;
}

template < typename T >
inline T FPS2toMPS2(T fps2)
{
	return fps2 / 3.28083;
}

template < typename T >
inline T MPS2toG(T mps2)
{
	return mps2 / 9.80665;
}

template < typename T >
inline T FPS2toG(T fps2)
{
	return fps2 / 32.17417;
}
//@}

//  *************************************************************************
/// @name Pressure Conversion functions.
// @{


//@}

///////////////////////
//DENSITY CONVERSIONS//
///////////////////////

//  *************************************************************************
/// @name Density Conversion functions.
// @{

template < typename T >
inline T KGCMtoPCF(T kgcm)
{
	return kgcm*2.20462 / 3.28084 / 3.28084 / 3.28084;
}

template < typename T >
inline T PCFtoKGCM(T pcf)
{
	return pcf / 2.20462*3.28084*3.28084*3.28084;
}
//@}

//////////////////////
//VOLUME CONVERSIONS//
//////////////////////

//  *************************************************************************
/// @name Volume Conversion functions.
// @{

template < typename T >
inline T M3toFT3(T m3)
{
	return m3*3.28084*3.28084*3.28084;
}

template < typename T >
inline T M3toIN3(T m3)
{
	return m3*3.28084*3.28084*3.28084 * 12 * 12 * 12;
}

template < typename T >
inline T FT3toM3(T ft3)
{
	return ft3 / 3.28084 / 3.28084 / 3.28084;
}

template < typename T >
inline T FT3toIN3(T ft3)
{
	return ft3*12.0*12.0*12.0;
}

template < typename T >
inline T IN3toM3(T in3)
{
	return in3 / 12.0 / 12.0 / 12.0 / 3.28084 / 3.28084 / 3.28084;
}

template < typename T >
inline T IN3toFT3(T in3)
{
	return in3 / 12.0 / 12.0 / 12.0;
}
//@}

///////////////////////
//ANGULAR CONVERSIONS//
///////////////////////

//  *************************************************************************
/// @name Angle Conversion functions.
// @{

template < typename T >
inline T DEGtoRAD(T deg)
{
	return ((deg / 180.0)*PI);
}

template < typename T >
inline T RADtoDEG(T rad)
{
	return ((rad / PI)*180.0);
}
//@}

/////////////////////////
//FUEL RATE CONVERSIONS//
/////////////////////////

//  *************************************************************************
/// @name Fuel Rate Conversion functions.
// @{

template < typename T >
inline T PPHtoKGPS(T pph)
{
	return pph / 7936.641438656;
}

template < typename T >
inline T KGPStoPPH(T kgps)
{
	return kgps * 7936.641438656;
}
//@}

//////////////////////
//TORQUE CONVERSIONS//
//////////////////////

//  *************************************************************************
/// @name Torque Conversion functions.
// @{

template < typename T >
inline T NMtoINLB(T torque_nm)
{
	return torque_nm * 8.850745792;
}

template < typename T >
inline T INLBtoNM(T torque_inlb)
{
	return torque_inlb / 8.850745792;
}
//@}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#endif //Conversion_H