/**
\file    GNSS_OptionFile.h
\brief   The option file for EssentialGNSS.

\author  Glenn D. MacGougan (GDM)
\date    2007-11-29
\since   2006-12-30

\b "LICENSE INFORMATION" \n
Copyright (c) 2007, refer to 'author' doxygen tags \n
All rights reserved. \n

Redistribution and use in source and binary forms, with or without
modification, are permitted provided the following conditions are met: \n

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer. \n
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution. \n
- The name(s) of the contributor(s) may not be used to endorse or promote
products derived from this software without specific prior written
permission. \n

THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

#ifndef _EGNSS_OPTIONFILE_H_
#define _EGNSS_OPTIONFILE_H_

#include <string>
#include "OptionFile.h"
#include "gnss_types.h"
#include "GNSS_RxData.h"

namespace GNSS
{
/// \brief   A derived option file class for the program.
class GNSS_OptionFile : public OptionFile
{
public:

	/// \brief    The default constructor (no data allocated yet).
	GNSS_OptionFile();

	/// \brief    The destructor.
	virtual ~GNSS_OptionFile();

private:

	/// \brief   The copy constructor. Disabled!
	GNSS_OptionFile(const GNSS_OptionFile& rhs) {};

	/// \brief   The assignment operator. Disabled!
	void operator=(const GNSS_OptionFile& rhs) {};

public:

	/// \brief    Read and interpret the option file.
	/// \return   true if successful, false if error.
	bool ReadAndInterpretOptions(std::string OptionFilePath);

public:

	struct stStationInformation
	{
		bool   isValid; //!< Is this information valid.

		std::string DataTypeStr;       //!< The data type string.
		GNSS_enumRxDataType DataType;  //!< The data type as an enumeration.

		double stdev_GPSL1_psr;        //!< default GPSL1 pseudorange measurement standard deviation [m]
		double stdev_GPSL1_doppler;    //!< default GPSL1 Doppler measurement standard deviation [Hz]
		double stdev_GPSL1_adr;        //!< default GPSL1 accumulated Doppler range measurement standard deviation [cycles]

		double latitudeRads;     //!< Station Latitude [rad].
		double latitudeDegrees;  //!< Station Latitude [deg].
		double longitudeRads;    //!< Station Longitude [rad].
		double longitudeDegrees; //!< Station Longitude [degrees].
		double height;           //!< Station Height [m]

		double x; //!< Station ECEF WGS84 X coordinate [m].
		double y; //!< Station ECEF WGS84 Y coordinate [m].
		double z; //!< Station ECEF WGS84 Z coordinate [m].

		bool useTropo; //!< A boolean to indicate if the tropospheric correction is enabled.
		bool useIono;  //!< A boolean to indicate if the broadcast ionospheric correction is enabled.

		double uncertaintyLatitudeOneSigma;  //!< Initial position uncertainty [m].
		double uncertaintyLongitudeOneSigma; //!< Initial position uncertainty [m].
		double uncertaintyHeightOneSigma;    //!< Initial position uncertainty [m].

		int satsToExclude[64];    //!< An array of satellite id (PRN for GPS) to exclude from processing.
		unsigned nrSatsToExclude; //!< The number of satellite id's specified in satsToExclude.

		std::string DataPath;     //!< The path to the datafile.

								  // default constructor
		stStationInformation()
			: isValid(false),
			stdev_GPSL1_psr(0.8),
			stdev_GPSL1_doppler(0.09),
			stdev_GPSL1_adr(0.03),
			latitudeRads(0.0),
			latitudeDegrees(0.0),
			longitudeRads(0.0),
			longitudeDegrees(0.0),
			height(0.0),
			useTropo(true),
			useIono(true),
			uncertaintyLatitudeOneSigma(1.0),
			uncertaintyLongitudeOneSigma(1.0),
			uncertaintyHeightOneSigma(1.0),
			nrSatsToExclude(0)
		{
			unsigned i;
			for (i = 0; i < 64; i++)
				satsToExclude[i] = 0;
		}
	};



	struct stGPSTime
	{
		int GPSWeek;
		double GPSTimeOfWeek;

		// default constructor
		stGPSTime()
			: GPSWeek(-1), GPSTimeOfWeek(0.0)
		{}
	};

	struct stKalmanOptions
	{
		double RTK4_sigmaNorth;
		double RTK4_sigmaEast;
		double RTK4_sigmaUp;
		double RTK4_sigmaClock;

		double alphaVn;
		double alphaVe;
		double alphaVup;
		double alphaClkDrift;
		double sigmaVn;
		double sigmaVe;
		double sigmaVup;
		double sigmaClkDrift;

		// default constructor
		stKalmanOptions()
			: RTK4_sigmaNorth(0.5),
			RTK4_sigmaEast(0.5),
			RTK4_sigmaUp(0.5),
			RTK4_sigmaClock(100.0),
			alphaVn(100.0),
			alphaVe(100.0),
			alphaVup(100.0),
			alphaClkDrift(100.0),
			sigmaVn(0.01),
			sigmaVe(0.01),
			sigmaVup(0.01),
			sigmaClkDrift(0.01)
		{}
	};

	struct stRoverDatum
	{
		bool isValid;
		double latitudeRads;
		double longitudeRads;
		double height;

		stRoverDatum()
			:isValid(false), latitudeRads(0), longitudeRads(0), height(0)
		{}
	};

	/// The Kalman filtering options.
	stKalmanOptions m_KalmanOptions;

	/// The path to the option file.
	std::string m_OptionFilePath;

	/// The path to the output file.
	std::string m_OutputFilePath;

	/// A string is used to indicate the processing method.
	std::string m_ProcessingMethod;

	/// A boolean to indicate if only single difference measurements
	/// between the reference and rover station will be used.
	bool m_processDGPSOnly;

	//!< The path to the navigation datafile (ephemeris), if applicable.
	std::string m_RINEXNavDataPath;

	/// The reference station information.
	stStationInformation m_Reference;

	/// The rover station information.
	stStationInformation m_Rover;

	/// A datum for the rover for which position difference calculations are performed.
	stRoverDatum m_RoverDatum;

	/// A boolean to indicate if the rover is static or dynamic.
	bool m_RoverIsStatic;

	/// The start time for processing.
	stGPSTime m_StartTime;

	/// The end time for processing.
	stGPSTime m_EndTime;

	/// A boolean to indicate if Doppler measurements should be used at all.
	bool m_UseDopplerMeasurements;

	/// The klobuchar ionospheric parameters.
	GNSS_structKlobuchar m_klobuchar;

	/// The elevation mask [degrees].
	double m_elevationMask;

	/// The C/N0 mask [dB-Hz].
	double m_cnoMask;

	/// The locktime mask [s].
	double m_locktimeMask;

	/// A boolean to indicate if the rover position is to be fixed.
	bool m_isPositionFixed;

	/// A boolean to indicate if the rover height is to be constrained.
	bool m_isHeightConstrained;

#ifdef GDM_UWB_RANGE_HACK    

	struct stUWB
	{
		int id;   //!< Station ID (-1 is invalid)
		double x; //!< Station ECEF WGS84 X coordinate [m].
		double y; //!< Station ECEF WGS84 Y coordinate [m].
		double z; //!< Station ECEF WGS84 Z coordinate [m].

		stUWB() : id(-1), x(0), y(0), z(0) {}
	};

	stUWB m_UWB_a;
	stUWB m_UWB_b;
	stUWB m_UWB_c;

	/// The path to an UWB range data file.
	/// containing:
	/// column 0  = gps time of week (s)
	/// column 1  = gps week (weeks)
	/// column 14 = range (feet) * 100
	std::string m_UWBFilePath;
#endif

};

} // end namespace Zenautics

#endif // _EGNSS_OPTIONFILE_H_
