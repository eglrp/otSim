/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       Filters.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef Filters_H
#define Filters_H

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

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Tustin Filters base class for discrete-time equivalent filters of
continuous-time filters.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Filter
{
public:

	///Reset state variables
	void reset() {
		input = 0;
		output = 0;
		initialized = false;
	}

	///Set the input (target) of the filter
	void set(double input_) { input = input_; }

	///Get the output from the filter
	double get() { return output; }

	///Initialize the filter
	void init() {
		output = input;
		initialized = true;
	}

	///Initialize the filter at the given input
	void init(double input_) {
		set(input_);
		init();
	}

protected:
	/// flag indicating whether the filter has been initialized
	bool initialized = false;

	/// current input (target) value for the filter
	double input = 0;

	/// current output value from the filter
	double output = 0;

	virtual void makeConcreteClass() = 0;

};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** First Order Tustin Filter abstract base class

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class FirstOrderFilter : public Filter
{
public:

	///Reset state variables
	void reset() {
		Filter::reset();
		previn = 0;
	}

	///Initialize the filter
	virtual void init() {
		previn = input;
		Filter::init();
	}

	///Initialize the filter at the given input
	virtual void init(double input_) {
		set(input_);
		init();
	}

	///Run filter with given input target, time delta (s), and constants C1, C2, C3, C4
	FirstOrderFilter* filter(double target, double dt, double c_1, double c_2, double c_3, double c_4)
	{
		c2 = c_2;		c3 = c_3;		c4 = c_4;
		return filter(target, dt, c_1);
	}

	///Run filter with given input target, time delta (s), and constant C1
	FirstOrderFilter* filter(double target, double dt, double c_1) {
		c1 = c_1;
		return filter(target, dt);
	}

	///Run filter with given intput target and time delta (s)
	FirstOrderFilter* filter(double target, double dt) {
		input = target;
		return filter(dt);
	}

	///Run filter with given time delta (s)
	FirstOrderFilter* filter(double dt) {
		runFilter(dt);
		return this;
	}

	//constants

	/// Constant C1 from the continous-time form
	double c1 = 1.0;


	/// Constant C2 from the continous-time form
	double c2 = 0.0;
	/// Constant C3 from the continous-time form
	double c3 = 0.0;
	/// Constant C4 from the continous-time form
	double c4 = 0.0;
protected:
	virtual void runFilter(double dt) = 0;

	/// previous input (target) value for the filter
	double previn = 0;

	void makeConcreteClass() override {}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Second Order Tustin Filter base class from the continuous form
((C1*s^2) + (C2*s) + C3) / ((C4*s^2) + (C5*s) + C6).

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class SecondOrderFilter : public Filter
{
public:

	///Reset state variables
	void reset() {
		Filter::reset();
		previn1 = previn2 = 0;
		prevout1 = prevout2 = 0;
	}

	///Initialize the filter
	virtual void init() {
		previn1 = previn2 = input;
		Filter::init();
		if (c6 != 0.0)
			output = (c3 / c6) * input;
		else
			output = 0;

		prevout1 = prevout2 = output;
	}

	///Initialize the filter at the given input
	virtual void init(double input_) {
		set(input_);
		init();
	}

	///Run filter with given input target, time delta (s), and constants C1, C2, C3, C4, C5, C6
	SecondOrderFilter* filter(double target, double dt, double c_1, double c_2, double c_3, double c_4, double c_5, double c_6)
	{
		c1 = c_1;		c2 = c_2;
		c3 = c_3;		c4 = c_4;
		c5 = c_5;		c6 = c_6;
		return filter(target, dt);
	}

	///Run filter with given input target, time delta (s), Natural Frequency in Hz and Damping Ratio
	SecondOrderFilter* filter(double target, double dt, double natural_freq_hz, double damping_ratio_zeta = 0.70711)
	{
		fixedConstants();
		setNaturalFreq(natural_freq_hz, damping_ratio_zeta);

		return SecondOrderFilter::filter(target, dt);
	}

	///Run filter with given intput target and time delta (s)
	SecondOrderFilter* filter(double target, double dt) {
		input = target;
		return filter(dt);
	}

	///Run filter with given time delta (s)
	SecondOrderFilter* filter(double dt) {
		runFilter(dt);
		return this;
	}

	//constants

	/// Constant C1 from the continous-time form
	double c1 = 1.0;
	/// Constant C2 from the continous-time form
	double c2 = 1.0;
	/// Constant C3 from the continous-time form
	double c3 = 1.0;
	/// Constant C4 from the continous-time form
	double c4 = 1.0;
	/// Constant C5 from the continous-time form
	double c5 = 1.0;
	/// Constant C6 from the continous-time form
	double c6 = 1.0;
protected:
	virtual void runFilter(double dt) {
		fixedConstants();
		if (!initialized) init();

		double den = 4.0*c4 + 2.0*c5*dt + c6*dt*dt;
		if (den != 0) {
			double ca = (4.0*c1 + 2.0*c2*dt + c3*dt*dt) / den;
			double cb = (2.0*c3*dt*dt - 8.0*c1) / den;
			double cc = (4.0*c1 - 2.0*c2*dt + c3*dt*dt) / den;
			double cd = (2.0*c6*dt*dt - 8.0*c4) / den;
			double ce = (4.0*c4 - 2.0*c5*dt + c6*dt*dt) / den;

			output = input*ca + previn1*cb + previn2*cc - prevout1*cd - prevout2*ce;
			previn2 = previn1;
			previn1 = input;
			prevout2 = prevout1;
			prevout1 = output;
		}
	}

	virtual void fixedConstants() {}
	virtual void setNaturalFreq(double natural_freq_hz, double damping_ratio_zeta = 0.70711) {};

	/// previous input (target) value for the filter
	double previn1 = 0;
	/// input (target) value before the last input value
	double previn2 = 0;

	/// previous output value from the filter
	double prevout1 = 0;
	/// output value before the last output value
	double prevout2 = 0;

	void makeConcreteClass() override {}
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Integrator Tustin Filter from the continous form (C1 / s).
C1 is the filter gain.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Integrator : public FirstOrderFilter
{
protected:
	void runFilter(double dt) override {
		if (!initialized) init();

		double ca = dt*c1 / 2.00;

		output = input*ca + previn*ca + output;
		previn = input;
	}
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Derivator Tustin Filter from the continous form (C1*s).
C1 is the filter gain.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Derivator : public FirstOrderFilter
{
protected:
	void runFilter(double dt) override {
		if (!initialized) init();

		double ca = 2.00*c1 / dt;

		output = input*ca - previn*ca - output;
		previn = input;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** First-Order Lag Filter from the continuous form (C1 / (s + C1)).
C1 is equivalent to one over the time constant (1 / tau).
This filter is also known as a first-order low-pass filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class FirstOrderLag : public FirstOrderFilter
{
protected:
	void runFilter(double dt) override {
		if (!initialized) init();

		double den = 2.00 + dt*c1;
		if (den != 0) {
			double ca = dt*c1 / den;
			double cb = (2.00 - dt*c1) / den;

			output = input*ca + previn*ca + output*cb;
			previn = input;
		}
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** First-Order Low Pass Filter from the continuous form (C1 / (s + C1)).
C1 is equivalent to one over the time constant (1 / tau).
This filter is also known as a first-order lag filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class FirstOrderLowPass : public FirstOrderLag
{

};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Washout Filter from the continuous form (s / (s + C1)).
C1 is equivalent to one over the time constant (1 / tau).
This filter is also known as a first order high-pass filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Washout : public FirstOrderFilter
{
protected:
	void runFilter(double dt) override {
		if (!initialized) init();

		double den = 2.00 + dt*c1;
		if (den != 0) {
			double ca = 2.00 / den;
			double cb = (2.00 - dt*c1) / den;

			output = input*ca - previn*ca + output*cb;
			previn = input;
		}
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** First-Order High-Pass Filter from the continuous form (s / (s + C1)).
C1 is equivalent to one over the time constant (1 / tau).
This filter is also known as a washout filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class FirstOrderHighPass : public Washout
{

};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Lead-Lag Filter from the continuous form ((C1*s) + C2) / ((C3*s) + C4).
C2 and C4 are usually 1, in which case C1 is the lead-time constant and C3 is
the lag-time constant.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class LeadLag : public FirstOrderFilter
{
public:
	LeadLag() {
		c2 = c3 = c4 = 1.0;
	}

	void init() override {
		FirstOrderFilter::init();
		if (c4 != 0.0)
			output = (c2 / c4) * input;
		else
			output = 0;
	}

	///Initialize the filter at the given input
	void init(double input_) override {
		set(input_);
		init();
	}

protected:
	void runFilter(double dt) override {
		if (!initialized) init();

		double den = 2.00*c3 + dt*c4;
		if (den != 0) {
			double ca = (2.00*c1 + dt*c2) / den;
			double cb = (dt*c2 - 2.00*c1) / den;
			double cc = (2.00*c3 - dt*c4) / den;

			output = input*ca + previn*cb + output*cc;
			previn = input;
		}
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Second-Order Aliasing Filter from the continous form
(1) / ((C4*s^2) + (C5*s) + C6).

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class SecondOrderAliasing : public SecondOrderFilter
{
public:
	SecondOrderAliasing() {
		fixedConstants();
	}

protected:
	void fixedConstants() override {
		c1 = c2 = 0.0;
		c3 = 1.0;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Low-Pass Filter from the continous form (ωn^2) / (s^2 + 2*zeta*ωn*s + ωn^2)
ωn is the natural frequency in Hz.  zeta is the damping ratio.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class SecondOrderLowPass : public SecondOrderFilter
{
public:
	SecondOrderLowPass() {
		fixedConstants();
		setNaturalFreq(80.0, 0.70711);
	}

	void setNaturalFreq(double natural_freq_hz, double damping_ratio_zeta = 0.70711) override {
		c5 = 2.0*damping_ratio_zeta*natural_freq_hz;
		c3 = c6 = natural_freq_hz*natural_freq_hz;
	}

protected:
	void fixedConstants() override {
		c1 = c2 = 0.0;
		c4 = 1.0;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** High-Pass Filter from the continous form (s^2) / (s^2 + 2*zeta*ωn*s + ωn^2)
ωn is the natural frequency in Hz.  zeta is the damping ratio.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class SecondOrderHighPass : public SecondOrderFilter
{
public:
	SecondOrderHighPass() {
		fixedConstants();
		setNaturalFreq(80.0, 0.70711);
	}

	void setNaturalFreq(double natural_freq_hz, double damping_ratio_zeta = 0.70711) override {
		c5 = 2.0*damping_ratio_zeta*natural_freq_hz;
		c6 = natural_freq_hz*natural_freq_hz;
	}

protected:
	void fixedConstants() override {
		c1 = c4 = 1.0;
		c2 = c3 = 0.0;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Band-Pass Filter from the continous form (2*zeta*ωn*s) / (s^2 + 2*zeta*ωn*s + ωn^2)
ωn is the natural frequency in Hz.  zeta is the damping ratio.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class BandPass : public SecondOrderFilter
{
public:
	BandPass() {
		fixedConstants();
		setNaturalFreq(80.0, 0.70711);
	}

	void setNaturalFreq(double natural_freq_hz, double damping_ratio_zeta = 0.70711) override {
		c2 = c5 = 2.0*damping_ratio_zeta*natural_freq_hz;
		c6 = natural_freq_hz*natural_freq_hz;
	}

protected:
	void fixedConstants() override {
		c1 = c3 = 0.0;
		c4 = 1.0;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Band-Stop Filter from the continous form (s^2 + ωn^2) / (s^2 + 2*zeta*ωn*s + ωn^2)
ωn is the natural frequency in Hz.  zeta is the damping ratio.
This filter is also known as a notch filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class BandStop : public SecondOrderFilter
{
public:
	BandStop() {
		fixedConstants();
		setNaturalFreq(80.0, 0.70711);
	}

	void setNaturalFreq(double natural_freq_hz, double damping_ratio_zeta = 0.70711) override {
		c5 = 2.0*damping_ratio_zeta*natural_freq_hz;
		c3 = c6 = natural_freq_hz*natural_freq_hz;
	}

protected:
	void fixedConstants() override {
		c1 = c4 = 1.0;
		c2 = 0.0;
	}
};

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Notch Filter from the continous form (s^2 + ωn^2) / (s^2 + 2*zeta*ωn*s + ωn^2)
ωn is the natural frequency in Hz.  zeta is the damping ratio.
This filter is also known as a band-stop filter.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class Notch : public BandStop
{

};


} //namespace otMath

#endif //Filters_H