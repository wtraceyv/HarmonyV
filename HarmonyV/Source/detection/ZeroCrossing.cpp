#include "ZeroCrossing.h"

namespace detection
{
	ZeroCrossing::ZeroCrossing()
	{
		this->sampleRate = 44100;
	}

	ZeroCrossing::ZeroCrossing(int sampleRate)
	{
		std::cout << "Sample rate was " << this->sampleRate << std::endl;
		this->sampleRate = sampleRate;
		std::cout << "Sample rate now " << this->sampleRate << std::endl;
	}

	float ZeroCrossing::GetPitch(const float* inputData, int numSamples)
	{
		int crosses = 0, prevSample = 0;
		float pitch = 0.0f;
		float secondsThisBlock = (float)numSamples / (float)this->sampleRate; // "period length"

		// Count zero crosses
		for (int curSample = 1; curSample < numSamples; curSample++, prevSample++)
		{
			// Found a cross (one direction, will skip every other)
			if (inputData[prevSample] > 0 && inputData[curSample] <= 0)
			{
				crosses++;
			}
		}

		pitch = crosses / secondsThisBlock;

		return pitch;
	}
}
