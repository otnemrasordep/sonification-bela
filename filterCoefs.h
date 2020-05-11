/***** filterCoefs.h *****/

#include <cmath>

//Feedback coefficients (excluding a0)
float a0 = 0.0;
float a1 = 0.0;
float a2 = 0.0;
// Feedforward coefficients 
float b0 = 0.0;
float b1 = 0.0;							
float b2 = 0.0;


// Calculate bandpass filter coefficients given specifications
void calculate_coefficients(string filterType, float sampleRate, float frequency, float q)
{
	float T = 1 / sampleRate;											// Sampling period, T
	float w0 = 2 * M_PI * frequency;									// Angular frequency of the filter cutoff
	a0 = ((4*q) + (w0 * 2 * T) + (pow(w0, 2.0) * q * pow(T, 2.0)));	
	// a0 to normalize other coefficients
	
	// Feedback coefficients
	a1 = ((2 * pow(w0, 2.0) * q * pow(T, 2.0)) - (8 * q)) / a0;	
	a2 = ((4 * q) - (w0 * 2 * T) + (pow(w0, 2.0) * q * pow(T, 2.0))) / a0;
	
	if(filterType=="lp"){
		// 2nd-order lowpass filter feedforward coefficients 
		b0 = (pow(w0, 2.0) * q * pow(T, 2.0)) / a0;
		b1 = (2 * (pow(w0, 2.0) * q * pow(T, 2.0))) / a0;
		b2 = (pow(w0, 2.0) * q * pow(T, 2.0)) / a0;
	}
	else if(filterType=="hp"){
		// 2nd-order highpass filter feedforward coefficients
		b0 = (4*q)/a0;
		b1 = (- 8*q)/a0;
		b2 = (4*q)/a0;
	}
	else if(filterType=="bp"){
		// 2nd-order bandpass filter feedforward coefficients
		b0 = (2.0 * T * w0) / a0;
		b1 = 0.0;						// b1 coefficient is zero
		b2 = (-2.0 * T * w0) / a0;
	}
	else{
		cout << "Filter type not recognized. Please choose from lp, hp or bp.\n";
	}
}