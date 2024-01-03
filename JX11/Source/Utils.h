/*
  ==============================================================================

    Utils.h
    Created: 2 Jan 2024 3:49:28pm
    Author:  Zaq Siefert

  ==============================================================================
*/

#pragma once

inline void preventGoingDeaf(float* buffer, int sampleCount)
{
    if (buffer == nullptr) {return;}
    bool firstWarning = true;
    for (int i = 0; i < sampleCount; ++i) {
        float x = buffer[i];
        bool silence = false;
        if (std::isnan(x)) {
            DBG("!!! WARNING: nan detected in audio buffer, silencing !!!");
            silence = true;
        } else if (std::isinf(x)) {
            DBG("!!! WARNING: inf detected in audio buffer, silencing !!!");
            silence = true;
        } else if (x < -2.0f || x > 2.0f) { //screaming feedback
            DBG("!!! WARNING: sample out of range, silencing !!!");
        } else if (x < -1.0f) {
            if (firstWarning) {
                DBG("!!! WARNING: SAMPLE OUT OF RANGE, clamping !!!");
                firstWarning = false;
            }
            buffer[i] = -1.0;
        } else if (x > 1.0f) {
            if (firstWarning) {
                DBG("!!! WARNING: SAMPLE OUT OF RANGE, clamping !!!");
                firstWarning = false;
            }
            buffer[i] = 1.0f;
        }
        if (silence) {
            memset(buffer, 0, sampleCount * sizeof(float));
            return;
        }
    }
}

template<typename T>
inline static void castParameter(juce::AudioProcessorValueTreeState& apvts,
                                 const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination); // Parameter does not exist or wrong type
}
