/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       Conversions.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Functions for unit conversions

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Conversions.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

////////////////////////
//DISTANCE CONVERSIONS//
////////////////////////

static const double dist_conv[static_cast<int>(DistanceUnits::DISTANCE_UNITS)][static_cast<int>(DistanceUnits::DISTANCE_UNITS)] =
{
	//M to other
	{ 1.0,						1.0 / 1000.0,				1.0 / 0.3048,				1.0 / 1852.0,					1.0 / 0.3048 / 5280.0,			100.0 / 2.54,		100.0,					1000.0 },
	//KM to other
	{ 1000.0,					1.0,						1000.0 / 0.3048,			1000.0 / 1852.0,				1000.0 / 0.3048 / 5280.0,		100000.0 / 2.54,	100000.0,				1000000.0 },
	//FT to other
	{ 0.3048,					0.3048 / 1000.0,			1.0,						1.0 / 6076.131,					1.0 / 5280.0,					12.0,				12.0*2.54,				12.0*25.4 },
	//NM to other
	{ 1852.0,					1852.0 / 1000.0,			6076.131,					1.0,							6076.131 / 5280.0,				6076.131*12.0,		6076.131*12.0*2.54,		6076.131*12.0*25.4 },
	//SM to other
	{ 5280.0*0.3048,			5280.0*0.3048 / 1000.0,		5280.0,						5280.0 / 6076.131,				1.0,							5280.0*12.0,		5280.0*12.0*2.54,		5280.0*12.0*25.4 },
	//INCH to other
	{ 2.54 / 100.0,				2.54 / 100000.0,			12.0,						1.0 / 12.0 / 6076.131,			1.0 / 12.0 / 5280.0,			1.0,				2.54,					25.4 },
	//CM to other
	{ 1.0 / 100.0,				1.0 / 100000.0,				1.0 / 2.54 / 12.0,			1.0 / 2.54 / 12.0 / 6076.131,	1.0 / 2.54 / 12.0 / 5280.0,		1.0 / 2.54,			1.0,					10.0 },
	//MM to other
	{ 1.0 / 1000.0,				1.0 / 1000000.0,			1.0 / 25.4 / 12.0,			1.0 / 25.5 / 12.0 / 6076.131,	1.0 / 2.54 / 12.0 / 5280.0,		1.0 / 25.54,		1.0 / 10.0,				1.0 }
};

/////////////////////
//SPEED CONVERSIONS//
/////////////////////

static const double speed_conv[static_cast<int>(SpeedUnits::SPEED_UNITS)][static_cast<int>(SpeedUnits::SPEED_UNITS)] =
{
	//MPS to other
	{ 1.0,						3.6,						1.0 / 0.3048,			3600.0 / 1852.0,		3600.0 / 0.3048 / 5280.0,	60.0 / 0.3048 },
	//KMH to other
	{ 1.0 / 3.6,				1.0,						1.0 / 0.3048 / 3.6,     1000.0 / 1852.0,		1000.0 / 0.3048 / 5280.0,	1000.0 / 0.3048 / 60.0 },
	//FPS to other
	{ 0.3048,					0.3048*3.6,					1.0,					3600.0 / 6076.131,		3600.0 / 5280.0,			60.0 },
	//KTS to other
	{ 1852.0 / 3600.0,			1852.0 / 1000.0,			6076.131 / 3600.0,      1.0,					6076.131 / 5280.0,			6076.131 / 60.0 },
	//MPH to other
	{ 5280.0*0.3048 / 3600.0,	5280.0*0.3048 / 1000.0,		5280.0 / 3600.0,        5280.0 / 6076.131,		1.0,						5280.0 / 60.0 },
	//FPM to other
	{ 0.3048 / 60.0,			60.0*0.3048 / 1000.0,		1.0 / 60.0,             60.0 / 6076.131,		60.0 / 5280.0,				1.0 }
};

///////////////////////////
//MASS/WEIGHT CONVERSIONS//
///////////////////////////

static const double mass_conv[static_cast<int>(MassUnits::MASS_UNITS)][static_cast<int>(MassUnits::MASS_UNITS)] =
{
	//KG to other
	{ 1.0,          1.0 / 0.45359237,			1.0 / 14.59390294 },
	//LB to other
	{ 0.45359237,   1.0,						0.45359237 / 14.59390294 },
	//SLUG to other
	{ 14.59390294,	14.59390294 / 0.45359237,	1.0 }
};

/////////////////////
//FORCE CONVERSIONS//
/////////////////////

static const double force_conv[static_cast<int>(ForceUnits::FORCE_UNITS)][static_cast<int>(ForceUnits::FORCE_UNITS)] =
{
	//N to other
	{ 1.0,                  1.0 / 1000.0,					1.0 / 9.80665 / 0.45359237,     1.0 / 9.80665 },
	//KN to other
	{ 1000.0,               1.0,							1000.0 / 9.80665 / 0.45359237,  1000.0 / 9.80665 },
	//LBF to other
	{ 0.45359237*9.80665,	0.45359237*9.80665 / 1000.0,	1.0,							0.45359237 },
	//KGF to other
	{ 9.80665,              9.80665 / 1000.0,				1.0 / 0.45359237,				1.0 }
};

////////////////////
//AREA CONVERSIONS//
////////////////////

static const double area_conv[static_cast<int>(AreaUnits::AREA_UNITS)][static_cast<int>(AreaUnits::AREA_UNITS)] =
{
	//M2 to other
	{ 1.0,							1.0 / 0.3048 / 0.3048,				100.0*100.0 / 2.54 / 2.54,  100.0*100.0 },
	//FT2 to other
	{ 0.3048*0.3048,				1.0,								12.0*12.0,					12.0*12.0*2.54*2.54 },
	//IN2 to other
	{ 2.54*2.54 / 100.0 / 100.0,	1.0 / 12.0 / 12.0,					1.0,						2.54*2.54 },
	//CM2 to other
	{ 1.0 / 100.0 / 100.0,			1.0 / 2.54 / 2.54 / 12.0 / 12.0,    1.0 / 2.54 / 2.54,          1.0 }
};

////////////////////////
//PRESSURE CONVERSIONS//
////////////////////////

static const double pressure_conv[static_cast<int>(PressureUnits::PRESSURE_UNITS)][static_cast<int>(PressureUnits::PRESSURE_UNITS)] =
{
	//ATM to other
	{ 1.0,							1.01325,						29.9213,						760.0,							101325.0,							14.69595,					14.69595*12.0*12.0,				1.033211 },
	//BAR to other
	{ 1.0 / 1.01325,				1.0,							29.9213 / 1.01325,				760.0 / 1.01325,				100000.0,							14.69595 / 1.01325,			14.69595*12.0*12.0 / 1.01325,	1.033211 / 1.01325 },
	//INHG to other
	{ 1.0 / 29.9213,				1.01325 / 29.9213,				1.0,							760.0 / 29.9213,				101325.0 / 29.9213,					14.69595 / 29.9213,			14.69595*12.0*12.0 / 29.9213,	1.033211 / 29.9213 },
	//MMHG to other
	{ 1.0 / 760.0,					1.01325 / 760.0,				29.9213 / 760.0,				1.0,							101325.0 / 760.0,					14.69595 / 760.0,			14.69595*12.0*12.0 / 760.0,		1.033211 / 760.0 },
	//PA to other
	{ 1.0 / 101325.0,				1.0 / 100000.0,					29.9213 / 101325.0,				760.0 / 101325.0,				1.0,								14.69595 / 101325.0,		14.69595*12.0*12.0 / 101325.0,	1.033211 / 101325.0 },
	//PSI to other
	{ 1.0 / 14.69595,				1.01325 / 14.69595,				29.9213 / 14.69595,				760.0 / 14.69595,				101325.0 / 14.69595,				1.0,						12.0*12.0,						1.033211 / 14.69595 },
	//PSF to other
	{ 1.0 / 12.0 / 12.0 / 14.69595,	1.01325 / 12.0 / 12.0 / 14.696,	29.9213 / 12.0 / 12.0 / 14.696,	760.0 / 12.0 / 12.0 / 14.696,	101325.0 / 12.0 / 12.0 / 14.696,	1.0 / 12.0 / 12.0,			1.0,							1.033211 / 12.0 / 12.0 / 14.69595 },
	//KPCM2 to other
	{ 1.0 / 1.033211,				1.01325 / 1.033211,				29.9213 / 1.033211,				760.0 / 1.033211,				101325.0 / 1.033211,				14.69595 / 1.033211,		14.69595*12.0*12.0 / 1.033211,	1.0 },
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
UNIT CONVERSION FUNCTIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

template < typename T >
T convert(const T val, const DistanceUnits from, const DistanceUnits to)
{
	return (T)(val * dist_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

template < typename T >
T convert(const T val, const SpeedUnits from, const SpeedUnits to)
{
	return (T)(val * speed_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

template < typename T >
T convert(const T val, const MassUnits from, const MassUnits to)
{
	return (T)(val * mass_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

template < typename T >
T convert(const T val, const ForceUnits from, const ForceUnits to)
{
	return (T)(val * force_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

template < typename T >
T convert(const T val, const AreaUnits from, const AreaUnits to)
{
	return (T)(val * area_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

template < typename T >
T convert(const T val, const PressureUnits from, const PressureUnits to)
{
	return (T)(val * pressure_conv[static_cast<int>(from)][static_cast<int>(to)]);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// Declare float and double types so the compiler can build these templates
template float MATH_API convert(const float val, const DistanceUnits from, const DistanceUnits to);
template float MATH_API convert(const float val, const SpeedUnits from, const SpeedUnits to);
template float MATH_API convert(const float val, const MassUnits from, const MassUnits to);
template float MATH_API convert(const float val, const ForceUnits from, const ForceUnits to);
template float MATH_API convert(const float val, const AreaUnits from, const AreaUnits to);
template float MATH_API convert(const float val, const PressureUnits from, const PressureUnits to);

template double MATH_API convert(const double val, const DistanceUnits from, const DistanceUnits to);
template double MATH_API convert(const double val, const SpeedUnits from, const SpeedUnits to);
template double MATH_API convert(const double val, const MassUnits from, const MassUnits to);
template double MATH_API convert(const double val, const ForceUnits from, const ForceUnits to);
template double MATH_API convert(const double val, const AreaUnits from, const AreaUnits to);
template double MATH_API convert(const double val, const PressureUnits from, const PressureUnits to);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%