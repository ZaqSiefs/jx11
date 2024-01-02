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
    float inc;
    float phase;
    
    void reset()
    {
        phase = 0.0f;
    }
    
    float nextSample()
    {
        phase += inc;
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
        return amp * std::sin(TWO_PI * phase);
    }
};
