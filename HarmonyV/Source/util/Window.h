#include <JuceHeader.h>

namespace util
{
	class Window
	{
		private:
			int size;
			float windowSum;
			float scaleFactor;
			juce::HeapBlock<float> window;

			float hann(float sample);
		public:
			Window(int size);
			// Get the factor the window applies at the given index
			float GetWeight(int index);
			// The sum of all the weights in the window
			float GetSum();
			float GetScaleFactor();
	};
}