#include <JuceHeader.h>

namespace detection
{
	class FreqDomain
	{
		private:
		public:
			/**
			 * Get a pitch estimate based on already-calculated fft info.
			 * Must supply a hopSize, will use size of freq domain info for fft size.
			*/
			static float GetPitch(
				juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain&,
				int hopSize
			);
	};
}