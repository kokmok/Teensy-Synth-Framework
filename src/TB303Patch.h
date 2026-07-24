#pragma once

#ifdef PATCH_DEFINED
  #error "Un seul patch à la fois"
#endif
#define PATCH_DEFINED

// --- Dimensions : la 303 est monophonique, un seul oscillateur ---
constexpr int VOICES        = 2;
constexpr int OSC_PER_VOICE = 1;

#include "Parameters/Filter.h"
#include "Parameters/Envelope.h"
#include <Audio.h>

// --- Le son : réglages caractéristiques de la 303 ---
struct PatchDescriptor {
    // Oscillateur : saw ou square, rien d'autre sur une 303
    short oscWaveform[OSC_PER_VOICE] = { WAVEFORM_SQUARE };
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

    inline Filter filter{ /* cutoffCC */ 1, /* resoCC */ 2 };
    inline Envelope envelope{ 5, 6, 7, 8 };
    
    inline PatchDescriptor describe() {
        PatchDescriptor d;
        d.filterCutoff = 600.0f;
        d.decay        = 300.0f;
        d.glideTime    = 0.06f;
        return d;
    }

    inline void configure() {
        envelope.setAttackRange(200.0f);
        envelope.setDecayRange(600.0f);
    }

    // La table que handleCC balaie : on pointe vers les Parameter DANS le filtre.
    inline Parameter* allParams[] = { &filter.cutoff, &filter.resonance, &envelope.attack, &envelope.decay, &envelope.sustain, &envelope.release };
    inline constexpr int PARAM_COUNT = 6;
}