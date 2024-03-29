
#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Log.h"

// TODO: should be set in the plugin where applicable
#define BUFFER_SIZE 1024 * 2
#define FFT_ORDER   9
#define HOP_DIVISOR 4

//==============================================================================
HarmonyVAudioProcessor::HarmonyVAudioProcessor() :  
    AudioProcessor (
        BusesProperties()
        .withInput ("Input", juce::AudioChannelSet::mono(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    ),
    mainProcessor(1024)
{
}

HarmonyVAudioProcessor::~HarmonyVAudioProcessor()
{
}

//==============================================================================

void HarmonyVAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

}

void HarmonyVAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}



void HarmonyVAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Vars which apply to whole block
    int numSamples = buffer.getNumSamples();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) // clean your room
        buffer.clear (i, 0, numSamples);

    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* inputRead = buffer.getReadPointer (channel);
        auto* outputWrite = buffer.getWritePointer (channel);

				// TODO: start using MainProcessor

        // process per sample
        // for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        // {

        // }
    
    }

    // handle anything midi per block
    for (auto meta : midiMessages)
    {
        auto m = meta.getMessage();
        Log::print(m);
    }
}


//==============================================================================
bool HarmonyVAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HarmonyVAudioProcessor::createEditor()
{
    return new HarmonyVAudioProcessorEditor (*this);
}

//==============================================================================
// Saving data for the plugin between shutdowns and such, like user settings

void HarmonyVAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HarmonyVAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// MIDI configurations

const juce::String HarmonyVAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HarmonyVAudioProcessor::acceptsMidi() const
{
  return true; 
}

bool HarmonyVAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HarmonyVAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HarmonyVAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HarmonyVAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HarmonyVAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HarmonyVAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HarmonyVAudioProcessor::getProgramName (int index)
{
    return {};
}

void HarmonyVAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================

#ifndef JucePlugin_PreferredChannelConfigurations
bool HarmonyVAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HarmonyVAudioProcessor();
}
