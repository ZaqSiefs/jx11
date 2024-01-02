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
    
    void reset()
    {
        note = 0;
        osc.reset();
    }
    
    float render()
    {
        return osc.nextSample();
    }
};
