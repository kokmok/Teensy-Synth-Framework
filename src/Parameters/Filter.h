#pragma once
#include <Audio.h>
#include "Parameter.h"

// Module de CONTRÔLE du filtre.
//
// Même logique que Envelope : Parameter reste pur (0..1), le module connaît ses
// unités (Hz, résonance) et fait le mapping, mais reste agnostique de sa
// destination. Il expose des getters mappés ; le consommateur branche où il veut.
//
// Les plages du cutoff/résonance sont désormais configurables "par après",
// comme sur Envelope — un patch peut restreindre la course du cutoff.
//
// applyTo() n'est qu'un raccourci pour un AudioFilterStateVariable. Contournable.
class Filter {
public:
    Parameter cutoff;      // 0..1
    Parameter resonance;   // 0..1

private:
    // Plages configurables. Le cutoff est exponentiel (perception logarithmique),
    // la résonance linéaire (plage acceptée par AudioFilterStateVariable).
    float cutoffMin = 300.0f;    // Hz
    float cutoffMax = 8000.0f;   // Hz
    float resoMin   = 0.7f;
    float resoMax   = 10.0f;

public:
    Filter(uint8_t cutoffCC = 255, uint8_t resoCC = 255)
        : cutoff(0.6f, cutoffCC),
          resonance(0.814f, resoCC) {}

    // --- Plages configurables "par après" ---
    void setCutoffRange(float minHz, float maxHz) { cutoffMin = minHz; cutoffMax = maxHz; }
    void setResonanceRange(float min, float max)  { resoMin = min; resoMax = max; }

    // --- Lecture continue des pots éventuels ---
    void loop() {
        cutoff.loop();
        resonance.loop();
    }

    // --- Getters mappés : la surface publique agnostique de destination ---
    float cutoffHz() const {
        // exponentiel : chaque portion de course multiplie la fréquence
        return cutoffMin * powf(cutoffMax / cutoffMin, cutoff.read());
    }
    float resonanceValue() const {
        // linéaire
        return resoMin + resonance.read() * (resoMax - resoMin);
    }

    // --- Raccourci optionnel pour un AudioFilterStateVariable. Contournable. ---
    void applyTo(AudioFilterStateVariable& f) const {
        f.frequency(cutoffHz());
        f.resonance(resonanceValue());
    }
};