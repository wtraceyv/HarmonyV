#include "MainComponent.h"

namespace gui
{
	MainComponent::MainComponent()
	{
		header.setColour(juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
		header.setButtonText("Header");
		addAndMakeVisible(header);

		alwaysLog.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
		alwaysLog.setButtonText("Always Log");
		addAndMakeVisible(alwaysLog);

		debugsLog.setColour(juce::TextButton::buttonColourId, juce::Colours::lime);
		debugsLog.setButtonText("Debugs");
		addAndMakeVisible(debugsLog);

		addAndMakeVisible(controls);

		setSize(1200, 1200);
	}

	// https://docs.juce.com/master/tutorial_rectangle_advanced.html
	void MainComponent::resized()
	{
		auto area = getLocalBounds();

		auto contentHeight = area.getHeight() / 4;

		header.setBounds(area.removeFromTop(contentHeight / 3));
		alwaysLog.setBounds(area.removeFromTop(contentHeight / 3));
		debugsLog.setBounds(area.removeFromTop(contentHeight / 3));
		controls.setBounds(area.removeFromTop(contentHeight + 40));
	}

	void MainComponent::paint (juce::Graphics& g)
	{
	}
}