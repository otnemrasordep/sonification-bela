// Final Assignment for ECS7012P Music and Audio Programming

// SONIFICATION OF AIR POLLUTION DATA 

// Aural comparison of hourly air pollution levels (NO2 values) on Mile End Road, London, from a week in April of 2019 and 2020.
// Sonification is conducted by applying a spectral delay effect to "Air on the G String", Wilhelmj's arrangement of a Bach composition.
// Spectral delay is achieved by a total of 10 bandpass filters with different cutoff frequencies delayed in time by different amounts and with different gains.
// Levels of NO2 are mapped to feedback and gain of every delay line, as well as to movement of cutoff frequencies.
// Higher values of NO2 correspond to:
//		- Higher delay feedback for each line/filter band;
//		- Higher gain of delayed content for each line/filter band;
//		- Lower cutoff frequencies for each line/filter band;
// In summary, low pollution/NO2 levels approximate the output to a "clean" rendition of the piece, whilst high pollution/NO2 levels "pollute" it with delay.
// Holding a button allows to switch from the 2020 scenario (no button pressed) to the 2019 scenario (button pressed and held).

// School of Electronic Engineering and Computer Science
// Queen Mary University of London
// Spring 2020

// Pedro Sarmento

#include <Bela.h>
#include <SampleLoader.h>
#include <CSVReader.h>
#include <filterCoefs.h>
#include <cmath>
#include <vector>

using namespace std;

#define DELAY_BUFFER_SIZE 88200						// Size of buffer delay in samples (2 seconds)
#define NUM_OF_DELAYFILTERS 10						// Number of (filter + delay) lines

// Variables for SampleLoader.h
string gFilename = "Air_Bach.wav";					// Name of the sound file (in project folder)
float *gSampleBuffer;								// Buffer that holds the sound file
int gSampleBufferLength;							// The length of the buffer in frames
int gReadPointer = 0;								// Position of the last frame we played 

// Variables for CSVReader.h
string gDataFile2019 = "NO2_2019_1804_2504.csv";	// Name of .csv data file
string gDataFile2020 = "NO2_2020_1604_2304.csv";	// Name of .csv data file
int gSize2019;										// Number of data points from 2019 file
int gSize2020;										// Number of data points from 2020 file
std::vector<float> gData2019;						// Vector to store data from 2019 file
std::vector<float> gData2020;						// Vector to store data from 2020 file
float gMinGlobal;									// To store minimum of both 2019 and 2020
float gMaxGlobal;									// To store maximum of both 2019 and 2020 

// Variables for data iteration
int gSamplesPerDataPoint;							// To store how many samples should be between every data point given audio file and sample rate
int gCounterDataSamples;							// To iterate data samples, in between every data point
int gReadPointerData;								// To iterate over data points
float gDataPoint;									// To save the value of the data point
float gNextDataPoint;								// To save the next value of the data point
float gPrevDataPoint;								// To save the previous value of the data point
float gDataPointIncrement;							// To save the increment between every data point, give the number of samples

// Variable for button
const int kBigButtonPin = 1;						// Declare pin of button that triggers patterns

// Variables for delays
float gDelayBuffer[NUM_OF_DELAYFILTERS][DELAY_BUFFER_SIZE] = {{0.0}};		// Buffer holding previous samples per channel
int gDelayBufWritePtr[NUM_OF_DELAYFILTERS] = {0};							// Write pointer for delay buffer
float gDelayAmount[NUM_OF_DELAYFILTERS];									// Amount of delay (gain)
float gDelayFeedbackAmount[NUM_OF_DELAYFILTERS];							// Amount of feedback 
float gDelayAmountPre[NUM_OF_DELAYFILTERS];									// Amount of pre-gain delay
int gDelayInSamples[NUM_OF_DELAYFILTERS];									// Amount of delay in samples 

// Variables for filters
float gFilterFrequencies[NUM_OF_DELAYFILTERS];								// Cutoff frequencies for every filter
float gFilterQ[NUM_OF_DELAYFILTERS];										// Quality factor (Q) for every filter
float gFiltXdelay[NUM_OF_DELAYFILTERS][2] = {{0.0}};						// Previous input samples of second-order filter
float gFiltYdelay[NUM_OF_DELAYFILTERS][2] = {{0.0}};						// Previous output samples of second-order filter
float gFilterOut[NUM_OF_DELAYFILTERS] = {0};								// Previous output samples of second-order filter


bool setup(BelaContext *context, void *userData)
{
	// Set the button pins as an input. This isn't strictly needed but is recommended
	pinMode(context, 0, kBigButtonPin, INPUT);
	
	// Get data from .csv files (number of rows and data values)
	gSize2019 = getSizeCSV(gDataFile2019);
	gData2019 = getDataCSV(gDataFile2019, 3);				// NO2 values are stored on column number 3
	gSize2020 = getSizeCSV(gDataFile2020);
	gData2020 = getDataCSV(gDataFile2020, 3);				// NO2 values are stored on column number 3
	
	// Calculate max and min for each dataset
	float min2019 = *std::min_element(gData2019.begin(), gData2019.end());
	float max2019 = *std::max_element(gData2019.begin(), gData2019.end());
	float min2020 = *std::min_element(gData2020.begin(), gData2020.end());
	float max2020 = *std::max_element(gData2020.begin(), gData2020.end());
	
	// Calculate max and min for both datasets
	if(min2019 < min2020){gMinGlobal = min2019;}
	else{gMinGlobal = min2020;}
	if(max2020 > max2019){gMaxGlobal = max2020;}
	else{gMaxGlobal = max2019;}
	
	// Check the length of the audio file and allocate memory
    gSampleBufferLength = getNumFrames(gFilename);
    
    if(gSampleBufferLength <= 0) {
    	rt_printf("Error loading audio file '%s'\n", gFilename.c_str());
    	return false;
    }
    
    gSampleBuffer = new float[gSampleBufferLength];
    
    // Make sure the memory allocated properly
    if(gSampleBuffer == 0) {
    	rt_printf("Error allocating memory for the audio buffer.\n");
    	return false;
    }
    
    // Load the sound into the file 
    getSamples(gFilename, gSampleBuffer, 0, 0, gSampleBufferLength);
    			
	// Calculate the number of samples between every data point
	gSamplesPerDataPoint = gSampleBufferLength/gSize2019;			// This assumes datasets are of equal size
	
	// Initialize filter and delay variables
	for(int i = 0; i < NUM_OF_DELAYFILTERS; i++){
		// Set delay pre gain
		gDelayAmountPre[i] = 1.0;
		// Set delay in samples for each delayfilter line
		gDelayInSamples[i] = (DELAY_BUFFER_SIZE/ (i+1));
		// rt_printf("%d\n", gDelayInSamples[i]);
		// Set Q for each filter
		gFilterQ[i] = 50.0;
		
	}
	return true;
}


void render(BelaContext *context, void *userData)
{
    for(unsigned int n = 0; n < context->audioFrames; n++) {
    	
    	// Read button pin (if pressed equals 0)
    	int buttonNotPressed = digitalRead(context, n, kBigButtonPin);	
    	float outMix = 0;

    	// Counter to get every datapoint, checking every n
    	if(++gCounterDataSamples>=gSamplesPerDataPoint){
    		if(buttonNotPressed == true){
    			// Read from 2020
    			rt_printf("*************** 2020 *\n");
    			gPrevDataPoint = gDataPoint;
    			// Get next data point 
    			gNextDataPoint = gData2020[gReadPointerData];
    			// Calculate increment between previous and next data point
    			gDataPointIncrement = (gNextDataPoint-gPrevDataPoint)/(gSamplesPerDataPoint);
    			// Update data point
    			gDataPoint = gPrevDataPoint;
    		}
    		else{
    			// Read from 2019
    			rt_printf("* 2019 ***************\n");
    			gPrevDataPoint = gDataPoint;
    			// Get next data point 
    			gNextDataPoint = gData2019[gReadPointerData];
    			// Calculate increment between previous and next data point
    			gDataPointIncrement = (gNextDataPoint-gPrevDataPoint)/(gSamplesPerDataPoint);
    			// Update data point
    			gDataPoint = gPrevDataPoint;
    		}
 
    		// Increment read pointer for data
			if(++gReadPointerData>=gSize2019){
				gReadPointerData = 0;
			}
			// rt_printf("%f\n", gPrevDataPoint);
			// rt_printf("%f\n", gNextDataPoint);
			
			// Reset counter after reaching gSamplesPerDataPoint
			gCounterDataSamples = 0;
    	}
    	
    	// To avoid jumps, update value of data point with every calculated Increment
    	gDataPoint += gDataPointIncrement;
    	// Data mappings, for every delay filter line
    	for(int i= 0; i < NUM_OF_DELAYFILTERS; i++){
    		// Map data points to feedback amount of every delay line (lower frequencies have higher feedback)
    		gDelayFeedbackAmount[i] = ::map(gDataPoint, gMinGlobal, gMaxGlobal, 0.07 * (NUM_OF_DELAYFILTERS - i), 0.999);
    		// Map data points to gain of every delay line (lower frequencies have higher gain)
	    	gDelayAmount[i] = ::map(gDataPoint, gMinGlobal, gMaxGlobal, 0.1 , 0.3 * (NUM_OF_DELAYFILTERS - i));
		    	// Uncomment below to experiment with changes on size of delay in samples
		    	// gDelayInSamples[i] = (int)(::map(gDataPoint, gMinGlobal, gMaxGlobal, DELAY_BUFFER_SIZE, DELAY_BUFFER_SIZE/ ((i*10)+1)));
	    	// Map data points to cutoff frequency in every frequency band (bands of 500 Hz) 
    		gFilterFrequencies[i] = ::map(gDataPoint, gMinGlobal, gMaxGlobal, 536.71 + (500.0 *i), 36.71 + (500 * i));
    	}
    
    	 // Read the next sample from the buffer
        float in = gSampleBuffer[gReadPointer];
        if(++gReadPointer >= gSampleBufferLength)
        	gReadPointer = 0;
    	
    	
    	// Apply filter and delay
    	for(int i=0; i < NUM_OF_DELAYFILTERS; i++){
    		
    		// Increment delay buffer write pointer
	        if(++gDelayBufWritePtr[i] >= DELAY_BUFFER_SIZE){
	            gDelayBufWritePtr[i] = 0;
	        }
    		
    		// Call function to get filter coefficients 
    		calculate_coefficients("bp", context->audioSampleRate, gFilterFrequencies[i], gFilterQ[i]);
    	
	    	// Output of the first 2nd-order filter
	        gFilterOut[i]    = b0*in 
	        				 + b1*gFiltXdelay[i][0] 
	        				 + b2*gFiltXdelay[i][1] 
	        				 - a1*gFiltYdelay[i][0] 
	        				 - a2*gFiltYdelay[i][1];
	        
	        gFiltXdelay[i][1] = gFiltXdelay[i][0];
	        gFiltXdelay[i][0] = in;
			
			gFiltYdelay[i][1] = gFiltYdelay[i][0];
			gFiltYdelay[i][0] = gFilterOut[i];
			

	        // Temporary variable to store sample to be written into every delay buffer
	        // Summing the output of each filter with delayed samples with a certain feedback amount
	        float del_input = (gDelayAmountPre[i] * gFilterOut[i] 
	        	            + gDelayBuffer[i][(gDelayBufWritePtr[i]-gDelayInSamples[i]+DELAY_BUFFER_SIZE)%DELAY_BUFFER_SIZE] * gDelayFeedbackAmount[i]);
			
			// Write into every delay buffer
	        gDelayBuffer[i][gDelayBufWritePtr[i]] = del_input;
	
			// Add everything to the output of every delayfilter line
	        gFilterOut[i] = gDelayBuffer[i][(gDelayBufWritePtr[i]-gDelayInSamples[i]+DELAY_BUFFER_SIZE)%DELAY_BUFFER_SIZE] * gDelayAmount[i];    
	    
	    	// Sum all outputs from every delayfilter line
	    	outMix += gFilterOut[i];
    	}
    
    	for(unsigned int channel = 0; channel < context->audioOutChannels; channel++) {
			// Write the sample to every audio output channel
    		audioWrite(context, n, channel, in *0.05 + outMix);						// Add input to keep consistency
    	}
    }
}

void cleanup(BelaContext *context, void *userData)
{
	// Free the buffer we created to load the WAV file
    delete[] gSampleBuffer;
}
