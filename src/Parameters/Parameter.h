#pragma once
#include <Arduino.h>
#include "../Controls/Control.h"
#include "../Controls/MidiCC.h"

// Seuil de détection de mouvement d'un pot (bruit ADC). En unités normalisées 0..1.
#ifndef POT_TOUCH_THRESHOLD
#define POT_TOUCH_THRESHOLD 0.01f
#endif

class Parameter {
    float defaultValue;   // valeur si aucune source n'a jamais été touchée
    float currentValue;   // valeur courante, toujours normalisée 0..1

    MidiCC   midiCC;      // source CC, encapsulée par valeur (sentinelle 255 = inactive)
    Control* pot;         // source hardware optionnelle, référencée par pointeur (peut être nullptr)

    float lastPotValue;   // dernière lecture pot, pour détecter le mouvement
    bool  potInitialized; // false tant qu'on n'a pas pris la première lecture

public:
    // ccNumber = 255 -> pas de CC.  pot = nullptr -> pas de hardware.
    Parameter(float def, uint8_t ccNumber = 255, Control* p = nullptr)
        : defaultValue(def),
          currentValue(def),
          midiCC(ccNumber),
          pot(p),
          lastPotValue(0.0f),
          potInitialized(false) {}

    // --- Routage CC entrant (appelé par Synth::handleCC) ---

    bool listensTo(uint8_t cc) const {
        return midiCC.listensTo(cc);
    }

    void setFromCC(uint8_t rawMidiValue) {
        midiCC.set(rawMidiValue);
        currentValue = midiCC.read();   // 0..1, dernier touché gagne
    }

    // --- Lecture continue du pot (appelée chaque tour dans update) ---

    void loop() {
        if (!pot) return;

        float v = pot->read();          // 0..1 attendu

        if (!potInitialized) {
            lastPotValue = v;
            potInitialized = true;
            return;                     // on ignore la position de boot : pas un "mouvement"
        }

        if (fabsf(v - lastPotValue) > POT_TOUCH_THRESHOLD) {
            lastPotValue = v;
            currentValue = v;           // le pot a bougé -> il prend la main
        }
    }

    // --- Accès ---

    float read() const { return currentValue; }

    uint8_t getCC() const { return midiCC.getCC(); }
};