#pragma once
#include <cmath>

class Control {
    protected:
        float value = 1.0f;
        
    public:
        virtual float read() = 0;

        virtual ~Control() {}
};