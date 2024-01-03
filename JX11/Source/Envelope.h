/*
  ==============================================================================

    Envelope.h
    Created: 3 Jan 2024 2:38:12pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

class Envelope 
{
public:
    float nextValue()
    {
        level *= 0.9999f;
        return level;
    }
    
    float level;
};
