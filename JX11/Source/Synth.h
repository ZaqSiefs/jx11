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
    void deallocateRecources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    void startVoice(int v, int note, int velocity);
    
    
    // Params
    const int LFO_MAX = 32;
    
    float noiseMix;
    float envAttack;
    float envDecay;
    float envSustain;
    float envRelease;
    float oscMix;
    float detune;
    float tune;
    float volumeTrim;
    float velocitySensitivity;
    float lfoInc;
    float vibrato;
    float pwmDepth;
    float modWheel;
    float glideRate;
    float glideBend;
    int glideMode;
    bool ignoreVelocity;
    juce::LinearSmoothedValue<float> outputLevelSmoother;
    
private:
    inline void updatePeriod(Voice& voice)
    {
        voice.osc1.period = voice.period * pitchBend;
        voice.osc2.period = voice.osc1.period * detune;
    }
    
    void noteOn(int note, int velocity);
    void noteOff(int note);
    void controlChange(uint8_t data1, uint8_t data2);
    void restartMonoVoice(int note, int velocity);
    void shiftQueuedNotes();
    void updateLFO();
    float calcPeriod(int v, int note) const;
    int nextQueuedNote();
    int findFreeVoice() const;
    bool isPlayingLegatoStyle() const;
    
    float sampleRate;
    float pitchBend;
    float lfo;
    int lfoStep;
    int lastNote;
    bool sustainPedalPressed;
    std::array<Voice, MAX_VOICES> voices;
    NoiseGenerator noiseGen;
};
