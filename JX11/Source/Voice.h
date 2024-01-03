/*
  ==============================================================================

    Voice.h
    Created: 2 Jan 2024 2:37:27pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

#include "Oscillator.h"

struct Voice
{
    int note;
    Oscillator osc;
    float saw;
    
    void reset()
    {
        note = 0;
        osc.reset();
        saw = 0.0f;
    }
    
    float render()
    {
        float sample = osc.nextSample();
        saw = saw * 0.997f + sample;
        return saw;
    }
};
