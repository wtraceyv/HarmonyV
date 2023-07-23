#include <JuceHeader.h>
#include <string>

class Log
{
	public:
		static void print(std::string toLog);
		static void print(juce::MidiMessage m);
};