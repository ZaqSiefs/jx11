/*
  ==============================================================================

    Synth.h
    Created: 2 Jan 2024 2:36:56pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Voice.h"
#include "NoiseGenerator.h"

class Synth
{
public:
    Synth();
    
    static constexpr int MAX_VOICES = 8;
    int numVoices;
    
    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateResources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void startVoice(int v, int note, int velocity);
    
    
    // Params
    
    float noiseMix;
    float envAttack;
    float envDecay;
    float envSustain;
    float envRelease;
    float oscMix;
    float detune;
    float tune;
    float volumeTrim;
    juce::LinearSmoothedValue<float> outputLevelSmoother;
    
private:
    void noteOn(int note, int velocity);
    void noteOff(int note);
    float calcPeriod(int v, int note) const;
    int findFreeVoice() const;
    void controlChange(uint8_t data1, uint8_t data2);
    
    float sampleRate;
    float pitchBend;
    bool sustainPedalPressed;
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
};
