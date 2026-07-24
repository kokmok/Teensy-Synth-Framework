#pragma once
#include <Audio.h>
#include "../Parameters/Parameter.h"

class Voice {
public:
    // --- Objets audio, permanents ---
    AudioSynthWaveform      osc;    // oscillateur
    AudioFilterStateVariable filter; // filtre (le nouveau maillon)
    AudioEffectEnvelope     env;    // enveloppe d'ampli

    // --- Plomberie : osc -> filtre -> env ---
    AudioConnection patchOscToFilter{ osc, 0, filter, 0 };
    AudioConnection patchFilterToEnv{ filter, 0, env, 0 };  // sortie 0 = passe-bas

    // --- État logique ---
    bool     active = false;
    int      note   = -1;
    uint32_t noteOnTime = 0;


    Voice() {
        osc.begin(WAVEFORM_SAWTOOTH);
        osc.amplitude(0.6f);
        osc.frequency(440.0f);

        filter.frequency(1200.0f);   // cutoff fixe pour l'instant
        filter.resonance(0.7f);      // valeur plancher au boot

        env.attack(5.0f);
        env.decay(50.0f);
        env.sustain(0.7f);
        env.release(200.0f);
    }

    void begin(const PatchDescriptor& d) {
        
    }

    void update(float dt) {
        // ton glide existant reste ici, s'il y en avait

        Patch::filter.applyTo(filter);   // le module pousse cutoff+résonance dans l'objet audio local
    }

    void noteOn(int midiNote) {
        noteOnTime = millis();
        note   = midiNote;
        active = true;
        float freq = 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
        osc.frequency(freq);
        Patch::envelope.applyTo(env);
        env.noteOn();
    }

    void noteOff() {
        env.noteOff();
        active = false;
    }

    Voice(const Voice&) = delete;
    Voice& operator=(const Voice&) = delete;
};