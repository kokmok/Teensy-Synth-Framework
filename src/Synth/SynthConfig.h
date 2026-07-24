#pragma once
#include "Parameters/WaveformParameter.h"

struct SynthDescriptor {
    // Un paramètre par oscillateur (pointeurs → permet le partage)
    WaveformParameter* oscWaveform[OSC_PER_VOICE] = {};

    // Réglages runtime
    float mixerGain = 0.25f;
    float attack    = 5.0f;
    float decay     = 50.0f;
    float sustain   = 0.7f;
    float release   = 200.0f;
};