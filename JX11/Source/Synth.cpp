/*
  ==============================================================================

    Synth.cpp
    Created: 2 Jan 2024 2:36:56pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int /*samplesPerBlock*/)
{
    sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateRecources()
{
    
}

void Synth::reset()
{
    voice.reset();
    noiseGen.reset();
    pitchBend = 1.0f;
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    
    voice.osc1.period = voice.period * pitchBend;
    voice.osc2.period = voice.osc1.period * detune;
    
    for (int sample = 0; sample < sampleCount; ++sample) {
        float noise = noiseGen.nextValue() * noiseMix;
        
        float outputLeft = 0.0f;
        float outputRight = 0.0f;
        
        if (voice.env.isActive()) {
            float output = voice.render(noise);
            outputLeft += output * voice.panLeft;
            outputRight += output * voice.panRight;
        }
        
        if (outputBufferRight != nullptr) {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        } else {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
        }
    }
    
    if (!voice.env.isActive()) {
        voice.env.reset();
    }
    
    preventGoingDeaf(outputBufferLeft, sampleCount);
    preventGoingDeaf(outputBufferRight, sampleCount);
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0 & 0xF0) {
        // Note Off
        case 0x80:
            noteOff(data1 & 0x7f);
            break;
            
        // Note On
        case 0x90: {
            uint8_t note = data1 & 0x7F;
            uint8_t velo = data2 & 0x7F;
            if (velo > 0) {
                noteOn(note, velo);
            } else {
                noteOff(note);
            }
            break;
        }
            
        // Pitch Bend
        case 0xE0:
            pitchBend = std::exp(-0.000014102f * float(data1 + 128 * data2 - 8192));
            break;
    }
}

void Synth::noteOn(int note, int vel)
{
    voice.note = note;
    voice.updatePanning();

    float period = calcPeriod(note);
    voice.period = period;
    
    voice.osc1.amp = (vel / 127.0f) * 0.5f;
    voice.osc1.reset(); // remove if reset on trigger is undesireable.
    
    voice.osc2.amp = voice.osc1.amp * oscMix;
    voice.osc2.reset(); // remove if reset on trigger is undesireable.
    
    // Envelope
    Envelope& env = voice.env;
    env.attackMultiplier = envAttack;
    env.decayMultiplier = envDecay;
    env.sustainLevel = envSustain;
    env.releaseMultiplier = envRelease;
    env.attack();
    

}

void Synth::noteOff(int note)
{
    if (voice.note == note) {
        voice.release();
    }
}

float Synth::calcPeriod(int note) const
{
    float period = tune * std::exp(-0.05776226505f* float(note));
    
    // drops synth an octave if over 7350 Hz.
    while (period < 6.0 || (period * detune) < 6.0) { period += period; }
    
    return period;
}
