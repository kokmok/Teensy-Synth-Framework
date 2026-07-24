    #pragma once
    #include <string>
    #include <Controls/Control.h>

class Parameter {
protected:                    
    std::string name;
    float value = 0.0f;
    Control& control;
    float untouchedControlValue = 1.0f;
    bool controlTouched = false;
    float presetValue = 1.0f;

public:
    Parameter(Control& c) : control(c) {
        untouchedControlValue = control.read();
    }

    virtual ~Parameter() {}

    float getValue() const { return value; }

    virtual void loop() {
        float controlVal = control.read();
        if (!controlTouched && controlVal != untouchedControlValue) {
            controlTouched = true;
        }
        if (controlTouched) {
            value = controlVal;
        }
    }
};