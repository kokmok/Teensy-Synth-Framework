// src/MyMoogPatch.h
#pragma once
#include <TeensySynth.h>

namespace MyMoog {

    // Hardware : les contrôles physiques
    inline Potentiometer wavePot1{ 6 };
    inline Potentiometer wavePot2{ 7 };
    inline Potentiometer cutoffPot{ 14 };

    // Paramètres
    inline WaveformParameter wave1{ wavePot1 };
    inline WaveformParameter wave2{ wavePot2 };

    // La description du synthé
    inline SynthDescriptor describe() {
        SynthDescriptor d;
        d.oscWaveform[0] = &wave1;
        d.oscWaveform[1] = &wave2;
        d.oscWaveform[2] = &wave1;
        d.attack  = 10.0f;
        d.release = 300.0f;
        return d;
    }
}