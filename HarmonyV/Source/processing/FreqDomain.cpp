#include "FreqDomain.h"
#include "../util/Functions.h"

FreqDomain::FreqDomain() : 
	fft(9),
	savedInputPhases(1, 1024),
	fftFrequencyDomain(1 << 9),
	fftAnalysisFrequencies(1 << 9),
	fftAnalysisMagnitudes(1 << 9)
{
	this->bufferSize = 1024;
	this->sampleRate = 44100;
	this->fftSize = 1 << 9;
	this->hopSize = 1 << 9 / 4;
}

FreqDomain::FreqDomain(
	int bufferSize, 
	int samepleRate, 
	int fftOrder,
	int hopSizeDivisor
) :
	fft(fftOrder),
	savedInputPhases(1, bufferSize),
	fftFrequencyDomain(1 << fftOrder),
	fftAnalysisFrequencies(1 << fftOrder),
	fftAnalysisMagnitudes(1 << fftOrder)
{
	this->bufferSize = bufferSize;
	this->sampleRate = sampleRate;
	this->fftSize = 1 << fftOrder;
	this->hopSize = this->fftSize / hopSizeDivisor;
}


void FreqDomain::fftAnalysisProcess()
{
	for (int bin = 0; bin <= this->fftSize / 2; ++bin)
	{
		float amplitude = abs (this->fftFrequencyDomain[bin]);
		float phase = arg (this->fftFrequencyDomain[bin]);

		// calculate other interesting quantities based on phase
		float phaseDiff = phase - this->savedInputPhases.getSample(0, bin);
		float binCentreFreq = 2.0f * M_PI * (float)bin / (float)this->fftSize;
		phaseDiff = util::wrapPhase(phaseDiff - binCentreFreq * this->hopSize);
		float binDeviation = phaseDiff * (float)this->fftSize / (float)this->hopSize / (2.0 * M_PI); // (fractional bin number)

		// save analysed values
		this->fftAnalysisFrequencies[bin] = (float)bin + binDeviation;
		this->fftAnalysisMagnitudes[bin] = amplitude;
		this->savedInputPhases.setSample(0, bin, phase);

		// save for pitch detection
		if (amplitude > this->maxBinValue) {
			this->maxBinValue = amplitude;
			this->maxBinIndex = bin;
		}
	}
}

float FreqDomain::GetPitch()
{
	return this->fftAnalysisFrequencies[this->maxBinIndex] * (float)this->sampleRate / (float)this->fftSize;
}