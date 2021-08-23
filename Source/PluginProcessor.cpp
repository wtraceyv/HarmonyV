/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "FrequencyMap.cpp"
#include "NoteUtil.cpp"

#include <iostream>


using namespace std;

//==============================================================================

// custom class for component to display current pitch detected
class RealtimeLabelComponent : public juce::Component, private juce::Timer
{
public:
    RealtimeLabelComponent (NewProjectAudioProcessor& processorToUse) : processor(processorToUse)
    {
        setOpaque(false);
        startTimerHz(30);
    }
    
    void paint (juce::Graphics& g) override
    {
        auto detectedFrequency = processor.curDetectedFrequency;
        auto noteName = processor.curDetectedNoteName;
        
        g.setColour (juce::Colours::white);
        g.drawFittedText("Freq: " + juce::String(detectedFrequency) + "\n" +
                         "Note: " + juce::String(noteName), getLocalBounds(), juce::Justification::centred, 3);
    }
    
private:
    void timerCallback() override
    {
        repaint();
    }
    
    NewProjectAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RealtimeLabelComponent)
};

// have to wrap above class here to actually get it displayed
class RealtimeLabelComponentItem : public foleys::GuiItem
{
public:
    FOLEYS_DECLARE_GUI_FACTORY(RealtimeLabelComponentItem)
    
    RealtimeLabelComponentItem (foleys::MagicGUIBuilder& builder, const juce::ValueTree& node) : foleys::GuiItem (builder, node)
    {
        if (auto* proc = dynamic_cast<NewProjectAudioProcessor*>(builder.getMagicState().getProcessor()))
        {
            info = std::make_unique<RealtimeLabelComponent>(*proc);
            addAndMakeVisible(info.get());
        }
    }
    
    void update() override {}
    
    juce::Component* getWrappedComponent() override { return info.get();
        
    }
private:
    std::unique_ptr<RealtimeLabelComponent> info;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RealtimeLabelComponentItem)
};

void NewProjectAudioProcessor::initialiseBuilder(foleys::MagicGUIBuilder &builder)
{
    builder.registerJUCEFactories();
    
    builder.registerFactory("RealtimeLabel", &RealtimeLabelComponentItem::factory);
}

// for parameters like choices that need some extra machinery to react properly
// (Audio parameter ints/floats I can access directly on the fly)
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    auto options = std::make_unique<juce::AudioProcessorParameterGroup>("Options", TRANS("OPTIONS"), "|");
    
    return layout;
}

//==============================================================================

// PLUGIN CONSTRUCTOR
NewProjectAudioProcessor::NewProjectAudioProcessor() : foleys::MagicProcessor (BusesProperties()
                               .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                               .withOutput("Original Voice", juce::AudioChannelSet::stereo(),true)
                       ),
        treeState(*this, nullptr, "PARAMETERS", createParameterLayout()),
        fft(fftOrder)
{
    /* * gui params * */
    
    // input channel:
    addParameter(inputGain = new juce::AudioParameterInt ("inputgain", "In Gain", -12, 6, 0));
    addParameter(inOutputGain = new juce::AudioParameterInt ("inoutputgain", "Out Gain", -12, 6, 0));
    addParameter(inputPan = new juce::AudioParameterInt ("inputpan", "Pan", -64, 64, 0));
    addParameter(inputSolo = new juce::AudioParameterBool ("inputsolo", "Solo", false));
    addParameter(inputMute = new juce::AudioParameterBool ("inputmute", "Mute", false));
    
    //v1
    addParameter(v1gain = new juce::AudioParameterInt ("v1gain", "Gain", -12, 6, 0));
    addParameter(v1pan = new juce::AudioParameterInt ("v1pan", "Pan", -64, 64, 0));
    addParameter(v1solo = new juce::AudioParameterBool ("v1solo", "Solo", false));
    addParameter(v1mute = new juce::AudioParameterBool ("v1mute", "Mute", false));
    //v2
    addParameter(v2gain = new juce::AudioParameterInt ("v2gain", "Gain", -12, 6, 0));
    addParameter(v2pan = new juce::AudioParameterInt ("v2pan", "Pan", -64, 64, 0));
    addParameter(v2solo = new juce::AudioParameterBool ("v2solo", "Solo", false));
    addParameter(v2mute = new juce::AudioParameterBool ("v2mute", "Mute", false));
    //v3
    addParameter(v3gain = new juce::AudioParameterInt ("v3gain", "Gain", -12, 6, 0));
    addParameter(v3pan = new juce::AudioParameterInt ("v3pan", "Pan", -64, 64, 0));
    addParameter(v3solo = new juce::AudioParameterBool ("v3solo", "Solo", false));
    addParameter(v3mute = new juce::AudioParameterBool ("v3mute", "Mute", false));
    //v4
    addParameter(v4gain = new juce::AudioParameterInt ("v4gain", "Gain", -12, 6, 0));
    addParameter(v4pan = new juce::AudioParameterInt ("v4pan", "Pan", -64, 64, 0));
    addParameter(v4solo = new juce::AudioParameterBool ("v4solo", "Solo", false));
    addParameter(v4mute = new juce::AudioParameterBool ("v4mute", "Mute", false));
    
    // bake foleys gui
    magicState.setGuiValueTree(BinaryData::harmonygui_xml, BinaryData::harmonygui_xmlSize);
    
    // fill window (hann)
    window.realloc(fftSize);
    for (int sample = 0; sample < fftSize; ++sample)
        window[sample] = 0.5f * (1.0f - cosf(2.0 * M_PI * sample / (float)(fftSize - 1)));
    float windowSum = 0.0f;
    for (int i  = 0; i < fftSize; i++)
        windowSum += window[i];
    windowScaleFactor = 1.0f / 4.0f / windowSum * (float)fftSize;
    
    clearBuffers();
    overlapOutputWrite = fftSize + 2 * hopSize; // inherent latency
    
    for (int voice = 0; voice < 4; voice++) {
        voiceIsActive[voice] = false;
    }
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

void NewProjectAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    
}

void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();
    
    // track detected pitch values
    static float maxBinValue = 0;
    static int maxBinIndex = 0;
    
    // DEBUG
    static int blockCount = 0;
    const int blocksWaitBeforeUpdate = 40;
    blockCount++;
    
    //=========================  MIDI  =================================//
    
    // (1) keep track of how many voices should be active + intended notes
    // (2) communicate when a voice should end because midi release (off note)
    // analysisFrequencies[maxBinIndex] * 44100.0 / fftSize = detected pitch, we thinks
    for (juce::MidiMessageMetadata note : midiMessages)
    {
        int num = note.getMessage().getNoteNumber();
        if (note.getMessage().isNoteOn() && activeVoices < 4)
        {
            int noteFind = findMidiNote(curDetectedFrequency, 3.0f);
            cout << "Frequency going against.." << curDetectedFrequency << endl;
            int shiftToGo = num - noteFind;
            if (noteFind == -1)
                shiftToGo = 12;
            activeVoices++;
            shifts[activeVoices - 1] = shiftToGo;
            voiceIsActive[activeVoices - 1] = true;
        }
        else if (note.getMessage().isNoteOff() && activeVoices > 0 && voiceIsActive[activeVoices - 1])
        {
            activeVoices--;
            voiceIsActive[activeVoices] = false;
            shifts[activeVoices] = 0;
        }
        cout << "active voices: " << activeVoices << endl;
    }
    
    //=========================  AUDIO  =================================//
    // track processed samples
    int curInWrite;
    int curOutWrite;
    int curOutRead;
    int curHopCount;
    
    // TODO shifting vars
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float *systemWrite = buffer.getWritePointer(channel);
        curInWrite = inputBufferWrite;
        curOutWrite = overlapOutputWrite;
        curOutRead = overlapOutputRead;
        curHopCount = hopCount;
        
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // set up reading from system input and write the last computed sample(s)
            const float in = systemWrite[sample];
            systemWrite[sample] = (in * 0.5f * genPanRatio(-1, channel)) + overlapOutputBuffer.getSample(channel, curOutRead);
            
            overlapOutputBuffer.setSample(channel, curOutRead, 0.0f);
            if (++curOutRead >= overlapOutputLength)
                curOutRead = 0;
            
            inputBuffer.setSample(channel, curInWrite, in);
            if (++curInWrite >= inputBufferLength)
                curInWrite = 0;

            // do an fft when hop size is reached
            if (++curHopCount >= hopSize)
            {
                curHopCount = 0;
                
                float oldx = 0;
                float x = 0;
                int oldZeroCross = 0;
                int inptr = 0;
                
                // apply hann window (analysis)
                int inBufferIndex = curInWrite;
                for (int index = 0; index < fftSize; ++index)
                {
                    fftTimeDomain[index].real(sqrtf(window[index]) * inputBuffer.getSample(channel, inBufferIndex));
                    fftTimeDomain[index].imag(0.0f);
                    
                    // trying to do pitch detection by zero crossing
                    oldx = x;
                    x = inputBuffer.getSample(channel, inBufferIndex);
                    inptr++;
                    if (oldx > 0 && x <= 0)
                    {
                        int periodLength = inptr - oldZeroCross;
                        curDetectedFrequency = (1 / (float)periodLength) * 44100;
                        curDetectedMidiNote = findMidiNote(curDetectedFrequency, 3.0f);
                        curDetectedNoteName = getNoteFromMidi(curDetectedMidiNote);
                        oldZeroCross = inptr;
                    }
                    
                    if (++inBufferIndex >= inputBufferLength)
                        inBufferIndex = 0;
                }
                
                // forward fft to frequency domain
                fft.perform(fftTimeDomain, fftFrequencyDomain, false);
                
                // TODO reset phases in any case?
                
                // frequency domain analysis
//                for (int bin = 0; bin <= fftSize / 2; ++bin)
//                {
//                    float amplitude = abs (fftFrequencyDomain[bin]);
//                    float phase = arg (fftFrequencyDomain[bin]);
//
//                    // calculate other interesting quantities based on phase
//                    float phaseDiff = phase - lastInputPhases.getSample(channel, bin);
//                    float binCentreFreq = 2.0f * M_PI * (float)bin / (float)fftSize;
//                    phaseDiff = wrapPhase(phaseDiff - binCentreFreq * hopSize);
//                    float binDeviation = phaseDiff * (float)fftSize / (float)hopSize / (2.0 * M_PI); // (fractional bin number)
//
//                    // save analysed values
//                    analysisFrequencies[bin] = (float)bin + binDeviation;
//                    analysisMagnitudes[bin] = amplitude;
//                    lastInputPhases.setSample(channel, bin, phase);
//
//                    // save for pitch detection
//                    if (amplitude > maxBinValue) {
//                        maxBinValue = amplitude;
//                        maxBinIndex = bin;
//                    }
//
//                    clearSynthesisBin(bin);
//                }
                
                // send a frequency infos out
//                if (blockCount % 8 == 0){
//                    curDetectedFrequency = analysisFrequencies[maxBinIndex] * 44100.0 / fftSize;
//                    curDetectedMidiNote = findMidiNote(curDetectedFrequency, 3.0f);
//                    curDetectedNoteName = getNoteNameFromMidi(curDetectedMidiNote);
//                    blockCount = 0;
//                }
                
                // frequency domain manipulation:
                // shift pitch
                for (int bin = 0; bin <= fftSize / 2; bin++)
                {
                    for (int voice = 0; voice < 4; voice++)
                    {
                        if (!voiceIsActive[voice])
                            continue;
                        float ratio = genRatio(shifts[voice]);
                        int newBin = floorf(bin * ratio + 0.5f); // 'nearest bin to shifted freq'
                        if (newBin <= fftSize / 2) // ignore above nyquist
                        {
                            shiftSynthesisBins(voice, newBin, bin, ratio);
                        }
                    }
                }
                
                // synthesize frequencies
                for (int bin = 0; bin <= fftSize / 2; bin++)
                {
                    for (int voice = 0; voice < 4; voice++)
                    {
                        if (!voiceIsActive[voice])
                            continue;
                        float amplitude = getSynthesisMagnitude(voice, bin);
                        float outPhase = genOutPhase(getSynthesisFrequency(voice, bin), bin, getOutputPhase(voice, channel, bin));
                        setNewFreqDomainUniques(voice, bin, amplitude, outPhase);
                        if (bin > 0 && bin < fftSize / 2)
                            storeFreqDomainConjugates(voice, bin);
                        // save for next hop
                        setOutPhases(voice, channel, bin, outPhase);
                    }
                }
                 
                // inverse ffts back to time domain
                for (int voice = 0; voice < 4; voice++)
                    if (voiceIsActive[voice])
                        fftSelectiveInverse(voice);
             
                // copy new time domains to my output
                int outBufferIndex = curOutWrite;
                for (int n = 0; n < fftSize; ++n)
                {
                    float out = overlapOutputBuffer.getSample(channel, outBufferIndex);
                    for (int voice = 0; voice < 4; voice++) {
                        if (voiceIsActive[voice])
                            out += getTimeOutputReal(voice, n) * window[n] * windowScaleFactor * genPanRatio(voice, channel);
                    }
                    overlapOutputBuffer.setSample(channel, outBufferIndex, out);
                    
                    if (++outBufferIndex >= overlapOutputLength)
                        outBufferIndex = 0;
                }
                
                // advance output writing by a hop,
                // since we just processed an fft
                curOutWrite += hopSize;
                if (curOutWrite >= overlapOutputLength)
                    curOutWrite = 0;
                
            } // end FFT processing
             
        } // end process each sample
    } // end process each channel
    
    inputBufferWrite = curInWrite;
    overlapOutputWrite = curOutWrite;
    overlapOutputRead = curOutRead;
    hopCount = curHopCount;
    
    // clear left over in buffer
    for (int channel = totalNumInputChannels; channel < totalNumOutputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);
}

//======================== HELPERS =============================

// phase at some point needs to be constrained to (-pi, pi)
float NewProjectAudioProcessor::wrapPhase(const float phase)
{
    if (phase >= 0.0f)
        return fmod(phase + M_PI, 2.0f * M_PI) - M_PI;
    else
        return fmod(phase + M_PI, -2.0f * M_PI) + M_PI;
}

/**
 * Get the 'percentage' of this signal that should go to this channel based on its pan setting.
 * Since we're only expecting stereo output, you can just supply 'channel' in processBlock to isRightChannel
 */
float NewProjectAudioProcessor::genPanRatio(int voice, int isRightChannel)
{
    switch (voice) {
        case -1:
            if (isRightChannel){
                return fminf(1 + *inputPan / 64.0f, 1.0);
            } else {
                return fminf(1 - *inputPan / 64.0f, 1.0);
            }
            break;
        case 0:
            if (isRightChannel){
                return fminf(1 + *v1pan / 64.0f, 1.0);
            } else {
                return fminf(1 - *v1pan / 64.0f, 1.0);
            }
            break;
        case 1:
            if (isRightChannel){
                return fminf(1 + *v2pan / 64.0f, 1.0);
            } else {
                return fminf(1 - *v2pan / 64.0f, 1.0);
            }
            break;
        case 2:
            if (isRightChannel){
                return fminf(1 + *v3pan / 64.0f, 1.0);
            } else {
                return fminf(1 - *v3pan / 64.0f, 1.0);
            }
            break;
        case 3:
            if (isRightChannel){
                return fminf(1 + *v4pan / 64.0f, 1.0);
            } else {
                return fminf(1 - *v4pan / 64.0f, 1.0);
            }
            break;
        default:
            return 0.0f;
    }
}

float NewProjectAudioProcessor::genRatio(const int shift)
{
    return powf(2.0f, shift / 12.0);
}

int NewProjectAudioProcessor::genResampleLength(float ratio)
{
    return floorf((float)fftSize / ratio);
}

// declutter synthesis processing
float NewProjectAudioProcessor::genOutPhase(float synthFreq, int bin, float lastOutputPhase)
{
    float binDeviation = synthFreq - bin;
    float phaseDiff = binDeviation * 2.0 * M_PI * (float)hopSize / (float)fftSize;
    float binCentreFrequency = 2.0 * M_PI * (float)bin / (float)fftSize;
    phaseDiff += binCentreFrequency * hopSize;
    return wrapPhase(lastOutputPhase + phaseDiff);
}

// clear buffers/make sure they have enough memory allocated
// ..which is a lot
void NewProjectAudioProcessor::clearBuffers()
{
    inputBufferWrite = 0;
    inputBuffer.clear();
    inputBuffer.setSize(getTotalNumInputChannels(), inputBufferLength);
    
    overlapOutputRead = 0;
    overlapOutputWrite = 0;
    overlapOutputBuffer.clear();
    overlapOutputBuffer.setSize(getTotalNumInputChannels(), overlapOutputLength);
    
    // this is for input
    fftTimeDomain.realloc(fftSize);
    fftTimeDomain.clear(fftSize);
    
    fftTimeDomainReturn1.realloc(fftSize);
    fftTimeDomainReturn1.clear(fftSize);
    fftTimeDomainReturn2.realloc(fftSize);
    fftTimeDomainReturn2.clear(fftSize);
    fftTimeDomainReturn3.realloc(fftSize);
    fftTimeDomainReturn3.clear(fftSize);
    fftTimeDomainReturn4.realloc(fftSize);
    fftTimeDomainReturn4.clear(fftSize);
    
    fftFrequencyDomain.realloc(fftSize); // can probably remove after proof of concept
    fftFrequencyDomain.clear(fftSize);
    
    fftFrequencyDomain1.realloc(fftSize);
    fftFrequencyDomain1.clear(fftSize);
    fftFrequencyDomain2.realloc(fftSize);
    fftFrequencyDomain2.clear(fftSize);
    fftFrequencyDomain3.realloc(fftSize);
    fftFrequencyDomain3.clear(fftSize);
    fftFrequencyDomain4.realloc(fftSize);
    fftFrequencyDomain4.clear(fftSize);
    
    analysisMagnitudes.realloc(fftSize / 2 + 1);
    analysisMagnitudes.clear(fftSize / 2 + 1);
    analysisFrequencies.realloc(fftSize / 2 + 1);
    analysisFrequencies.clear(fftSize / 2 + 1);
    
    synthesisMagnitudes1.realloc(fftSize / 2 + 1);
    synthesisMagnitudes1.clear(fftSize / 2 + 1);
    synthesisFrequencies1.realloc(fftSize / 2 + 1);
    synthesisFrequencies1.clear(fftSize / 2 + 1);
    synthesisMagnitudes2.realloc(fftSize / 2 + 1);
    synthesisMagnitudes2.clear(fftSize / 2 + 1);
    synthesisFrequencies2.realloc(fftSize / 2 + 1);
    synthesisFrequencies2.clear(fftSize / 2 + 1);
    synthesisMagnitudes3.realloc(fftSize / 2 + 1);
    synthesisMagnitudes3.clear(fftSize / 2 + 1);
    synthesisFrequencies3.realloc(fftSize / 2 + 1);
    synthesisFrequencies3.clear(fftSize / 2 + 1);
    synthesisMagnitudes4.realloc(fftSize / 2 + 1);
    synthesisMagnitudes4.clear(fftSize / 2 + 1);
    synthesisFrequencies4.realloc(fftSize / 2 + 1);
    synthesisFrequencies4.clear(fftSize / 2 + 1);
    
    lastInputPhases.setSize(getTotalNumInputChannels(), overlapOutputLength);
    lastInputPhases.clear();
    
    lastOutputPhases.setSize(getTotalNumInputChannels(), overlapOutputLength); // probably remove after proof of concept
    lastOutputPhases.clear();
    lastOutputPhases1.setSize(getTotalNumInputChannels(), overlapOutputLength);
    lastOutputPhases1.clear();
    lastOutputPhases2.setSize(getTotalNumInputChannels(), overlapOutputLength);
    lastOutputPhases2.clear();
    lastOutputPhases3.setSize(getTotalNumInputChannels(), overlapOutputLength);
    lastOutputPhases3.clear();
    lastOutputPhases4.setSize(getTotalNumInputChannels(), overlapOutputLength);
    lastOutputPhases4.clear();
    
    hopCount = 0;
}

// TODO decide if this is necessary/how often?
void NewProjectAudioProcessor::clearPhases()
{
    lastOutputPhases.clear();
    lastOutputPhases1.clear();
    lastOutputPhases2.clear();
    lastOutputPhases3.clear();
    lastOutputPhases4.clear();
}

void NewProjectAudioProcessor::clearSynthesisBin(int bin)
{
    synthesisMagnitudes1[bin] = 0.0f;
    synthesisFrequencies1[bin] = 0.0f;
    synthesisMagnitudes2[bin] = 0.0f;
    synthesisFrequencies2[bin] = 0.0f;
    synthesisMagnitudes3[bin] = 0.0f;
    synthesisFrequencies3[bin] = 0.0f;
    synthesisMagnitudes4[bin] = 0.0f;
    synthesisFrequencies4[bin] = 0.0f;
}

/**
 * Perform inverse FFT for given voice;
 * really want to minimize/optimize this operation as much as possible.
 */
void NewProjectAudioProcessor::fftSelectiveInverse(int voice) // TODO any way to nicely multithread this?
{
    switch (voice) {
        case 0:
            fft.perform(fftFrequencyDomain1, fftTimeDomainReturn1, true);
            break;
        case 1:
            fft.perform(fftFrequencyDomain2, fftTimeDomainReturn2, true);
            break;
        case 2:
            fft.perform(fftFrequencyDomain3, fftTimeDomainReturn3, true);
            break;
        case 3:
            fft.perform(fftFrequencyDomain4, fftTimeDomainReturn4, true);
            break;
            
        default:
            break;
    }
}

/* * * getters, but for individual voices. -- maybe better done with lists of pointers to these objects? * * */

float NewProjectAudioProcessor::getTimeOutputReal(int voice, int sample)
{
    switch (voice) {
        case 0:
            return fftTimeDomainReturn1[sample].real();
            break;
        case 1:
            return fftTimeDomainReturn2[sample].real();
            break;
        case 2:
            return fftTimeDomainReturn3[sample].real();
            break;
        case 3:
            return fftTimeDomainReturn4[sample].real();
            break;
        default:
            break;
    }
    return 0.0f;
}

float NewProjectAudioProcessor::getOutputPhase(int voice, int channel, int bin)
{
    switch (voice) {
        case 0:
            return lastOutputPhases1.getSample(channel, bin);
            break;
        case 1:
            return lastOutputPhases2.getSample(channel, bin);
            break;
        case 2:
            return lastOutputPhases3.getSample(channel, bin);
            break;
        case 3:
            return lastOutputPhases4.getSample(channel, bin);
            break;
        default:
            break;
    }
    return 0.0f;
}

float NewProjectAudioProcessor::getSynthesisFrequency(int voice, int bin)
{
    switch (voice) {
        case 0:
            return synthesisFrequencies1[bin];
            break;
        case 1:
            return synthesisFrequencies2[bin];
            break;
        case 2:
            return synthesisFrequencies3[bin];
            break;
        case 3:
            return synthesisFrequencies4[bin];
            break;
        default:
            break;
    }
    return 0.0f;
}

float NewProjectAudioProcessor::getSynthesisMagnitude(int voice, int bin)
{
    switch (voice) {
        case 0:
            return synthesisMagnitudes1[bin];
            break;
        case 1:
            return synthesisMagnitudes2[bin];
            break;
        case 2:
            return synthesisMagnitudes3[bin];
            break;
        case 3:
            return synthesisMagnitudes4[bin];
            break;
        default:
            break;
    }
    return 0.0f;
}

/* * * Declutter some operations in the main loop by funneling them by voice
 * * * (and also avoid doing calculations on unused voices) * * */

void NewProjectAudioProcessor::setNewFreqDomainUniques(int voice, int bin, float amplitude, float outPhase)
{
    switch (voice) {
        case 0:
            fftFrequencyDomain1[bin].real(amplitude * cosf(outPhase));
            fftFrequencyDomain1[bin].imag(amplitude * sinf(outPhase));
            break;
        case 1:
            fftFrequencyDomain2[bin].real(amplitude * cosf(outPhase));
            fftFrequencyDomain2[bin].imag(amplitude * sinf(outPhase));
            break;
        case 2:
            fftFrequencyDomain3[bin].real(amplitude * cosf(outPhase));
            fftFrequencyDomain3[bin].imag(amplitude * sinf(outPhase));
            break;
        case 3:
            fftFrequencyDomain4[bin].real(amplitude * cosf(outPhase));
            fftFrequencyDomain4[bin].imag(amplitude * sinf(outPhase));
            break;
        default:
            break;
    }
}

void NewProjectAudioProcessor::shiftSynthesisBins(int voice, int newBin, int origBin, float pitchRatio)
{
    switch (voice) {
        case 0:
            synthesisMagnitudes1[newBin] += analysisMagnitudes[origBin];
            synthesisFrequencies1[newBin] = analysisFrequencies[origBin] * pitchRatio;
            break;
        case 1:
            synthesisMagnitudes2[newBin] += analysisMagnitudes[origBin];
            synthesisFrequencies2[newBin] = analysisFrequencies[origBin] * pitchRatio;
            break;
        case 2:
            synthesisMagnitudes3[newBin] += analysisMagnitudes[origBin];
            synthesisFrequencies3[newBin] = analysisFrequencies[origBin] * pitchRatio;
            break;
        case 3:
            synthesisMagnitudes4[newBin] += analysisMagnitudes[origBin];
            synthesisFrequencies4[newBin] = analysisFrequencies[origBin] * pitchRatio;
            break;
        default:
            break;
    }
}

void NewProjectAudioProcessor::setOutPhases(int voice, int channel, int bin, float outPhase)
{
    switch (voice) {
        case 0:
            lastOutputPhases1.setSample(channel, bin, outPhase);
            break;
        case 1:
            lastOutputPhases2.setSample(channel, bin, outPhase);
            break;
        case 2:
            lastOutputPhases3.setSample(channel, bin, outPhase);
            break;
        case 3:
            lastOutputPhases4.setSample(channel, bin, outPhase);
            break;
            
        default:
            break;
    }
}

void NewProjectAudioProcessor::storeFreqDomainConjugates(int voice, int bin)
{
    switch (voice) {
        case 0:
            fftFrequencyDomain1[fftSize - bin].real(fftFrequencyDomain1[bin].real());
            fftFrequencyDomain1[fftSize - bin].imag(-fftFrequencyDomain1[bin].imag());
            break;
        case 1:
            fftFrequencyDomain2[fftSize - bin].real(fftFrequencyDomain2[bin].real());
            fftFrequencyDomain2[fftSize - bin].imag(-fftFrequencyDomain2[bin].imag());
            break;
        case 2:
            fftFrequencyDomain3[fftSize - bin].real(fftFrequencyDomain3[bin].real());
            fftFrequencyDomain3[fftSize - bin].imag(-fftFrequencyDomain3[bin].imag());
            break;
        case 3:
            fftFrequencyDomain4[fftSize - bin].real(fftFrequencyDomain4[bin].real());
            fftFrequencyDomain4[fftSize - bin].imag(-fftFrequencyDomain4[bin].imag());
            break;
        default:
            break;
    }
}

//====================JUCE STUFF===============================

const juce::String NewProjectAudioProcessor::getName() const { return JucePlugin_Name; }

bool NewProjectAudioProcessor::acceptsMidi() const { return true; }

bool NewProjectAudioProcessor::producesMidi() const { return false; }

bool NewProjectAudioProcessor::isMidiEffect() const{ return false; }

double NewProjectAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int NewProjectAudioProcessor::getNumPrograms(){ return 1; }

int NewProjectAudioProcessor::getCurrentProgram(){ return 0; }

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
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
    return new NewProjectAudioProcessor();
}
