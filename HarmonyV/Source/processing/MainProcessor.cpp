#include <string>

#include "MainProcessor.h"
#include "../util/Functions.h"
#include "../util/CBuffer.h"
#include "../Log.h"

using namespace util;

namespace processing
{
	/* constructors */

	MainProcessor::MainProcessor(){}

	MainProcessor::MainProcessor(int bufferSize) :
		inputBuffer(1, bufferSize),
		outputBuffer(2, bufferSize),
		freqDomain()
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

	CBuffer MainProcessor::process(const float* inputData, int numSamples)
	{

		// fill input circular buffer no matter what
		for(int sample = 0; sample < numSamples; sample++)
		{
			this->inputBuffer.write(0, inputData[sample]);
			Log::print(std::to_string(inputData[sample]));
		}

		// If we are using frequency domain info elsewhere

		// TODO: detection 

		// TODO: shifting

		return this->outputBuffer;
	}

}
