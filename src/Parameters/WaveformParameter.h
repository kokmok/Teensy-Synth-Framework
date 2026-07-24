#pragma once
#include <Audio.h>
#include "Parameter.h"

enum class Waveform : short {
    Sine     = WAVEFORM_SINE,
    Triangle = WAVEFORM_TRIANGLE,
    Sawtooth = WAVEFORM_SAWTOOTH,
    Square   = WAVEFORM_SQUARE,
    Pulse    = WAVEFORM_PULSE
};

class WaveformParameter : public Parameter {
private:
    static constexpr Waveform TABLE[] = {
        Waveform::Sine,
        Waveform::Triangle,
        Waveform::Sawtooth,
        Waveform::Square,
        Waveform::Pulse
    };
    static constexpr int COUNT = sizeof(TABLE) / sizeof(TABLE[0]);

public:
    WaveformParameter(Control& c) : Parameter(c) {}

    // La seule sortie utile : un enum, jamais un float
    Waveform get() const {
        int idx = (int)(value * COUNT);
        if (idx >= COUNT) idx = COUNT - 1;
        if (idx < 0) idx = 0;
        return TABLE[idx];
    }

    // Pour passer directement à osc.begin()
    short toAudioLib() const {
        return static_cast<short>(get());
    }
};