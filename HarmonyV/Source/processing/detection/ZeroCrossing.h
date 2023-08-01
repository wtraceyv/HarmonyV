#include <JuceHeader.h>

namespace detection {

	/**
	 * The most basic method of pitch detection. Count alternating crosses over 0db 
	 * and divide amount counted by the amount of time taken up by that block of samples.
	 * 
	 * May be some tricks to get it more consistent, but it's a good baseline for how 
	 * to architect things.
	*/
	class ZeroCrossing {
		private:
			int sampleRate = 0;
		public:
			ZeroCrossing();
			ZeroCrossing(int sampleRate);
			float GetPitch(const float* inputData, int numSamples);
	};
}