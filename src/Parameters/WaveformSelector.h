#pragma once
#include <Audio.h>
#include <initializer_list>
#include "Parameter.h"

enum class Waveform : short {
    Sine     = WAVEFORM_SINE,
    Triangle = WAVEFORM_TRIANGLE,
    Sawtooth = WAVEFORM_SAWTOOTH,
    Square   = WAVEFORM_SQUARE,
    Pulse    = WAVEFORM_PULSE
};

// Module de CONTRÔLE de la forme d'onde.
//
// Possède un Parameter pur (0..1) et le mappe vers un enum discret. La liste des
// formes DISPONIBLES est configurable par patch : un 303 n'expose que saw+square,
// ce qui rend aussi le knob plus utile (moins de tranches, chacune plus large).
//
// Pas d'allocation dynamique : table de capacité fixe + compteur d'éléments réels.
class WaveformSelector {
public:
    Parameter selection;   // 0..1

private:
    static constexpr int CAPACITY = 8;
    Waveform table[CAPACITY];
    int count;

public:
    // Par défaut : les 5 formes, saw au centre.
    explicit WaveformSelector(uint8_t cc = 255)
        : selection(0.5f, cc), count(5) {
        table[0] = Waveform::Sine;
        table[1] = Waveform::Triangle;
        table[2] = Waveform::Sawtooth;
        table[3] = Waveform::Square;
        table[4] = Waveform::Pulse;
    }

    // Restreint / redéfinit les formes disponibles. Appelé "par après" dans le patch :
    //   waveform.setWaveforms({ Waveform::Sawtooth, Waveform::Square });
    void setWaveforms(std::initializer_list<Waveform> list) {
        count = 0;
        for (Waveform w : list) {
            if (count >= CAPACITY) break;
            table[count++] = w;
        }
    }

    void loop() { selection.loop(); }

    // Getter mappé : la forme choisie. Découpe 0..1 en `count` tranches égales.
    Waveform get() const {
        if (count <= 0) return Waveform::Sawtooth;   // garde-fou : jamais vide
        int idx = (int)(selection.read() * count);
        if (idx >= count) idx = count - 1;           // read()==1.0 -> dernière
        if (idx < 0)      idx = 0;
        return table[idx];
    }

    short toAudioLib() const {
        return static_cast<short>(get());
    }
};