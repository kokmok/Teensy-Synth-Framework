    #pragma once
    #include <string>
    #include <Controls/Control.h>

    class Parameter {
        private:
            std::string name;
            float value;
            Control& control;
            float untouchedControlValue = 1.0f;
            bool controlTouched = false;
            float presetValue = 1.0f;
        public:
            Parameter(Control& c)
            : control(c) {
                untouchedControlValue = control.read();
            }
            void preset(float presetVal) {
                value = presetVal;
                presetValue = presetVal;
                controlTouched = false;
            }

            void loop() {
                float controlVal = control.read();
                if (!controlTouched && controlVal != untouchedControlValue) {
                    controlTouched = true;
                }

                if (controlTouched) {
                    value = control.getValue();
                }
            }
    };