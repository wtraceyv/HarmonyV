#include "MainProcessor.h"
#include "../util/Functions.h"

using namespace util;

namespace processing
{
	/* constructors */

	MainProcessor::MainProcessor(){}

	MainProcessor::MainProcessor(int bufferSize) :
		inputBuffer(1, bufferSize),
		outputBuffer(2, bufferSize)
	{}

	MainProcessor::MainProcessor(
		int bufferSize, 
		int sampleRate, 
		int fftOrder, 
		int hopSizeDivisor
	) : 
		inputBuffer(1, bufferSize),
		outputBuffer(2, bufferSize),
		freqDomain(
			bufferSize, 
			sampleRate, 
			fftOrder, 
			hopSizeDivisor
		)
	{}

	/* forward declare */

	CBuffer MainProcessor::process(const float* inputData, int numSamples)
	{
		// If we are using frequency domain info elsewhere

		// TODO: detection 

		// TODO: shifting

		return this->outputBuffer;
	}

}