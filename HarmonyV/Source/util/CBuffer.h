#include <JuceHeader.h>

namespace util
{
	/**
	 * Circular buffer, useful for many conventional ways of working with audio.
	 * In particular, trying to abstract windowing and hops away from main processor,
	 * and make that code as concise as possible.
	 * 
	 * This class manages the wrapping and everything automatically, 
	 * so that's why you do everything via method.
	*/
	class CBuffer
	{
		private:
			juce::AudioBuffer<float> buffer;
			int reader;
			int writer;
			int size;
		public:
			CBuffer();
			CBuffer(int channels, int bufferSize);
			// Push read pointer forward one sample, wrap around if reaches "end"
			void pushRead();			
			// Push write pointer forward one sample, wrap around if reaches "end"
			void pushWrite();
			// Read the read pointer's current sample
			float read(int channel);
			// Write to the write pointer's current sample
			void write(int channel, float sample);
	};
}