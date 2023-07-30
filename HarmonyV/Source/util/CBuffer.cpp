#include "CBuffer.h"

namespace util
{
	CBuffer::CBuffer()
	{
		buffer = juce::AudioBuffer<float>();
	}

	CBuffer::CBuffer(int channels, int bufferSize)
	{
		this->buffer = juce::AudioBuffer<float>(channels, bufferSize);
		this->reader = 0;
		this->writer = 0;
		this->size = bufferSize;
	}

	void CBuffer::pushRead()
	{
		if (++this->reader >= this->size)
			this->reader = 0;
	}

	void CBuffer::pushWrite()
	{
		if (++this->writer >= this->size)
			this->writer = 0;
	}

	float CBuffer::read(int channel)
	{
		return buffer.getSample(channel, this->reader);
	}

	void CBuffer::write(int channel, float sample)
	{
		buffer.setSample(channel, this->writer, sample);
	}
}