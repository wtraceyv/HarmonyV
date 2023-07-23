#include "Log.h"

void Log::print (std::string toLog)
{
	std::cout << toLog << std::endl;
}

void Log::print (juce::MidiMessage m)
{
	std::cout << m.getDescription() << std::endl;
	(m.isNoteOn()) ? std::cout << "ON" << std::endl : std::cout << "OFF" << std::endl;
}