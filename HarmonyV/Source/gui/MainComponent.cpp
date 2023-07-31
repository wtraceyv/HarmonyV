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

		setSize(800, 800);
	}

	// https://docs.juce.com/master/tutorial_rectangle_advanced.html
	void MainComponent::resized()
	{
		auto area = getLocalBounds();

		auto contentHeight = area.getHeight() / 4;

		header.setBounds(area.removeFromTop(contentHeight));
		alwaysLog.setBounds(area.removeFromTop(contentHeight));
		debugsLog.setBounds(area.removeFromTop(contentHeight));
		controls.setBounds(area.removeFromTop(contentHeight));
	}

	void MainComponent::paint (juce::Graphics& g)
	{
	}
}