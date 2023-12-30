#include <JuceHeader.h>
#include "../util/CBuffer.h"
#include "FreqDomain.h"

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

			// Main frequency domain analysis
			FreqDomain freqDomain;
		public:
			MainProcessor();
			MainProcessor(int bufferSize);
			MainProcessor(
				int bufferSize, 
				int sampleRate, 
				int fftOrder, 
				int hopSizeDivisor
			);
			// Plugin is monophonic; only need to take float* as input.
			CBuffer process(const float* inputData, int numSamples);
	};
}