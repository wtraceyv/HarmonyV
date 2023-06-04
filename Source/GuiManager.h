/*
  ==============================================================================

    GuiManager.h
    Created: 4 Jun 2023 5:04:30pm
    Author:  walter

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

class GuiManager : public juce::Component
{
public:
    GuiManager()
    {
        testHeader.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
        testHeader.setButtonText ("Hello yes I work");
        addAndMakeVisible (testHeader);

        setSize (400, 400);
    }

    void paint (juce::Graphics& g) override
    {
        // paint background here
        g.fillAll (juce::Colours::darkgrey);
    }

    void resized() override
    {
        auto area = getLocalBounds();

        auto headerHeight = 36;
        testHeader.setBounds (area.removeFromTop (headerHeight));
    }

private:
    juce::TextButton testHeader;
};
