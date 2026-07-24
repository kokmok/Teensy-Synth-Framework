#include <Arduino.h>
#include "TB303Patch.h"     // AVANT Synth.h : définit les dimensions
#include "Synth/Synth.h"

#define DEBUG_MIDI_CC


Synth synth;

void onControlChange(byte ch, byte cc, byte value) {
    #ifdef DEBUG_MIDI_CC
        Serial.print("CC ");
        Serial.print(cc);
        Serial.print(" = ");
        Serial.print(value);
        Serial.print("  (ch ");
        Serial.print(ch);
        Serial.println(")");
    #endif
    synth.handleCC(cc, value);
}

void setup() {
    Serial.begin(115200);
    Patch::configure();
    synth.begin(Patch::describe());

    usbMIDI.setHandleNoteOn([](byte ch, byte note, byte vel) {
        synth.noteOn(note, vel);
    });
    usbMIDI.setHandleNoteOff([](byte ch, byte note, byte vel) {
        synth.noteOff(note);
    });
    usbMIDI.setHandleControlChange(onControlChange);
}

void loop() {
    usbMIDI.read();
    synth.update();
}
