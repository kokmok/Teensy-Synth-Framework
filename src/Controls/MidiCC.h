#pragma once
#include <Arduino.h>
#include "Control.h"

class MidiCC : public Control {
    uint8_t ccNumber;     // 255 = pas de CC assigné (sentinelle)

public:
    explicit MidiCC(uint8_t cc = 255) : ccNumber(cc) {
        value = 0.0f;     // un CC démarre à 0, pas à 1 comme le défaut de Control
    }

    bool listensTo(uint8_t cc) const {
        return ccNumber != 255 && cc == ccNumber;
    }

    // Push : appelé quand un message CC entrant nous concerne
    void set(uint8_t rawMidiValue) {
        value = rawMidiValue / 127.0f;   // normalisation ici, une bonne fois
    }

    // Pull : lecture normalisée 0.0 .. 1.0
    float read() override {
        return value;
    }

    uint8_t getCC() const { return ccNumber; }
};