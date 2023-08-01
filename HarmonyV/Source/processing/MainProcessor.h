#include <JuceHeader.h>
#include "../util/CBuffer.h"

using namespace util;

namespace processing
{
	class MainProcessor
	{
		private:
			// store input from system
			CBuffer inputBuffer;
			// summed output ready for system
			CBuffer outputBuffer;
			// Phase info from frequency domain stuff is useful
			juce::AudioBuffer<float> savedInputPhases;
		public:
			MainProcessor();
			MainProcessor(int bufferSize);
			// Plugin is monophonic; only need to take float* as input.
			CBuffer process(const float* inputData, int numSamples);
	};
}