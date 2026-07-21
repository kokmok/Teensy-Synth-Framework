#pragma once
#include <Audio.h>

#include <vector>



class Voice {
    public:
        float currentPitch;
        float targetPitch;
        bool gate;

        std::vector<Oscillator> oscillators;
}