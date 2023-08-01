#include "FreqDomain.h"
#include "../../util/Functions.h"

namespace detection
{
	FreqDomain::FreqDomain(){}
	FreqDomain::FreqDomain(int fftSize, int hopSize)
	{
		this->fftSize = fftSize;
		this->hopSize = hopSize;
	}

	float FreqDomain::GetPitch(juce::HeapBlock<juce::dsp::Complex<float>> &fftFrequencyDomain)
	{
		float maxBinValue = 0;

		// frequency domain analysis
		// for (int bin = 0; bin <= fftSize / 2; ++bin)
		// {
		// 	float amplitude = abs (fftFrequencyDomain[bin]);
		// 	float phase = arg (fftFrequencyDomain[bin]);

		// 	// calculate other interesting quantities based on phase
		// 	float phaseDiff = phase - lastInputPhases.getSample(channel, bin);
		// 	float binCentreFreq = 2.0f * M_PI * (float)bin / (float)fftSize;
		// 	phaseDiff = wrapPhase(phaseDiff - binCentreFreq * hopSize);
		// 	float binDeviation = phaseDiff * (float)fftSize / (float)hopSize / (2.0 * M_PI); // (fractional bin number)

		// 	return maxBinValue;

		// 	// // save analysed values
		// 	// analysisFrequencies[bin] = (float)bin + binDeviation;
		// 	// analysisMagnitudes[bin] = amplitude;
		// 	// lastInputPhases.setSample(channel, bin, phase);

		// 	// // save for pitch detection
		// 	// if (amplitude > maxBinValue) {
		// 	// 	maxBinValue = amplitude;
		// 	// 	maxBinIndex = bin;
		// 	// }
		// }

		return 0.0f;
	}
}