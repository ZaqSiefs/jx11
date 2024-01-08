/*
  ==============================================================================

    Synth.cpp
    Created: 2 Jan 2024 2:36:56pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

static const float ANALOG = 0.002f;
static const float SUSTAIN = -1;

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
    for(int v = 0; v < MAX_VOICES; ++v) {
        voices[v].reset();
    }
    
    noiseGen.reset();
    pitchBend = 1.0f;
    sustainPedalPressed = false;
    outputLevelSmoother.reset(sampleRate, 0.05);
}

void Synth::render(float** outputBuffers, int sampleCount)
{
    float* outputBufferLeft = outputBuffers[0];
    float* outputBufferRight = outputBuffers[1];
    
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (voice.env.isActive()) {
            voice.osc1.period = voice.period * pitchBend;
            voice.osc2.period = voice.osc1.period * detune;
        }
    }
    
    for (int sample = 0; sample < sampleCount; ++sample) {
        float noise = noiseGen.nextValue() * noiseMix;
        
        float outputLeft = 0.0f;
        float outputRight = 0.0f;
        
        for (int v = 0; v < MAX_VOICES; ++v) {
            Voice& voice = voices[v];
            if (voice.env.isActive()) {
                float output = voice.render(noise);
                outputLeft += output * voice.panLeft;
                outputRight += output * voice.panRight;
                
                float outputLevel = outputLevelSmoother.getNextValue();
                outputLeft *= outputLevel;
                outputRight *= outputLevel;
            }
        }
        
        if (outputBufferRight != nullptr) {
            outputBufferLeft[sample] = outputLeft;
            outputBufferRight[sample] = outputRight;
        } else {
            outputBufferLeft[sample] = (outputLeft + outputRight) * 0.5f;
        }
    }
    
    for (int v = 0; v < MAX_VOICES; ++v) {
        Voice& voice = voices[v];
        if (!voice.env.isActive()) {
            voice.env.reset();
        }
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
            
        // Sustain Pedal
        case 0xB0:
            controlChange(data1, data2);
            break;
    }
}

void Synth::startVoice(int v, int note, int vel)
{
    float period = calcPeriod(v, note);
    
    Voice& voice = voices[v];
    voice.period = period;
    voice.note = note;
    voice.updatePanning();
    
//    float velCurve = 0.004f * float((vel + 64) * (vel + 64)) - 8.0f; // Original JX11
    float velCurve = float(vel * vel / 127.0f); // Official MIDI Association Curve;
    voice.osc1.amp = volumeTrim * velCurve;
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

void Synth::noteOn(int note, int velocity)
{
    if (ignoreVelocity) { velocity = 80; }
    
    int v = 0;
    
    if (numVoices == 1) {
        if (voices[0].note > 0) {
            shiftQueuedNotes();
            restartMonoVoice(note, velocity);
            return;
        }
    } else {
        v = findFreeVoice();
    }
        
    startVoice(v, note, velocity);
}


void Synth::noteOff(int note)
{
    if ((numVoices == 1) && (voices[0].note == note)) {
        int queuedNote = nextQueuedNote();
        if (queuedNote > 0) {
            restartMonoVoice(queuedNote, -1);
        }
    }
    
    for (int v = 0; v < MAX_VOICES; v++) {
        if (voices[v].note == note) {
            if (sustainPedalPressed) {
                voices[v].note = SUSTAIN;
            } else {
                voices[v].release();
                voices[v].note = 0;
            }
        }
    }
}

float Synth::calcPeriod(int v, int note) const
{
    float period = tune * std::exp(-0.05776226505f* (float(note) + ANALOG * float(v)));
    
    // drops synth an octave if over 7350 Hz.
    while (period < 6.0 || (period * detune) < 6.0) { period += period; }
    
    return period;
}

int Synth::findFreeVoice() const
{
    int v = 0;
    float l = 100.0f;
    
    for (int i = 0; i < MAX_VOICES; ++i) {
        if (voices[i].env.level < l && !voices[i].env.isInAttack()) {
            l = voices[i].env.level;
            v = i;
        }
    }
    return v;
}

void Synth::controlChange(uint8_t data1, uint8_t data2)
{
    switch (data1) {
        case 0x40:
            sustainPedalPressed = (data2 >= 64);
            
            if (!sustainPedalPressed) {
                noteOff(SUSTAIN);
            }
            break;
            
        default:
            if (data1 >= 0x78) {
                for (int v = 0; v < MAX_VOICES; ++v) {
                    voices[v].reset();
                }
                sustainPedalPressed = false;
            }
            break;
    }
}

void Synth::restartMonoVoice(int note, int velocity)
{
    float period = calcPeriod(0, note);
    
    Voice& voice = voices[0];
    voice.period = period;
    
    voice.env.level += SILENCE + SILENCE;
    voice.note = note;
    voice.updatePanning();
}

void Synth::shiftQueuedNotes()
{
    for (int tmp = MAX_VOICES - 1; tmp > 0; tmp--) {
        voices[tmp].note = voices[tmp - 1].note;
        voices[tmp].release();
    }
}

int Synth::nextQueuedNote()
{
    int held = 0;
    for (int v = MAX_VOICES - 1; v > 0; v--) {
        if (voices[v].note > 0) {held = v;}
    }
    
    if (held > 0) {
        int note = voices[held].note;
        voices[held].note = 0;
        return note;
    }
    
    return 0;
}
