/*
  ==============================================================================

    Envelope.h
    Created: 3 Jan 2024 2:38:12pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

const float SILENCE = 0.0001f;

class Envelope
{
public:
    float nextValue()
    {
        level *= multiplier;
        return level;
    }
    
    float level;
    float multiplier;
};
