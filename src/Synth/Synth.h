#pragma once
#include <Audio.h>
#include "Voice.h"

#ifndef PATCH_DEFINED
  #error "Aucun patch inclus. Inclus un fichier de patches/ avant Synth.h"
#endif

static_assert(VOICES >= 1, "VOICES doit valoir au moins 1");
static_assert(VOICES <= 4, "VOICES > 4 : cascading de mixers non implémenté");
static_assert(OSC_PER_VOICE >= 1, "OSC_PER_VOICE doit valoir au moins 1");
static_assert(OSC_PER_VOICE <= 4, "OSC_PER_VOICE > 4 : AudioMixer4 a 4 entrées");

class Synth {
private:
    AudioConnection* patches[VOICES];   // pointeurs, construits dans begin()

    // Cherche une voix libre ; sinon vole la plus ancienne
    int findVoice() {
        for (int i = 0; i < VOICES; i++) {
            if (!voices[i].active) return i;
        }
        int oldest = 0;
        for (int i = 1; i < VOICES; i++) {
            if (voices[i].noteOnTime < voices[oldest].noteOnTime) oldest = i;
        }
        return oldest;
    }

public:
    Voice voices[VOICES];
    AudioMixer4 mixer;

    AudioOutputUSB out;   // sortie vers l'USB, pas de shield

    AudioConnection patchOutL{ mixer, 0, out, 0 };
    AudioConnection patchOutR{ mixer, 0, out, 1 };

    void begin(const PatchDescriptor& d) {
        AudioMemory(60);
        for (int i = 0; i < VOICES; i++) {
            voices[i].begin(d);                    // ← manquait
            patches[i] = new AudioConnection(voices[i].env, 0, mixer, i);
            mixer.gain(i, d.mixerGain);            // ← vient du patch
        }
    }

    void update() {
        static uint32_t last = 0;
        uint32_t now = millis();
        float dt = (now - last) / 1000.0f;
        last = now;
        if (dt > 0.0f && dt < 1.0f) {
            for (int i = 0; i < VOICES; i++) voices[i].update(dt);
        }
    }

    void noteOn(int note, int) { 
        int v = findVoice();
        voices[v].noteOn(note);
    }
    
    void noteOff(int note) {
        for (int i = 0; i < VOICES; i++) {
            if (voices[i].active && voices[i].note == note) {
                voices[i].noteOff();
                return;   // une seule voix par note
            }
        }
    }

    Synth(const Synth&) = delete;
    Synth& operator=(const Synth&) = delete;
    Synth() = default;
};