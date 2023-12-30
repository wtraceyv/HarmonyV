#include <JuceHeader.h>
#include "../util/Window.h"
#include "../util/CBuffer.h"

using namespace util;

class FreqDomain
{
	private:
		juce::dsp::FFT fft;
		util::Window window;
	public:
		FreqDomain();
		FreqDomain(
			int bufferSize, 
			int sampleRate, 
			int fftOrder, 
			int hopSizeDivisor
		);
		void processSample(CBuffer &input);
		void fftAnalysisProcess();
		float GetPitch();

		/* Members for analysis */
		int sampleRate;
		int bufferSize;
		int fftSize;
		int hopCount = 0;
		int hopSize;
		juce::AudioBuffer<float> savedInputPhases;
		juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomain;
		juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain;
		juce::HeapBlock<float> fftAnalysisFrequencies;
		juce::HeapBlock<float> fftAnalysisMagnitudes;
		float maxBinValue = 0.0f;
		int maxBinIndex = 0;
};