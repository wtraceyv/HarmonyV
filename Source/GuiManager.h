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
    GuiManager();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::TextButton testHeader;
};
