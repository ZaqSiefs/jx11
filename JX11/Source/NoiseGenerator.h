/*
  ==============================================================================

    NoiseGenerator.h
    Created: 2 Jan 2024 3:23:28pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

class NoiseGenerator
{
public:
    void reset()
    {
        noiseSeed = 22222;
    }
    
    float nextValue()
    {
        // generate the next int pseudorandom number.
        noiseSeed = noiseSeed * 196314165 + 907633515;
        
        // convert to a signed value.
        int temp = int(noiseSeed >> 7) - 16777216;
        
        // convert to a floating-point number between -1.0 and 1.0.
        return float(temp) / 16777216.0f;
    }
    
private:
    unsigned int noiseSeed;
};
