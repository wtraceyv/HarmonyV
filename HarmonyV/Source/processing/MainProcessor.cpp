#include "MainProcessor.h"

namespace processing
{
	MainProcessor::MainProcessor() :
		inputBuffer(1, 1024),
		outputBuffer(2, 1024)
	{}

	MainProcessor::MainProcessor(int bufferSize) :
		inputBuffer(1, bufferSize),
		outputBuffer(2, bufferSize)
	{}

	CBuffer MainProcessor::process(const float* inputData, int numSamples)
	{
		// TODO: do some dang ol processing

		// TODO: detection 

		// TODO: shifting

		// TODO: make sure that somehow the phases from FFT stuff are saved and accessible as soon as they are calulated

		return this->outputBuffer;
	}
}