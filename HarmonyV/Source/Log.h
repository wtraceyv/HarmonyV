#include <JuceHeader.h>
#include <string>

/**
 * Eventually wanting to be the generic logging gateway
*/
class Log
{
	public:
		static void print(std::string toLog);
		static void print(juce::MidiMessage m);

		/**
		 * Print on the same line continuously to watch a value
		 * which may change rapidly.
		*/
		static void cr_print(std::string toLogOnSameLine);
};