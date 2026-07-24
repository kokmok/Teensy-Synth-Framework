#pragma once

#ifdef PATCH_DEFINED
  #error "Un seul patch à la fois"
#endif
#define PATCH_DEFINED

// --- Dimensions : la 303 est monophonique, un seul oscillateur ---
constexpr int VOICES        = 1;
constexpr int OSC_PER_VOICE = 1;

#include <Audio.h>

// --- Le son : réglages caractéristiques de la 303 ---
struct PatchDescriptor {
    // Oscillateur : saw ou square, rien d'autre sur une 303
    short oscWaveform[OSC_PER_VOICE] = { WAVEFORM_SAWTOOTH };
    float oscAmplitude = 0.8f;

    // Filtre : c'est là que vit le son
    float filterCutoff    = 800.0f;   // Hz
    float filterResonance = 3.5f;     // 0.7 à 5.0 ; la 303 vit dans le haut

    // Enveloppe d'ampli : attaque immédiate, pas de sustain
    float attack  = 1.0f;
    float decay   = 200.0f;
    float sustain = 0.0f;
    float release = 50.0f;

    // Glide (slide) : central au son 303
    float glideTime = 0.0f;           // secondes ; 0 = désactivé

    float mixerGain = 0.8f;           // une seule voix : pas besoin de diviser
};

namespace Patch {
    inline PatchDescriptor describe() {
        PatchDescriptor d;
        d.filterCutoff    = 600.0f;
        d.filterResonance = 4.2f;
        d.decay           = 300.0f;
        d.glideTime       = 0.06f;
        return d;
    }
}