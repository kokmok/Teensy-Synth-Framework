#include <Parameters/Parameter.h>

class Synth {
    public:
        Synth(int oscNumber, int voiceNumber) {
            
        }    
        Parameter cutoff;
        Parameter resonance;
        Parameter glideTime;

        std::vector<Voice> voices;

        void process();
};