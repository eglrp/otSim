/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       PID.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef PID_H
#define PID_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otMath {

enum class INTEGRATOR_TYPE
{
	RECTANGULAR = 0,
	TRAPEZOIDAL,
	ADAMS_BASHFORTH_2, //2nd order
	ADAMS_BASHFORTH_3, //3rd order
};

enum class PID_TYPE
{
	IDEAL = 0,
	STANDARD = 1,
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Proportional-Integral-Derivative (PID) Controller class

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


class PID
{
public:

	///Set the Constants for the Proportional, Integral, and Derivative terms
	void setConstants(double kp, double ki = 0, double kd = 0) {
		Kp = kp;		Ki = ki;		Kd = kd;
	}

	///Set the PID Type
	void setPIDType(PID_TYPE type) { pidType = type; }

	///Set the Integrator Type
	void setIntegratorType(INTEGRATOR_TYPE type) { integratorType = type; }

	///Get the output from the PID
	double get() const { return output; }

	///Run PID with given error, time delta (s), constants Kp, Ki, Kd, and windup prevention boolean
	PID* filter(double error_, double dt, double kp, double ki, double kd, bool stop_ = false)
	{
		setConstants(kp, ki, kd);
		return filter(error_, dt, stop_);
	}

	///Run PID with given error, time delta (s), and windup prevention boolean
	PID* filter(double error_, double dt, bool stop_ = false)
	{
		error = error_;		stop = stop_;
		solve(dt);
		return this;
	}

	///Reset the PID fully
	void reset()
	{
		stop = false;
		integration = 0;
		error = error_prev = error_prev2 = 0.0;
		output = 0.0;
	}

private:
	///PID Type
	PID_TYPE		pidType = PID_TYPE::STANDARD;
	///Integrator Type
	INTEGRATOR_TYPE integratorType = INTEGRATOR_TYPE::ADAMS_BASHFORTH_2;

	//Constants
	///Proportional Constant
	double Kp = 1.0;
	///Integral Constant
	double Ki = 0.0;
	///Derivative Constant
	double Kd = 0.0;

	//State variables
	bool stop = false; //boolean to stop integrating and reset integrator to 0 (zero)
	double integration = 0.0; //total integration value
	double error = 0.0; //current error
	double error_prev = 0.0; //previous error (Not used for RECTANGULAR integration)
	double error_prev2 = 0.0;  //error before the last error (Only used for 3rd order integration)
	double output = 0.0; //current result

	void solve(double dt)
	{
		double derivative = (error - error_prev) / dt;

		// Reset the integration to 0 if "stop" wind-up condition is true
		if (stop) {
			integration = 0.0; // Reset integrator to 0.0
		}
		else {
			double integration_delta = 0;
			switch (integratorType) {
			case INTEGRATOR_TYPE::RECTANGULAR:
				integration_delta = Ki * dt * error;
				break;
			case INTEGRATOR_TYPE::TRAPEZOIDAL:
				integration_delta = (Ki / 2.0) * dt * (error + error_prev);
				break;
			case INTEGRATOR_TYPE::ADAMS_BASHFORTH_2:
				integration_delta = Ki * dt * (1.5*error - 0.5*error_prev);
				break;
			case INTEGRATOR_TYPE::ADAMS_BASHFORTH_3:
				integration_delta = (Ki / 12.0) * dt * (23.0*error - 16.0*error_prev + 5.0*error_prev2);
				break;
			}
			integration += integration_delta;
		}

		if (pidType == PID_TYPE::IDEAL) {
			output = Kp*error + integration + Kd*derivative;
		}
		else if (pidType == PID_TYPE::STANDARD) {
			output = Kp*(error + integration + Kd*derivative);
		}

		error_prev2 = error_prev;
		error_prev = error;
	}
};


} //namespace otMath

#endif //PID_H