#pragma once
#include "Parameter.h"

// Module de CONTRÔLE du glide (portamento).
//
// Même patron que Filter et Envelope :
//  - possède son Parameter (0..1, pur),
//  - connaît son unité (secondes : c'est une CONSTANTE DE TEMPS, pas une durée)
//    et fait le mapping,
//  - reste agnostique de sa destination : expose un getter mappé, la Voice
//    l'utilise pour son lissage exponentiel.
//
// Rappel : la valeur retournée est la constante de temps tau du lissage
// exponentiel dans Voice (currentNote += (target-current) * (1 - exp(-dt/tau))).
// Le glissement AUDIBLE dure ~3-4x tau. tau = 0 -> saut instantané (pas de glide).
class Glide {
public:
    Parameter time;   // 0..1

private:
    float timeMax = 0.3f;   // secondes : x=1 -> tau=0.3s (slide lent mais jouable)

public:
    // 255 = pas de CC. Départ à 0 = pas de glide par défaut.
    explicit Glide(uint8_t timeCC = 255)
        : time(0.0f, timeCC) {}

    // Plage configurable "par après", comme les autres modules.
    void setTimeRange(float maxSeconds) { timeMax = maxSeconds; }

    // Lecture continue d'un pot éventuel.
    void loop() { time.loop(); }

    // Getter mappé : la constante de temps en secondes. Surface publique.
    float timeConstant() const {
        return time.read() * timeMax;   // 0 = pas de glide
    }
};