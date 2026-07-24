#pragma once
#include <Audio.h>
#include "Parameter.h"

// Module de CONTRÔLE de l'enveloppe ADSR.
//
// Deux niveaux de pureté :
//  - Parameter est pur : 0..1, aucune unité.
//  - Envelope est agnostique de sa DESTINATION : il connaît son unité (ms) et
//    fait le mapping, mais ne sait pas où ses valeurs seront branchées.
//    Il expose des getters mappés ; le consommateur (Voice ou autre) décide
//    où pousser. Un même Envelope peut piloter un AudioEffectEnvelope, un delay,
//    un LFO, n'importe quoi qui accepte des ms / un niveau.
//
// applyTo() n'est plus qu'un RACCOURCI pour le cas courant (un AudioEffectEnvelope) :
// il ne fait qu'appeler les getters. On peut l'ignorer et brancher à la main.
class Envelope {
public:
    Parameter attack;    // 0..1
    Parameter decay;     // 0..1
    Parameter sustain;   // 0..1 (niveau)
    Parameter release;   // 0..1

private:
    float attackMax  = 1000.0f;   // ms
    float decayMax   = 1000.0f;   // ms
    float releaseMax = 2000.0f;   // ms

public:
    Envelope(uint8_t attackCC  = 255,
             uint8_t decayCC   = 255,
             uint8_t sustainCC = 255,
             uint8_t releaseCC = 255)
        : attack(0.005f, attackCC),
          decay(0.2f,    decayCC),
          sustain(0.0f,  sustainCC),
          release(0.1f,  releaseCC) {}

    // --- Plages configurables "par après" ---
    void setAttackRange(float maxMs)  { attackMax  = maxMs; }
    void setDecayRange(float maxMs)   { decayMax   = maxMs; }
    void setReleaseRange(float maxMs) { releaseMax = maxMs; }

    // --- Lecture continue des pots éventuels ---
    void loop() {
        attack.loop();
        decay.loop();
        sustain.loop();
        release.loop();
    }

    // --- Getters mappés : le module dit ce que valent ses paramètres en unités
    //     réelles, sans imposer la destination. C'est LA surface publique. ---
    float attackMs()   const { return attack.read()  * attackMax; }
    float decayMs()    const { return decay.read()   * decayMax; }
    float sustainLvl() const { return sustain.read(); }        // niveau, pas de mapping
    float releaseMs()  const { return release.read() * releaseMax; }

    // --- Raccourci optionnel pour le cas courant. Contournable. ---
    void applyTo(AudioEffectEnvelope& e) const {
        e.attack(attackMs());
        e.decay(decayMs());
        e.sustain(sustainLvl());
        e.release(releaseMs());
    }
};