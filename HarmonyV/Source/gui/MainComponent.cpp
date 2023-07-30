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

		outputChannelControls.setColour(juce::TextButton::buttonColourId, juce::Colours::yellow);
		outputChannelControls.setButtonText("OutputChannelControls");
		addAndMakeVisible(outputChannelControls);

		setSize(600, 420);
	}

	// https://docs.juce.com/master/tutorial_rectangle_advanced.html
	void MainComponent::resized()
	{
		auto area = getLocalBounds();

		auto contentHeight = area.getHeight() / 4;

		header.setBounds(area.removeFromTop(contentHeight));
		alwaysLog.setBounds(area.removeFromTop(contentHeight));
		debugsLog.setBounds(area.removeFromTop(contentHeight));
		outputChannelControls.setBounds(area.removeFromTop(contentHeight));
	}

	void MainComponent::paint (juce::Graphics& g)
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		// g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

		// g.setColour (juce::Colours::white);
		// g.setFont (15.0f);
		// g.drawFittedText ("Yea bb we change something", getLocalBounds(), juce::Justification::centred, 1);
	}
}