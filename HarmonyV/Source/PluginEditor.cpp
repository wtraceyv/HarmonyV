/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmonyVAudioProcessorEditor::HarmonyVAudioProcessorEditor (HarmonyVAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 800);
}

HarmonyVAudioProcessorEditor::~HarmonyVAudioProcessorEditor()
{
}

//==============================================================================
void HarmonyVAudioProcessorEditor::paint (juce::Graphics& g)
{
    addAndMakeVisible(mc);
}

void HarmonyVAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
