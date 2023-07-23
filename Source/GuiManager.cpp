/*
  ==============================================================================

    GuiManager.cpp
    Created: 4 Jun 2023 5:04:37pm
    Author:  walter

  ==============================================================================
*/

#include "GuiManager.h"

GuiManager::GuiManager()
{
  testHeader.setColour (juce::TextButton::buttonColourId, juce::Colours::cornflowerblue);
  testHeader.setButtonText ("last test, bitch");
  addAndMakeVisible (testHeader);

  setSize (400, 400);
}

void GuiManager::paint (juce::Graphics& g)
{
  g.fillAll (juce::Colours::darkgrey);
}

void GuiManager::resized()
{
  auto area = getLocalBounds();
  auto headerHeight = 36;
  testHeader.setBounds (area.removeFromTop (headerHeight));
}
