#pragma once
#include <Audio.h>
#include "../Parameters/Parameter.h"

class Voice {
private: 
    Waveform lastWaveform = Patch::waveform.get();
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

    float currentNote = 69.0f;   // note MIDI en float, glisse vers targetNote
    float targetNote  = 69.0f;
    float glideTime   = 0.0f;    // secondes ; 0 = saut instantané


    Voice() {
        osc.begin(Patch::waveform.toAudioLib());
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
        if (Patch::waveform.get() != lastWaveform) {
            osc.begin(Patch::waveform.toAudioLib());
            lastWaveform = Patch::waveform.get();
        }
        if (currentNote != targetNote) {
            float tau = Patch::glide.timeConstant();   // relu en continu
            if (tau <= 0.0f) {
                currentNote = targetNote;
            } else {
                float k = 1.0f - expf(-dt / tau);
                currentNote += (targetNote - currentNote) * k;
                if (fabsf(targetNote - currentNote) < 0.001f) currentNote = targetNote;
            }
            osc.frequency(noteToFreq(currentNote));
        }
        Patch::filter.applyTo(filter);
    }

    void noteOn(int midiNote) {
        noteOnTime = millis();
        note   = midiNote;
        active = true;

        targetNote = (float)midiNote;
        if (Patch::glide.timeConstant() <= 0.0f) {
            currentNote = targetNote;              // pas de glide : saut immédiat
            osc.frequency(noteToFreq(currentNote));
        }
        // si glide : on ne touche pas currentNote ici, update() s'en charge

        Patch::envelope.applyTo(env);
        env.noteOn();
    }

    void noteOff() {
        env.noteOff();
        active = false;
    }


    static float noteToFreq(float midiNote) {
        return 440.0f * powf(2.0f, (midiNote - 69.0f) / 12.0f);
    }

    Voice(const Voice&) = delete;
    Voice& operator=(const Voice&) = delete;
};