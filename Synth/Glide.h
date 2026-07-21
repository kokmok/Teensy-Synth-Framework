#pragma once
#include <Parameters/Parameter.h>

class Glide {
    private:
        Parameter& parameter;

    public:
        Glide(Parameter& p)
            : parameter(p)
        {
        }

        float getMilliseconds() {
            return 5.0f + parameter.getValue() * 1995.0f;
        }
};