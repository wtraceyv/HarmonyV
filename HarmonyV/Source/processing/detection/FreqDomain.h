#include <JuceHeader.h>

namespace detection
{
	class FreqDomain
	{
		private:
			int fftSize;
			int hopSize;
		public:
			FreqDomain();
			FreqDomain(int fftSize, int hopSize);
			/**
			 * Get a pitch estimate based on already-calculated fft info.
			*/
			static float GetPitch(
				juce::HeapBlock<juce::dsp::Complex<float>> &fftFrequencyDomain
			);
	};
}