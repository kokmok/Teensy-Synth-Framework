#pragma once
#include <Audio.h>

#ifndef PATCH_DEFINED
  #error "Aucun patch inclus."
#endif

class Voice {
public:
    AudioSynthWaveform       osc[OSC_PER_VOICE];
    AudioMixer4              oscMixer;
    AudioFilterStateVariable filter;
    AudioEffectEnvelope      env;

    // osc -> oscMixer construites dans begin() ; le reste est fixe
    AudioConnection patchMixToFilter{ oscMixer, 0, filter, 0 };
    AudioConnection patchFilterToEnv{ filter, 0, env, 0 };   // sortie 0 = lowpass

    bool     active     = false;
    int      note       = -1;
    uint32_t noteOnTime = 0;

    float currentFreq = 110.0f;
    float targetFreq  = 110.0f;
    float glideTime   = 0.0f;

    void begin(const PatchDescriptor& d) {
        glideTime = d.glideTime;

        for (int o = 0; o < OSC_PER_VOICE; o++) {
            oscPatches[o] = new AudioConnection(osc[o], 0, oscMixer, o);
            osc[o].begin(d.oscWaveform[o]);
            osc[o].amplitude(d.oscAmplitude);
            osc[o].frequency(currentFreq);
            oscMixer.gain(o, 1.0f / OSC_PER_VOICE);
        }

        filter.frequency(d.filterCutoff);
        filter.resonance(d.filterResonance);
        filter.octaveControl(0.0f);

        env.attack(d.attack);
        env.decay(d.decay);
        env.sustain(d.sustain);
        env.release(d.release);
    }

    void noteOn(int midiNote) {
        float freq = 440.0f * powf(2.0f, (midiNote - 69) / 12.0f);
        bool wasActive = active;

        note       = midiNote;
        active     = true;
        noteOnTime = millis();
        targetFreq = freq;

        if (!(glideTime > 0.0f && wasActive)) {
            currentFreq = freq;
            setFrequencies(freq);
        }
        env.noteOn();
    }

    void noteOff() {
        env.noteOff();
        active = false;
    }

    // Fait avancer le glide ; appelée depuis Synth::update()
    void update(float dt) {
        if (currentFreq == targetFreq) return;

        if (glideTime <= 0.0f) {
            currentFreq = targetFreq;
        } else {
            // Interpolation exponentielle : la perception du pitch est logarithmique
            float ratio = targetFreq / currentFreq;
            currentFreq *= powf(ratio, dt / glideTime);
            if (fabsf(currentFreq - targetFreq) < 0.5f) currentFreq = targetFreq;
        }
        setFrequencies(currentFreq);
    }

    Voice(const Voice&) = delete;
    Voice& operator=(const Voice&) = delete;
    Voice() = default;

private:
    AudioConnection* oscPatches[OSC_PER_VOICE];

    void setFrequencies(float f) {
        for (int o = 0; o < OSC_PER_VOICE; o++) osc[o].frequency(f);
    }
};