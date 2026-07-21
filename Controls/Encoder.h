#pragma once

#include "Control.h"
#include <cmath>
#include <Encoder.h>

class Encoder : public Control {
private:
    Encoder enc;
    long currentPos = 0L;
    

public:
    Encoder(int clkPin, int dtPin) : enc(clkPin, dtPin) {
        currentPos = read();
    }

    float read() override {
        long pos = enc.read();

        long delta = pos - currentPos;
        currentPos = pos;

        float localVal = value + delta * 0.01f;

        if (localVal < 0) localVal = 0;
        if (localVal > 1) localVal = 1;

        value = localVal;

        return value;
    }
};