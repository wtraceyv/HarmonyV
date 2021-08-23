/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <string>
#include <cmath>

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public foleys::MagicProcessor , juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
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
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void initialiseBuilder (foleys::MagicGUIBuilder& builder) override;
    
    //==============================================================================
    const float maxRatio = powf (2.0f, -24 / 12.0f);
    
    // https://docs.juce.com/master/tutorial_spectrum_analyser.html
    enum
    {
        // pretty dinstinctly affect performance/sound/latency etc.:
        fftOrder    = 9,
        fftSize     = 1 << fftOrder,
        hopSize = (int) (fftSize / 4),
        numVoices = 4
    };
    float curDetectedFrequency;
    int curDetectedMidiNote;
    std::string curDetectedNoteName;
    
    // helpers/init
    float wrapPhase (const float phase);
    void clearBuffers ();
    void clearSynthesisBin(int bin);
    void clearPhases();
    
    // remove some equations from processBlock for readability
    float genRatio (const int shift);
    int genResampleLength (float ratio);
    float genOutPhase (float synthFreq, int bin, float lastOutputPhase);
    float genPanRatio (int voice, int isRightChannel);

    // handle synthesis of multiple voices outside of processBlock
    void shiftSynthesisBins (int voice, int newBin, int origBin, float pitchRatio);
    void setNewFreqDomainUniques (int voice, int bin, float amplitude, float outPhase);
    void setOutPhases (int voice, int channel, int bin, float outPhase);
    void storeFreqDomainConjugates(int voice, int bin);
    void fftSelectiveInverse(int voice);

    // getters for multiple voices
    float getSynthesisMagnitude(int voice, int bin);
    float getSynthesisFrequency(int voice, int bin);
    float getOutputPhase(int voice, int channel, int bin);
    float getTimeOutputReal(int voice, int sample);

private:
    // input gui params
    juce::AudioParameterInt* inputGain;
    juce::AudioParameterInt* inOutputGain;
    juce::AudioParameterInt* inputPan;
    juce::AudioParameterBool* inputSolo;
    juce::AudioParameterBool* inputMute;
    
    // v1
    juce::AudioParameterInt* v1gain;
    juce::AudioParameterInt* v1pan;
    juce::AudioParameterBool* v1solo;
    juce::AudioParameterBool* v1mute;
    
    // v2
    juce::AudioParameterInt* v2gain;
    juce::AudioParameterInt* v2pan;
    juce::AudioParameterBool* v2solo;
    juce::AudioParameterBool* v2mute;
    
    // v3
    juce::AudioParameterInt* v3gain;
    juce::AudioParameterInt* v3pan;
    juce::AudioParameterBool* v3solo;
    juce::AudioParameterBool* v3mute;
    
    // v4
    juce::AudioParameterInt* v4gain;
    juce::AudioParameterInt* v4pan;
    juce::AudioParameterBool* v4solo;
    juce::AudioParameterBool* v4mute;
    
    juce::AudioProcessorValueTreeState treeState;
    
    // my input buffer
    int inputBufferLength = 2 * fftSize;
    int inputBufferWrite;
    juce::AudioSampleBuffer inputBuffer;
    
    // shifting (affected by midi event)
    int shifts[numVoices];
    bool voiceIsActive[numVoices];
    int activeVoices;
    
    
    // fft tools
    juce::dsp::FFT fft;
    juce::HeapBlock<float> window;
    float windowScaleFactor;
    int hopCount = 0;
    
    // a whole bunch of lists to track sampled info for each voice
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomain; // Initial time domain is same for everyone (based on input voice)
    
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomainReturn1;
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomainReturn2;
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomainReturn3;
    juce::HeapBlock<juce::dsp::Complex<float>> fftTimeDomainReturn4;
    
    juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain;
    juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain1;
    juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain2;
    juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain3;
    juce::HeapBlock<juce::dsp::Complex<float>> fftFrequencyDomain4;
    
    // analysis same for all voices again
    juce::HeapBlock<float> analysisFrequencies;
    juce::HeapBlock<float> analysisMagnitudes;
    
    juce::HeapBlock<float> synthesisFrequencies;
    juce::HeapBlock<float> synthesisMagnitudes;
    juce::HeapBlock<float> synthesisFrequencies1;
    juce::HeapBlock<float> synthesisMagnitudes1;
    juce::HeapBlock<float> synthesisFrequencies2;
    juce::HeapBlock<float> synthesisMagnitudes2;
    juce::HeapBlock<float> synthesisFrequencies3;
    juce::HeapBlock<float> synthesisMagnitudes3;
    juce::HeapBlock<float> synthesisFrequencies4;
    juce::HeapBlock<float> synthesisMagnitudes4;
    
    juce::AudioSampleBuffer lastInputPhases;
    
    juce::AudioSampleBuffer lastOutputPhases;
    juce::AudioSampleBuffer lastOutputPhases1;
    juce::AudioSampleBuffer lastOutputPhases2;
    juce::AudioSampleBuffer lastOutputPhases3;
    juce::AudioSampleBuffer lastOutputPhases4;
    
    // data compiles here BEFORE sending to SYSTEM audio buffer
    int overlapOutputLength = (int)floorf((float)fftSize / maxRatio);
    int overlapOutputWrite; // saving the next block of processed audio
    int overlapOutputRead; // read -> write to system
    juce::AudioSampleBuffer overlapOutputBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
