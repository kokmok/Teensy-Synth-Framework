#pragma once

#include <Arduino.h>
#include <cmath>
#include "Control.h"

class Potentiometer : public Control {
private:
    int pin;
        
public:
    Potentiometer(int p) : pin(p) {}

    float read() override {
        float readVal = analogRead(pin) / 1023.0f;
        if (abs(readVal - value) > 0.02f ) {
            value = readVal;
        }

        return value;
    }
};