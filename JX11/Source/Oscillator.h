/*
  ==============================================================================

    Oscillator.h
    Created: 2 Jan 2024 5:30:58pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

const float TWO_PI = 6.2831853071795864f;

class Oscillator
{
public:
    float amp;
    float freq;
    float sampleRate;
    float phaseOffset;
    int sampleIndex;
    
    void reset()
    {
        sampleIndex = 0;
    }
    
    float nextSample()
    {
        float output = amp * std::sin(
                                      TWO_PI * sampleIndex * freq
                                      / sampleRate + phaseOffset);
        sampleIndex += 1;
        return output;
    }
};
