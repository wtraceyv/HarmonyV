#pragma once

#include <JuceHeader.h>
#include "detection/ZeroCrossing.h"

class HarmonyVAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonyVAudioProcessor)

    // many algorithms want an FFT at the beginning

    enum settings {
      fftOrder  = 9,
      fftSize   = 1 << fftOrder,
      hopSize   = fftSize / 4
    } fftSettings;
    juce::dsp::FFT fft;
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomain;     // Time domain results of fft.perform
    juce::HeapBlock<juce::dsp::Complex<float>> fftFreqencyDomain; // Freq domain results

    // detection method
    detection::ZeroCrossing zcDetect;

    // shifting engine
  

public:

    //==============================================================================
    HarmonyVAudioProcessor();
    ~HarmonyVAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
};
