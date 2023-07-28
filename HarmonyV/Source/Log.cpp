#include "Log.h"

// TODO: std::cout could be an arbitrary stream so it can go to a file or be printed anywhere

void Log::print (std::string toLog)
{
	std::cout << toLog << std::endl;
}

void Log::print (juce::MidiMessage m)
{
	std::cout << m.getDescription() << std::endl;
	(m.isNoteOn()) ? std::cout << "ON" << std::endl : std::cout << "OFF" << std::endl;
}

void Log::cr_print (std::string toLog)
{
	std::cout << "\r" + toLog << std::flush;
}