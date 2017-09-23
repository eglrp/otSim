/**
\file    GNSS_Estimator.h
\brief   The header file for the GNSS_Estimator class.

\author  Glenn D. MacGougan (GDM)
\date    2007-10-29
\since   2006-11-16

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

#ifndef _GNSS_ESTIMATOR_H_
#define _GNSS_ESTIMATOR_H_

#include <stdio.h>
#include <list>
#include "gnss_types.h"
#include "Matrix.h"

using namespace Zenautics; // for Matrix
using namespace std;

namespace GNSS
{

// forwarding declaration
class GNSS_RxData;


/**
\brief   A class for estimating navigation information provided GNSS
measurements.

\author  Glenn D. MacGougan (GDM)
\date    2006-11-16
*/
class GNSS_Estimator
{
public:

	enum GNSS_FilterType
	{
		GNSS_FILTER_TYPE_INVALID = 0,
		GNSS_FILTER_TYPE_LSQ = 1,
		GNSS_FILTER_TYPE_EKF = 2,
		GNSS_FILTER_TYPE_RTK4 = 3,
		GNSS_FILTER_TYPE_RTK8 = 4,
		GNSS_FILTER_TYPE_RESERVED
	};

public:

	/// \brief    The default constructor (no data allocated yet).
	GNSS_Estimator();

	/// \brief    The destructor.
	virtual ~GNSS_Estimator();

private:

	/// \brief   The copy constructor. Disabled!
	GNSS_Estimator(const GNSS_Estimator& rhs);

	/// \brief   The assignment operator. Disabled!
	void operator=(const GNSS_Estimator& rhs)
	{}

public:

	bool Initialize(
		double latitudeRads,
		double longitudeRads,
		double height,
		double std_lat,
		double std_lon,
		double std_hgt
	);

	bool InitializeDPGS(
		const double referenceLatitudeRads,
		const double referenceLongitudeRads,
		const double refernceHeight,
		double latitudeRads,
		double longitudeRads,
		double height,
		double std_lat,
		double std_lon,
		double std_hgt
	);


	/**
	\brief  Deal with any millisecond clock jumps or any large arbitrary clock jumps
	detected in the receiver data. The jumps are compensated only by adjusting
	the estimate of the clock offset.

	\pre  The following must be determined before this function is called: \n
	rxData->m_msJumpDetected_Positive \n
	rxData->m_msJumpDetected_Negative \n
	rxData->m_clockJumpDetected \n
	rxData->m_clockJump \n
	rxBaseData->m_msJumpDetected_Positive \n
	rxBaseData->m_msJumpDetected_Negative \n
	rxBaseData->m_clockJumpDetected \n
	rxBaseData->m_clockJump \n

	\post  The clock offset state is adjusted if needed: \n
	rxData->m_pvt.clockOffset \n

	\return true if successful, false otherwise
	*/
	bool DealWithClockJumps(
		GNSS_RxData *rxData,      //!< A pointer to the rover receiver data. This must be a valid pointer.
		GNSS_RxData *rxBaseData   //!< A pointer to the reference receiver data if available. NULL if not available.
	);


	/// \brief  Compute the DOP values for this epoch.
	///
	/// \pre    The following must be valid:        \n
	/// rxData->m_ObsArray[i].flags.isPsrUsedInSolution \n
	/// \post   The following are set: either rxData->m_pvt or rxData->m_pvt_lsq\n
	/// dop.ndop \n
	/// dop.edop \n
	/// dop.vdop \n
	/// dop.tdop \n
	/// dop.hdop \n
	/// dop.pdop \n
	/// dop.gdop \n
	/// \return true if successful, false otherwise
	bool ComputeDOP(
		GNSS_RxData *rxData,       //!< The receiver data.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);



	/// \brief    Determine the satellite clock corrections, positions, and 
	///           velocities for the rover receiver and the reference
	///           receiver if aviailable (!NULL). Also determine if the 
	///           ephemeris is valid (not too old). If differential,
	///           rxBaseData != NULL, the reference station receiver 
	///           ephemeris data is used.
	///
	/// \pre      The following must be valid:        \n
	/// rxData.m_pvt or rxData.m_pvt_lsq must be a valid estimate.  \n
	/// rxData.m_maxAgeEphemeris must be set.         \n
	/// rxData.m_time must be a valid time.           \n           
	/// if differential                               \n
	///   rxBaseData.m_pvt must be a valid estimate.  \n
	///   rxBaseData.m_maxAgeEphemeris must be set.   \n
	///   rxBaseData.m_time must be a valid time.     \n               
	///
	/// \post     The following are set:                      \n
	/// rxData.m_ObsArray[i].flags.isEphemerisValid           \n
	/// rxData.m_ObsArray[i].satellite                        \n
	/// rxData.m_ObsArray[i].corrections.prcSatClk            \n
	/// rxData.m_ObsArray[i].corrections.rrcSatClkDrift       \n
	/// if differntial                                        \n
	///   rxBaseData.m_ObsArray[i].flags.isEphemerisValid     \n
	///   rxBaseData.m_ObsArray[i].satellite                  \n
	///   rxBaseData.m_ObsArray[i].corrections.prcSatClk      \n
	///   rxBaseData.m_ObsArray[i].corrections.rrcSatClkDrift \n    
	///
	/// \return   true if successful, false if error.    
	bool DetermineSatellitePVT_GPSL1(
		GNSS_RxData *rxData,       //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,   //!< The pointer to the reference receiver data. NULL if not available.
		unsigned &nrValidEph,      //!< The number of GPS L1 channels with valid ephemeris for the rover.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);

	/// \brief    Determine the tropospheric and ionospheric delay for each
	///           GPS L1 channel in rxData. 
	///
	/// \pre      The following values must be valid: \n
	/// rxData.m_time (all values) \n
	/// rxData.m_pvt or rxData.m_pvt_lsq must be a valid estimate \n
	/// rxData.m_ObsArray[i].satellite.elevation \n
	/// rxData.m_ObsArray[i].satellite.azimuth \n
	/// rxData.m_klobuchar \n
	///
	/// \post     The following values are set: \n
	/// rxData.m_ObsArray[i].corrections.prcTropoDry \n
	/// rxData.m_ObsArray[i].corrections.prcTropoWet \n
	/// rxData.m_ObsArray[i].corrections.prcIono \n
	bool DetermineAtmosphericCorrections_GPSL1(
		GNSS_RxData &rxData,       //!< The receiver data.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.)
	);




	/// \brief    Determine the usable GPS L1 pseudroange measurements.
	///
	/// \pre      The rxData.m_ObsArray[i].flags values must be set 
	///           properly including: \n
	/// rxData.m_ObsArray[i].flags.isCodeLocked         \n 
	/// rxData.m_ObsArray[i].flags.isPsrValid           \n
	/// rxData.m_ObsArray[i].flags.isEphemerisValid     \n
	/// rxData.m_ObsArray[i].flags.isNotPsrRejected     \n
	/// rxData.m_ObsArray[i].flags.isNotUserRejected    \n
	///
	/// \post     The following flags are set:
	/// rxData.m_ObsArray[i].flags.isAboveElevationMask \n
	/// rxData.m_ObsArray[i].flags.isAboveCNoMask       \n
	/// rxData.m_ObsArray[i].flags.isAboveLockTimeMask  \n
	/// rxData.m_ObsArray[i].flags.isPsrUsedInSolution.    
	///
	/// \return   true if successful, false if error.    
	bool DetermineUsablePseudorangeMeasurementsForThePositionSolution_GPSL1(
		GNSS_RxData &rxData,            //!< The receiver data.
		unsigned char &nrUsablePseudoranges, //!< The number of usable GPS L1 pseudorange measurements.
		unsigned char &nrPsrObsAvailable,     //!< The number of psr measurements available for use.
		unsigned char &nrPsrObsRejected       //!< The number of psr measurements flagged as rejected.
	);



	/// \brief    Determine the usable GPS L1 ADR measurements.
	///
	/// \pre      The rxData.m_ObsArray[i].flags values must be set 
	///           properly including: \n
	/// rxData.m_ObsArray[i].flags.isCodeLocked         &
	/// rxData.m_ObsArray[i].flags.isPhaseLocked        &
	/// rxData.m_ObsArray[i].flags.isParityValid        & // if not, there is a half cycle amibiguity.
	/// rxData.m_ObsArray[i].flags.isAdrValid           &
	/// rxData.m_ObsArray[i].flags.isAboveElevationMask &
	/// rxData.m_ObsArray[i].flags.isAboveCNoMask       &
	/// rxData.m_ObsArray[i].flags.isAboveLockTimeMask  &
	/// rxData.m_ObsArray[i].flags.isNotUserRejected    &
	/// rxData.m_ObsArray[i].flags.isNotAdrRejected     &
	/// rxData.m_ObsArray[i].flags.isEphemerisValid
	///
	/// \post     The following flags are set:
	/// rxData.m_ObsArray[i].flags.isAboveElevationMask \n
	/// rxData.m_ObsArray[i].flags.isAboveCNoMask       \n
	/// rxData.m_ObsArray[i].flags.isAboveLockTimeMask  \n
	/// rxData.m_ObsArray[i].flags.isAdrUsedSolution.    
	///
	/// \return   true if successful, false if error.        
	bool DetermineUsableAdrMeasurements_GPSL1(
		GNSS_RxData &rxData,   //!< The receiver data.
		unsigned &nrUsableAdr  //!< The number of usable GPS L1 adr measurements.
	);


	/**
	\brief   Determine the corresponding indices for between receiver differencing.
	\author  Glenn D. MacGougan
	\date    2007-12-13

	\pre     The following must be valid: for rxData and rxBaseData obsservations \n
	flags.isPsrUsedInSolution      \n
	flags.isDopplerUsedInSolution  \n
	flags.isAdrUsedInSolution      \n

	\post    The following are set: for rxData and rxBaseData obsservations \n
	flags.isDifferentialPsrAvailable     \n
	flags.isDifferentialDopplerAvailable \n
	flags.isDifferentialAdrAvailable     \n
	index_differential                   \n

	\return  true if successful, false if error.
	*/
	bool DetermineBetweenReceiverDifferentialIndex(
		GNSS_RxData *rxData,                 //!< (input) The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,             //!< (input) The pointer to the reference receiver data. NULL if not available.        
		const bool setToUseOnlyDifferential  //!< (input) This indicates that only differential measurements should be used.    
	);


	/// \brief    Determine the design matrix for the GPS L1 
	///           position solution based on pseudroange measurements.
	///
	/// \pre      The following must be valid:
	/// rxData.m_pvt
	/// rxData.m_ObsArray[i].satellite
	/// rxData.m_ObsArray[i].flags.isPsrUsedInSolution
	/// 
	/// \return   true if successful, false if error.        
	bool DetermineDesignMatrixForThePositionSolution_GPSL1(
		GNSS_RxData &rxData,      //!< The receiver data.
		const unsigned nrRowsInH, //!< The number of valid rows in H.    
		Matrix &H                 //!< The position & rx clock design matrix [n x 4].
	);



	/**
	\brief    Determine a single design matrix element for the GPS L1
	position solution based on its pseudroange measurement.

	\pre      The following must be valid: \n
	rxData.m_pvt or rxData.m_pvt_lsq \n
	rxData.m_ObsArray[index].satellite \n
	rxData.m_ObsArray[index].flags.isPsrUsedInSolution \n

	\post     The following are set. \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[0] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[1] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[2] \n
	rxData.m_ObsArray[index].range \n

	\return   true if successful, false if error.
	*/
	bool DetermineDesignMatrixElement_GPSL1_Psr(
		GNSS_RxData &rxData,       //!< The receiver data.
		const unsigned int index,  //!< The index of the observation i.e. rxData.m_ObsArray[index].
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);


	/**
	\brief    Determine a single design matrix element for the GPS L1
	position solution based on its adr measurement.

	\pre      The following must be valid: \n
	rxData.m_pvt \n
	rxData.m_ObsArray[index].satellite \n
	rxData.m_ObsArray[index].flags.isAdrUsedInSolution \n

	\post     The following are set. \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_a[0] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_a[1] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_a[2] \n
	rxData.m_ObsArray[index].range \n

	\return   true if successful, false if error.
	*/
	bool DetermineDesignMatrixElement_GPSL1_Adr(GNSS_RxData &rxData, const unsigned int index);



	/**
	\brief    Determine the design matrix elements for the GPS L1
	position solution based on pseudroange measurements.

	\pre      The following must be valid: \n
	rxData.m_pvt or rxData.m_pvt_lsq \n
	rxData.m_ObsArray[index].satellite \n
	rxData.m_ObsArray[index].flags.isPsrUsedInSolution \n

	\post     The following are set. \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[0] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[1] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_p[2] \n
	rxData.m_ObsArray[indexi].range \n

	\return   true if successful, false if error.
	*/
	bool DetermineDesignMatrixElements_GPSL1_Psr(
		GNSS_RxData &rxData,       //!< The receiver data.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.)
	);








	/// \brief    Determine the measurement weight matrix for the GPS L1 
	///           position solution based on pseudroange measurement
	///           standard deviation values specified in 
	///           rxData.m_ObsArray[i].stdev_psr.
	///
	/// \pre      The following must be valid:         \n
	/// rxData.m_ObsArray[i].flags.isPsrUsedInSolution \n
	/// rxData.m_ObsArray[i].stdev_psr (not zero)      \n
	/// 
	/// \return   true if successful, false if error.            
	bool DetermineMeasurementWeightMatrixForThePositionSolution_GPSL1(
		GNSS_RxData &rxData,                 //!< The receiver data.
		const unsigned nrUsablePseudoranges, //!< The number of usable GPS L1 pseudorange measurements.
		Matrix &W                            //!< The inverse of the pseudorange measurement variance-coraiance matrix [nP x nP].
	);

	/// \brief    Determine the measurement variance-coariance matrix for 
	///           the GPS L1 position solution based on pseudroange measurement
	///           standard deviation values specified in 
	///           rxData.m_ObsArray[i].stdev_psr.
	///
	/// \pre      The following must be valid:         \n
	/// rxData.m_ObsArray[i].flags.isPsrUsedInSolution \n
	/// rxData.m_ObsArray[i].stdev_psr (not zero)      \n
	/// 
	/// \return   true if successful, false if error.                
	bool DetermineMeasurementVarianceCovarianceMatrixForThePositionSolution_GPSL1(
		GNSS_RxData &rxData,  //!< The receiver data.
		const unsigned n,     //!< The number of measurements and pseudo-measurements (constraints).
		Matrix &R             //!< The pseudorange measurement variance-covariance matrix [n x n].
	);


	/**
	\brief    Compute a single GPS L1 pseudroange misclosures.

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isPsrUsedInSolution  \n
	rxData->m_ObsArray[index].satellite.clkdrift         \n
	rxData->m_ObsArray[index].corrections.prcTropoDry    \n
	rxData->m_ObsArray[index].corrections.prcTropoWet    \n
	rxData->m_ObsArray[index].corrections.prcIono        \n
	rxData->m_ObsArray[index].range                      \n
	rxData->m_pvt.clockOffset  or rxData->m_pvt_lsq.clockOffset \n
	if diffential, the same above for rxBaseData.

	\post     The following is set:     \n
	rxData.m_ObsArray[index].psr_misclosure \n

	\return   true if successful, false if error.
	*/
	bool GNSS_Estimator::DeterminePseudorangeMisclosure_GPSL1(
		GNSS_RxData *rxData,       //!< The pointer to the receiver data.    
		const unsigned int index,  //!< The index of the observation in the receiver data.
		GNSS_RxData *rxBaseData,   //!< The pointer to the reference receiver data. NULL if not available.    
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);


	/**
	\brief    Compute all the GPS L1 pseudroange misclosures.

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[i].flags.isPsrUsedInSolution  \n
	rxData->m_ObsArray[i].satellite.clkdrift         \n
	rxData->m_ObsArray[i].corrections.prcTropoDry    \n
	rxData->m_ObsArray[i].corrections.prcTropoWet    \n
	rxData->m_ObsArray[i].corrections.prcIono        \n
	rxData->m_ObsArray[i].range                      \n
	rxData->m_pvt.clockOffset  or rxData->m_pvt_lsq.clockOffset \n
	if diffential, the same above for rxBaseData.

	\post     The following is set:     \n
	rxData.m_ObsArray[i].psr_misclosure \n

	\return   true if successful, false if error.
	*/
	bool DeterminePseudorangeMisclosures_GPSL1(
		GNSS_RxData *rxData,       //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,   //!< The pointer to the reference receiver data. NULL if not available.    
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);


	/**
	\brief  Compute the miscloures from applying a position constraint.
	The position constraints are pseudo-observations such that \n
	current latitude = previous latitude (the pseudo-obs) \n
	current longitude = previous longitude (the pseudo-obs) \n
	current height = previous height (the pseudo-obs).

	Note that this allows position variation depending on the variance of the
	previous position. It is only fixing the position (fixed position constraint)
	when the variance of the previuos position 'observations' is very very small.

	\return   true if successful, false if error.
	*/
	bool DeterminePositionConstraintMisclosures(
		GNSS_RxData *rxData, //!< The rover receiver data.
		double &w_lat,       //!< The computed latitude constraint misclosure [m].
		double &w_lon,       //!< The computed longitude constraint misclosure [m].
		double &w_hgt        //!< The computed height constraint misclosure [m].
	);


	/**
	\brief  Compute the miscloure from applying a height constraint.
	The height constraints is a pseudo-observations such that
	current height = previous height (the pseudo-obs).

	Note that this allows height variation depending on the variance of the
	previous height. It is only fixing the height (fixed height constraint)
	when the variance of the previuos height 'observations' is very very small.

	\return   true if successful, false if error.
	*/
	bool DetermineHeightConstraintMisclosures(
		GNSS_RxData *rxData, //!< The rover receiver data.
		double &w_hgt        //!< The computed height constraint misclosure [m].
	);


	/**
	\brief    Compute one single difference GPS L1 ADR misclosure.

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isAdrUsedInSolution  \n
	rxData->m_ObsArray[index].satellite.clkdrift         \n
	rxData->m_ObsArray[index].corrections.prcTropoDry    \n
	rxData->m_ObsArray[index].corrections.prcTropoWet    \n
	rxData->m_ObsArray[index].corrections.prcIono        \n
	rxData->m_ObsArray[index].range                      \n
	rxData->m_pvt.clockOffset                        \n
	if diffential, the same above for rxBaseData.

	\post     The following is set:     \n
	rxData.m_ObsArray[index].adr_misclosure \n

	\return   true if successful, false if error.
	*/
	bool DetermineSingleDifferenceADR_Misclosure_GPSL1(
		GNSS_RxData *rxData,      //!< The pointer to the receiver data.    
		const unsigned int index, //!< The index of the observation in the receiver data.
		GNSS_RxData *rxBaseData   //!< The pointer to the reference receiver data. NULL if not available.      
	);

	/**
	\brief    Compute all the single difference GPS L1 ADR misclosures.

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isAdrUsedInSolution  \n
	rxData->m_ObsArray[index].satellite.clkdrift         \n
	rxData->m_ObsArray[index].corrections.prcTropoDry    \n
	rxData->m_ObsArray[index].corrections.prcTropoWet    \n
	rxData->m_ObsArray[index].corrections.prcIono        \n
	rxData->m_ObsArray[index].range                      \n
	rxData->m_pvt.clockOffset                        \n
	if diffential, the same above for rxBaseData.

	\post     The following is set:     \n
	rxData.m_ObsArray[index].adr_misclosure \n

	\return   true if successful, false if error.
	*/
	bool DetermineSingleDifferenceADR_Misclosures_GPSL1(
		GNSS_RxData *rxData,     //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData  //!< The pointer to the reference receiver data. NULL if not available.    
	);


	/**
	\brief    Compute all the single difference GPS L1 ADR residuals (Post-Update).

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isAdrUsedInSolution  \n
	rxData->m_ObsArray[index].satellite.clkdrift         \n
	rxData->m_ObsArray[index].corrections.prcTropoDry    \n
	rxData->m_ObsArray[index].corrections.prcTropoWet    \n
	rxData->m_ObsArray[index].corrections.prcIono        \n
	rxData->m_ObsArray[index].range                      \n
	rxData->m_pvt.clockOffset                        \n
	if diffential, the same above for rxBaseData.

	\post     The following is set:     \n
	rxData.m_ObsArray[index].adr_residual_sd \n

	\return   true if successful, false if error.
	*/
	bool DetermineSingleDifferenceADR_Residuals_GPSL1(
		GNSS_RxData *rxData,    //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData //!< The pointer to the reference receiver data. NULL if not available.        
	);


	/**
	\brief    Compute all the double difference GPS L1 ADR residuals (Post-Update).

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isAdrUsedInSolution  \n
	rxData->m_ObsArray[index].flags.isBaseSatellite      \n
	rxData->m_ObsArray[index].adr_residual_sd            \n

	\post     The following is set:     \n
	rxData.m_ObsArray[index].adr_residual_dd \n

	\return   true if successful, false if error.
	*/
	bool DetermineDoubleDifferenceADR_Residuals_GPSL1(
		GNSS_RxData *rxData,          //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,      //!< The pointer to the reference receiver data. NULL if not available.        
		const unsigned index_baseSat  //!< The index into rxData->m_ObsArray for the base satellite observation data.
	);


	/**
	\brief    Compute all the double difference GPS L1 ADR residuals (Post-Update).

	\pre      The following must be valid:           \n
	rxData->m_ObsArray[index].flags.isAdrUsedInSolution  \n
	rxData->m_ObsArray[index].flags.isBaseSatellite      \n
	rxData->m_ObsArray[index].adr_residual_sd            \n
	rxData->m_ObsArray[index].ambiguity \n
	rxData->m_ObsArray[index].ambiguity_dd_fixed \n

	\post     The following is set:     \n
	rxData.m_ObsArray[index].adr_residual_dd_fixed \n

	\return   true if successful, false if error.
	*/
	bool DetermineDoubleDifferenceADR_Residuals_GPSL1_Fixed(
		GNSS_RxData *rxData,          //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,      //!< The pointer to the reference receiver data. NULL if not available.        
		const unsigned index_baseSat  //!< The index into rxData->m_ObsArray for the base satellite observation data.
	);



	/*
	/// \brief    Compute the double differntial GPS L1 ADR misclosures.
	///
	/// \pre      The following must be valid:           \n
	/// rxData->m_ObsArray[i].flags.isAdrUsedInSolution  \n
	/// rxData->m_ObsArray[i].satellite.clkdrift         \n
	/// rxData->m_ObsArray[i].corrections.prcTropoDry    \n
	/// rxData->m_ObsArray[i].corrections.prcTropoWet    \n
	/// rxData->m_ObsArray[i].corrections.prcIono        \n
	/// rxData->m_ObsArray[i].range                      \n
	/// rxData->m_pvt.clockOffset                        \n
	/// if diffential, the same above for rxBaseData.
	///
	/// \post     The following is set:     \n
	/// rxData.m_ObsArray[i].adr_misclosure \n
	///
	/// \return   true if successful, false if error.
	bool DetermineDoubleDifferenceADR_Misclosures_GPSL1(
	GNSS_RxData *rxData,     //!< The pointer to the receiver data.
	GNSS_RxData *rxBaseData, //!< The pointer to the reference receiver data. NULL if not available.
	Matrix &subB,             //!< The matrix that describes the differencing from SD to DD adr measurements
	const unsigned n,        //!< The number of DD misclosures required.
	Matrix &w                //!< The adr misclosure vector [n x 1].
	);
	*/

	/// \brief    Determine the usable GPS L1 Doppler measurements.
	/// \pre      The rxData.m_ObsArray[i].flags values must be set 
	///           properly including: \n
	/// rxData.m_ObsArray[i].flags.isCodeLocked         \n
	/// rxData.m_ObsArray[i].flags.isDopplerValid       \n
	/// rxData.m_ObsArray[i].flags.isAboveElevationMask \n
	/// rxData.m_ObsArray[i].flags.isAboveCNoMask       \n
	/// rxData.m_ObsArray[i].flags.isAboveLockTimeMask  \n
	/// rxData.m_ObsArray[i].flags.isNotUserRejected    \n
	/// rxData.m_ObsArray[i].flags.isNotDopplerRejected \n
	/// rxData.m_ObsArray[i].flags.isEphemerisValid     \n
	///
	/// \post     The rxData.m_ObsArray[i].flags.isDopplerUsedInSolution
	///           will be set to 1(used) or 0(not used).
	/// \return   true if successful, false if error.        
	bool DetermineUsableDopplerMeasurementsForTheVelocitySolution_GPSL1(
		GNSS_RxData &rxData,                   //!< The receiver data.
		unsigned char &nrUsableDopplers,       //!< The number of usable GPS L1 Doppler measurements.
		unsigned char &nrDopplerObsAvailable,  //!< The number of psr measurements available for use.
		unsigned char &nrDopplerObsRejected    //!< The number of psr measurements flagged as rejected.
	);



	/**
	\brief    Compute a single GPS L1 Doppler misclosure.

	\pre      The folloing must be valid: \n
	rxData.m_ObsArray[index].flags.isDopplerUsedInSolution  \n
	rxData.m_ObsArray[index].satellite.clkdrift \n
	rxData.m_ObsArray[index].rangerate \n
	rxData.m_pvt.clockDrift or rxData.m_pvt_lsq.clockDrift \n

	\post     The following is set: \n
	rxData.m_ObsArray[index].doppler_misclosure \n

	\return   true if successful, false if error.
	*/
	bool DetermineDopplerMisclosure_GPSL1(
		GNSS_RxData *rxData,      //!< The pointer to the receiver data.    
		const unsigned int index, //!< The index of the observation in the receiver data.
		GNSS_RxData *rxBaseData,  //!< The pointer to the reference receiver data. NULL if not available.    
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);

	/**
	\brief    Compute all the GPS L1 Doppler misclosures.

	\pre      The folloing must be valid: \n
	rxData.m_ObsArray[index].flags.isDopplerUsedInSolution  \n
	rxData.m_ObsArray[index].satellite.clkdrift \n
	rxData.m_ObsArray[index].rangerate \n
	rxData.m_pvt.clockDrift or rxData.m_pvt_lsq.clockDrift \n

	\post     The following is set: \n
	rxData.m_ObsArray[index].doppler_misclosure \n

	\return   true if successful, false if error.
	*/
	bool DetermineDopplerMisclosures_GPSL1(
		GNSS_RxData *rxData,       //!< The pointer to the receiver data.    
		GNSS_RxData *rxBaseData,   //!< The pointer to the reference receiver data. NULL if not available.    
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);


	/**
	\brief    Determine a row of the design matrix for the GPS L1
	velocity solution based on Doppler measurement.

	\pre      The following must be valid: \n
	rxData.m_pvt \n
	rxData.m_ObsArray[index].satellite \n
	rxData.m_ObsArray[index].flags.isDopplerUsedInSolution \n

	\post     The following are set: \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[0] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[1] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[2] \n

	\return   true if successful, false if error.
	*/
	bool DetermineDesignMatrixElement_GPSL1_Doppler(
		GNSS_RxData &rxData,      //!< The receiver data.
		const unsigned int index, //!< The index of the observation in the receiver data.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);

	/**
	\brief    Determine all rows of the design matrix for the GPS L1
	velocity solution based on Doppler measurement.

	\pre      The following must be valid: \n
	rxData.m_pvt \n
	rxData.m_ObsArray[index].satellite \n
	rxData.m_ObsArray[index].flags.isDopplerUsedInSolution \n

	\post     The following are set: \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[0] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[1] \n
	rxData.m_ObsArray[index].rxData.m_ObsArray[i].H_v[2] \n

	\return   true if successful, false if error.
	*/
	bool DetermineDesignMatrixElements_GPSL1_Doppler(
		GNSS_RxData &rxData,       //!< The receiver data.
		const bool isLeastSquares  //!< A boolean to indicate if the rover position and velocity values are from least squares rxData->m_pvt_lsq or from rxData->m_pvt.
	);


	/// \brief    Determine the measurement weight matrix for the GPS L1 
	///           velocity solution based on Doppler measurement
	///           standard deviation values specified in 
	///           rxData.m_ObsArray[i].stdev_doppler.
	///
	/// \pre      The following must be valid:         \n
	/// rxData.m_ObsArray[i].flags.isDopplerUsedInSolution \n
	/// rxData.m_ObsArray[i].stdev_doppler (not zero)  \n
	/// 
	/// \return   true if successful, false if error.                
	bool DetermineMeasurementWeightMatrixForTheVelocitySolution_GPSL1(
		GNSS_RxData &rxData,  //!< The receiver data.
		const unsigned n,     //!< The number of rows in W.
		Matrix &W             //!< The inverse of the Doppler measurement variance-coraiance matrix [n x n].
	);

	/// \brief    Determine the measurement variance-covariance matrix for 
	///           the GPS L1 velocity solution based on Doppler measurement
	///           standard deviation values specified in 
	///           rxData.m_ObsArray[i].stdev_doppler.
	///
	/// \pre      The following must be valid:         \n
	/// rxData.m_ObsArray[i].flags.isDopplerUsedInSolution \n
	/// rxData.m_ObsArray[i].stdev_doppler (not zero)  \n
	/// 
	/// \return   true if successful, false if error.                    
	bool DetermineMeasurementVarianceCovarianceMatrixForTheVelocitySolution_GPSL1(
		GNSS_RxData &rxData,  //!< The receiver data.
		const unsigned n,     //!< The number of rows in R.
		Matrix &R             //!< The inverse of the Doppler measurement variance-coraiance matrix [n x n].
	);


	/// \brief    Perform the Global Internal Reliability Test, followed by a 
	///           search using local testing for a single measurement fault if 
	///           the global test fails.
	///
	/// The Global Internal Reliability Test: \n
	/// The aposteriori variance factor is computed and compared to the value 
	/// generated using Chi^2 lookup table. The aposerteriori variance factor 
	/// indicates the following: \n
	/// if( apv ~= 1 ) \n
	///   solution and observations are well modelled. \n
	/// if( apv < 1 ) \n
	///   observation variance matrix is pessimistic. \n
	///   (observations are better than what you said). \n
	/// if( apv > 1 ) \n
	///   (1) The math model incorrect, (not likely)
	///   (2) The weight matrix/observation variance matrix is optimistic 
	///      (observations are worse than what you said). \n
	///   (3) Their is a blunder in the observations that is skewing the normality 
	///       of the solution, and a local test for blunder my be needed. \n
	///
	/// \return   true if successful, false if error.                    
	bool PerformGlobalTestAndTestForMeasurementFaults(
		GNSS_RxData &rxData,           //!< The receiver data object.
		bool testPsrOrDoppler,         //!< This indicates if the psr misclosures are checked, otherwise the Doppler misclosures are checked. 
		Matrix& H,                     //!< The design matrix, H,                           [n x u].
		Matrix& Ht,                    //!< The design matrix transposed, H,                [n x u].
		Matrix& W,                     //!< The observation weight matrix, W,               [n x n].
		Matrix& R,                     //!< The observation variance-covariance matrix, R,  [n x n].
		Matrix& r,                     //!< The observation residual vector,                [n x 1].
		Matrix& P,                     //!< The state variance-covariance matrix,           [u x u].
		const unsigned char n,         //!< The number of observations, n.
		const unsigned char u,         //!< The number of unknowns, u.
		double &avf,                   //!< The computed a-posteriori variance factor is returned.
		bool &isGlobalTestPassed,      //!< This indicates if the global test passed.    
		bool &hasRejectionOccurred,    //!< This indicates if a rejection occurred. Only one measurement is flagged.
		unsigned char &indexOfRejected //!< This is the index of the rejected observation.
	);


	bool ComputeTransitionMatrix_RTK(
		const double dT  //!< The change in time since the last update [s].      
	);
	bool ComputeProcessNoiseMatrix_RTK(
		const double dT  //!< The change in time since the last update [s].      
	);
	bool PredictAhead_RTK(
		GNSS_RxData &rxData, //!< The receiver data.
		const double dT      //!< The change in time since the last update [s].      
	);
	bool InitializeStateVarianceCovarianceFromLeastSquares_RTK(
		Matrix &pos_P, //!< The variance covariance of the position and clock states from least squares, state order: latitude, longitude, height, clock ofset [4x4].
		Matrix &vel_P  //!< The variance covariance of the velocity and clock drift states from least squares, state order: latitude rate, longitude rate, height rate, clock drift [4x4].      
	);
	//bool Kalman_Update_RTK(
	//	GNSS_RxData *rxData,     //!< A pointer to the rover receiver data. This must be a valid pointer.
	//	GNSS_RxData *rxBaseData  //!< A pointer to the reference receiver data if available. NULL if not available.      
	//);


	bool ComputeTransitionMatrix_EKF(
		const double dT  //!< The change in time since the last update [s].
	);

	bool ComputeProcessNoiseMatrix_EKF(
		const double dT  //!< The change in time since the last update [s].
	);

	/// \brief    Initialize the state variance-covariance matrix 
	///           for the 8 state PV Gauss Markov model.
	///
	/// \return   true if successful, false if error.
	bool InitializeStateVarianceCovariance_EKF(
		Matrix &pos_P, //!< The variance covariance of the position and clock states from least squares, state order: latitude, longitude, height, clock ofset [4x4].
		Matrix &vel_P  //!< The variance covariance of the velocity and clock drift states from least squares, state order: latitude rate, longitude rate, height rate, clock drift [4x4].      
	);

	bool PredictAhead_EKF(
		GNSS_RxData &rxData, //!< The receiver data.
		const double dT      //!< The change in time since the last update [s].
	);

	bool Kalman_Update_EKF(
		GNSS_RxData *rxData,      //!< A pointer to the rover receiver data. This must be a valid pointer.
		GNSS_RxData *rxBaseData   //!< A pointer to the reference receiver data if available. NULL if not available.
	);

	/// \brief    Update the position, velocity, and time struct based on the 
	///           gps week and time of week only.
	///
	/// \return   true if successful, false if error.
	bool UpdateTime(
		GNSS_structPVT& pvt // The position, velocity, and time information struct.
	);


	/// \brief    Perform least squares using pseudoranges and Doppler 
	///           measurments to determine the eight states:
	///           (lat,lon,hgt,vn,ve,vup,clk,clkdrift).
	///
	/// \return   true if successful, false if error.
	bool PerformLeastSquares(
		GNSS_RxData *rxData,       //!< A pointer to the rover receiver data. This must be a valid pointer.
		GNSS_RxData *rxBaseData,   //!< A pointer to the reference receiver data if available. NULL if not available.
		bool &wasPositionComputed, //!< A boolean to indicate if a position solution was computed.    
		bool &wasVelocityComputed  //!< A boolean to indicate if a velocity solution was computed.    
	);


	/// \brief    A static function for outputting a matrix to the console.
	bool PrintMatToDebug(const char *name, Matrix& M, const unsigned precision = 6);



	/// \brief    Deal with changes in ambiguities. Remove the 
	///           rows and columns from P of the ambiguities that
	///           are no longer active. Add rows and columns to P
	///           for new ambiguities.
	///
	/// \return   true if successful, false if error.    
	bool DetermineAmbiguitiesChanges(
		GNSS_RxData *rxData,     //!< Pointer to the receiver data.
		GNSS_RxData *rxBaseData, //!< Pointer to the reference receiver data if any (NULL if not available).
		Matrix &P,               //!< The state variance-covariance matrix.
		const bool isEightStateModel, //!< A boolean indicating if the velocity and clock drift states are included.
		bool& changeOccured
	);

	/// \brief	Takes a square matrix and performs U*D*transpose(U)
	///
	/// \return true if successful, false if error.
	bool UDU(
		Matrix &Mat,		//!< Square matrix (input)								[n x n].
		Matrix &U,			//!< Upper triangular matrix (output)					[n x n].
		Matrix &D			//!< Diagonal matrix (output)							[n x n].
	);

	/// \brief Performs UDU on variance covariance matrix P
	///
	/// \return true if successful, falst if error.
	bool Bierman(
		Matrix &P,			//!< Variance-Covariance matrix P- (input)
		Matrix &H,			//!< Matrix H (input)
		Matrix &Ht,			//!< Matrix H transposed (input)
		Matrix &alpha,		//!< Scalar value for (HP-Ht + R)^-1
		Matrix &Uplus,		//!< Resultant upper triangular matrix (output)
		Matrix &Dplus		//!< Resultant diagonal matrix (output)
	);

	/// \brief Performs decorrelation on the process noise matrix Q
	///
	/// \return true if successful, false if error.
	bool Thornton(
		Matrix &UP,		//!< Upper triangular matrix of UDU of P
		Matrix &DP,		//!< Diagonal matrix of UDU of P
		Matrix &P,
		Matrix &T,	//!< Transition matrix
		Matrix &Q		//!< Process noise matrix
	);

	/// \brief Performs inversion on an upper triangular matrix without
	/// explicitly inverting the matrix. Uses backwards substitution
	/// and saves processing time.
	/// U*H' = H  ---> H' = inverse(U)*H
	///
	/// \return true if successful, false if error.
	bool UInverse(
		Matrix &U,
		Matrix &Mat);

	/// \brief Performs decorrelation on measurement noise matrix R
	///
	/// \return true if successful, false if error.
	bool RDecorrelation(
		Matrix &R,
		Matrix &H,
		Matrix &w);

	// Publically accessable data
public:

	// number of unknown paramters: u 
	// number of observations:      n

	struct stLSQ
	{
		Matrix x;    //!< The states,                                                  [u x 1].
		Matrix dx;   //!< The iterative update to the states,                          [u x 1].
		Matrix P;    //!< The states variance-covariance matrix,                       [u x u].
		Matrix H;    //!< The design matrix,                                           [n x u].
		Matrix w;    //!< The observation misclosure vector,                           [n x 1].
		Matrix R;    //!< The variance covariance matrix of the observations,          [n x n].
		Matrix W;    //!< The inverse of m_R,                                          [n x n].
		Matrix r;    //!< The diagonal of the observations variance-covariance matrix, [n x 1].
		double apvf; //!< The a-posteriori variance factor.
		double sqrt_apvf;
		unsigned n;
		unsigned u;
		double rms_residual;
	};

	struct stEKF
	{
		Matrix x;   //!< The states,                                                  [u x 1].
		Matrix dx;  //!< The iterative update to the states,                          [u x 1].
		Matrix P;   //!< The states variance-covariance matrix,                       [u x u].
		Matrix H;   //!< The design matrix,                                           [n x u].
		Matrix w;   //!< The observation misclosure vector,                           [n x 1].
		Matrix R;   //!< The variance covariance matrix of the observations,          [n x n].
		Matrix W;   //!< The inverse of m_R,                                          [n x n].
		Matrix r;   //!< The diagonal of the observations variance-covariance matrix, [n x 1].
		Matrix T;   //!< The transition matrix,                                       [u x u].
		Matrix Q;   //!< The process noise matrix,                                    [u x u].
		Matrix K;   //!< The Kalman gain matrix,                                      [u x n]. 
	};

	struct stRTK
	{
		Matrix x;   //!< The states,                                                  [u x 1].
		Matrix dx;  //!< The iterative update to the states,                          [u x 1].
		Matrix P;   //!< The states variance-covariance matrix,                       [u x u].
		Matrix H;   //!< The design matrix,                                           [n x u].
		Matrix w;   //!< The observation misclosure vector,                           [n x 1].
		Matrix R;   //!< The variance covariance matrix of the observations,          [n x n].
		Matrix W;   //!< The inverse of m_R,                                          [n x n].
		Matrix r;   //!< The diagonal of the observations variance-covariance matrix, [n x 1].
		Matrix T;   //!< The transition matrix,                                       [u x u].
		Matrix Q;   //!< The process noise matrix,                                    [u x u].
		Matrix K;   //!< The Kalman gain matrix,                                      [u x n]. 
		Matrix U_Bierman; //!< The upper triangular matrix UDUt of P                  [u x u].
		Matrix D_Bierman; //!< The diagonal matrix of UDUt of P                       [u x u].
	};

	struct stRTKDD
	{
		Matrix x;     //!< The states,                                                  [u x 1].
		Matrix dx;    //!< The iterative update to the states,                          [u x 1].
		Matrix P;     //!< The states variance-covariance matrix,                       [u x u].
		Matrix H;     //!< The design matrix,                                           [n x u].
		Matrix w;     //!< The observation misclosure vector,                           [n x 1].
		Matrix R;     //!< The variance covariance matrix of the observations,          [n x n].
		Matrix W;     //!< The inverse of m_R,                                          [n x n].
		Matrix r;     //!< The diagonal of the observations variance-covariance matrix, [n x 1].
		Matrix T;     //!< The transition matrix,                                       [u x u].
		Matrix Q;     //!< The process noise matrix,                                    [u x u].
		Matrix K;   //!< The Kalman gain matrix,                                      [u x n]. 
		Matrix B;     //!< The double difference operator matrix.                       [n x n-1/2/3].
		Matrix prevB; //!< The previous double difference operator matrix.              [n_prev x n_prev-1/2/3].
		Matrix SubB;  //!< The double difference operator matrix for just ambiguities.      
		Matrix prevSubB; //!< The previuos double difference operator matrix just ambiguities.
		Matrix U_Bierman; //!< The upper triangular matrix UDUt of P                  [u x u].
		Matrix D_Bierman; //!< The diagonal matrix of UDUt of P                       [u x u].
	};

	struct stEightStateFirstOrderGaussMarkovKalmanModel
	{
		double alphaVn;
		double alphaVe;
		double alphaVup;
		double alphaClkDrift;
		double sigmaVn;
		double sigmaVe;
		double sigmaVup;
		double sigmaClkDrift;

		// default constructor
		stEightStateFirstOrderGaussMarkovKalmanModel()
			: alphaVn(20.0),
			alphaVe(20.0),
			alphaVup(20.0),
			alphaClkDrift(10.0),
			sigmaVn(0.01),
			sigmaVe(0.01),
			sigmaVup(0.01),
			sigmaClkDrift(1000.0)
		{}
	};

	struct stFourStateRandomWalkKalmanModel
	{
		double sigmaNorth;
		double sigmaEast;
		double sigmaUp;
		double sigmaClock;

		// default constructor
		stFourStateRandomWalkKalmanModel()
			: sigmaNorth(0.5),
			sigmaEast(0.5),
			sigmaUp(0.5),
			sigmaClock(100.0)
		{}
	};

	GNSS_FilterType m_FilterType;

	stLSQ m_posLSQ; //!< The Least Sqaures estimation matrix information for the position and clock offset solution.
	stLSQ m_velLSQ; //!< The Least Sqaures estimation matrix information for the velocity and clock drift solution.

	stEKF m_EKF; // The extended kalman filter matrix information for the pvt solution from pseudorange and Doppler.

	stRTK m_RTK; //!< The RTK estimation matrix information.
	stRTKDD m_RTKDD; //!< The double difference RTK estimation matrix information.


					 /// Kalman filter model settings for 1st order Gauss Markov
					 /// Velocity/ClkDrift states. 8 state PVGM model.
	stEightStateFirstOrderGaussMarkovKalmanModel m_FirstOrderGaussMarkovKalmanModel;

	/// Kalman filter model settings for a 4 state random walk model.
	/// Latitude, Longitude, Height, and Clock Offset.
	stFourStateRandomWalkKalmanModel m_FourStateRandomWalkKalmanModel;


	struct stAmbiguityInfo
	{
		unsigned short      channel;        //!< The channel number associated with this measurement.
		unsigned short      id;             //!< The unique id for this channel (eg PRN for GPS).    
		int                 state_index;    //!< The index of the corresponding row and column of the state variance-covariance matrix. -1 means not estimated.
		int                 state_index_dd; //!< The index of the corresponding row and column of the state variance-covariance matrix for the double difference case. -1 means not estimated.
		GNSS_enumSystem     system;         //!< The satellite system associated with this channel.
		GNSS_enumFrequency  freqType;       //!< The frequency type for this channel.
											// GDM most recent ambiguity estimate
											// GDM time of last above
											// GDM flag indicating amb reset
											// GDM quality indicator?
		stAmbiguityInfo()
			: channel(0), id(0), state_index(-1), state_index_dd(-1)
		{}
	};

	/// This list keeps track of which amibiguities are active. i.e. already included in the state vector
	/// and state variance-covariance matrix.
	/// GDM store in RxData?
	std::list<stAmbiguityInfo> m_ActiveAmbiguitiesList;

protected:

	Matrix HtW;  //!< The design matrix, H, transposed times W                      u x n.
	Matrix Ninv; //!< The inverse of the normal matrix, N = (H^T*W*H)^-1,           u x u. 

	FILE* m_debug;
};

} // end namespace GNSS

#endif // _GNSS_ESTIMATOR_H_
