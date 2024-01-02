/*
  ==============================================================================

    Voice.h
    Created: 2 Jan 2024 2:37:27pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

struct Voice
{
    int note;
    int velocity;
    
    void reset()
    {
        note = 0;
        velocity = 0;
    }
    
};
