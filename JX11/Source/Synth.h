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
    
    void allocateResources(double sampleRate, int samplesPerBlock);
    void deallocateRecources();
    void reset();
    void render(float** outputBuffers, int sampleCount);
    void midiMessage(uint8_t data0, uint8_t data1, uint8_t data2);
    
private:
    float sampleRate;
    Voice voice;
    void noteOn(int note, int velocity);
    void noteOff(int note);
    NoiseGenerator noiseGen;
};
