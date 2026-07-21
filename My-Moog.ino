#include "config.h"

#include <Parameters/Paremeter.h>
#include <Controls/Potentiometer.h>
#include <Controls/Encoder.h>
#include <Synth/Synth.h>
#include <Audio.h>

#ifdef USE_MIDI_HOST
USBHost myusb;
USBHub hub1(myusb);
MIDIDevice midi1(myusb);
#endif

#ifdef USE_DIN_MIDI
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
#endif

#ifdef USE_LCD_DISPLAY
  #include <LiquidCrystal_I2C.h>
#endif

#ifdef USE_OLED_DISPLAY
  #include <U8g2lib.h>
  #define OLED_WIDTH 128
  #define OLED_HEIGHT 32
  #define OLED_RESET -1
#endif

// std::unordered_map<std::string, Parameter> parameters;

// parameters["waveform"] = Parameter("waveform", Encoder(1,2));
// parameters["decay"] = Parameter("decay", Potentiometer(11));
// parameters["resononance"] = Parameter("resonnace", Potentiometer(10));
// parameters["cutoff"] = Parameter("cutoff", Potentiometer(5));
// parameters["accent"] = Parameter("accent", Potentiometer(6));


void setup() {
  // put your setup code here, to run once:
  resonnance = Parameter("resonnance", Potentiometer(10));
  cutoff = Parameter("cutoff", Potentiometer(5));
  glideTime = Parameter("glide", Potentiometer(6));
  synth = Synth();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void noteOn(int note, int velocity) {
  int voiceNum = -1;
  
  if (playMode == 0) {
    // Mono mode - use note stack but retrigger envelope for every new note
    addToMonoStack(note);
    
    // Turn off other voices if they're somehow active
    for (int v = 1; v < VOICES; v++) {
      if (voices[v].active) {
        ampEnv[v].noteOff();
        filtEnv[v].noteOff();
        voices[v].active = false;
      }
    }
    
    // Always use voice 0 for mono mode
    voiceNum = 0;
    
    // If this is not the top note, don't trigger it yet
    if (getTopMonoNote() != note) {
      return; // Wait until this becomes the top note
    }
    
    // Stop current envelope if active
    if (voices[0].active) {
      ampEnv[0].noteOff();
      filtEnv[0].noteOff();
    }
    
    // Check if voice was active before setting up new note (for glide)
    bool wasActive = voices[0].active;
    
    // Set up the voice
    voices[0].note = note;
    voices[0].active = true;
    voices[0].noteOnTime = millis();
    
    // Calculate and set frequencies
    float baseFreq = 440.0 * pow(2.0, (note - 69) / 12.0);
    float pitchWheelMultiplier = pow(2.0, pitchWheelValue * 2.0 / 12.0);
    
    if (glideTime > 0.0 && wasActive) {
      // Glide enabled and voice was already active - start glide to new frequency
      targetFreq[0] = baseFreq;
      gliding[0] = true;
      // currentFreq[0] keeps its current value to glide from
    } else {
      // No glide or first note - set frequency directly
      currentFreq[0] = baseFreq;
      targetFreq[0] = baseFreq;
      gliding[0] = false;
      osc1[0].frequency(baseFreq * osc1Range * osc1Fine * pitchWheelMultiplier);
      osc2[0].frequency(baseFreq * osc2Range * osc2Fine * pitchWheelMultiplier);
      osc3[0].frequency(baseFreq * osc3Range * osc3Fine * pitchWheelMultiplier);
    }
    
    // Always trigger envelopes in mono mode (retrigger for every note)
    ampEnv[0].noteOn();
    filtEnv[0].noteOn();
  } 
  else if (playMode == 2) {
    // Legato mode - use note stack for smooth transitions without envelope retrigger
    addToMonoStack(note);
    
    // Always use voice 0 for legato mode
    voiceNum = 0;
    
    // Turn off other voices if they're somehow active
    for (int v = 1; v < VOICES; v++) {
      if (voices[v].active) {
        ampEnv[v].noteOff();
        filtEnv[v].noteOff();
        voices[v].active = false;
      }
    }
    
    // If this is not the top note, don't trigger it yet
    if (getTopMonoNote() != note) {
      return; // Wait until this becomes the top note
    }
    
    // Don't retrigger envelope if voice 0 is already active (legato behavior)
    bool wasActive = voices[0].active;
    voices[0].note = note;
    voices[0].active = true;
    voices[0].noteOnTime = millis();
    
    // Calculate and set frequencies
    float baseFreq = 440.0 * pow(2.0, (note - 69) / 12.0);
    float pitchWheelMultiplier = pow(2.0, pitchWheelValue * 2.0 / 12.0);
    
    if (glideTime > 0.0 && wasActive) {
      // Glide enabled and voice was already active - start glide to new frequency
      targetFreq[0] = baseFreq;
      gliding[0] = true;
      // currentFreq[0] keeps its current value to glide from
    } else {
      // No glide or first note - set frequency directly
      currentFreq[0] = baseFreq;
      targetFreq[0] = baseFreq;
      gliding[0] = false;
      osc1[0].frequency(baseFreq * osc1Range * osc1Fine * pitchWheelMultiplier);
      osc2[0].frequency(baseFreq * osc2Range * osc2Fine * pitchWheelMultiplier);
      osc3[0].frequency(baseFreq * osc3Range * osc3Fine * pitchWheelMultiplier);
    }
    
    // Only trigger envelopes if no note was previously active
    if (!wasActive) {
      ampEnv[0].noteOn();
      filtEnv[0].noteOn();
    }
  } 
  else {
    // Poly mode - normal polyphonic behavior
    voiceNum = findAvailableVoice();
    
    // Check if voice was active before stealing (for glide)
    bool wasActive = voices[voiceNum].active;
    
    // If voice stealing, turn off the old note
    if (voices[voiceNum].active) {
      ampEnv[voiceNum].noteOff();
      filtEnv[voiceNum].noteOff();
    }
    
    // Set up the voice
    voices[voiceNum].note = note;
    voices[voiceNum].active = true;
    voices[voiceNum].noteOnTime = millis();
    
    // Calculate and set frequencies
    float baseFreq = 440.0 * pow(2.0, (note - 69) / 12.0);
    float pitchWheelMultiplier = pow(2.0, pitchWheelValue * 2.0 / 12.0);
    
    if (glideTime > 0.0 && wasActive) {
      // Glide enabled and voice was being reused - start glide to new frequency
      targetFreq[voiceNum] = baseFreq;
      gliding[voiceNum] = true;
      // currentFreq[voiceNum] keeps its current value to glide from
    } else {
      // No glide or new voice - set frequency directly
      currentFreq[voiceNum] = baseFreq;
      targetFreq[voiceNum] = baseFreq;
      gliding[voiceNum] = false;
      osc1[voiceNum].frequency(baseFreq * osc1Range * osc1Fine * pitchWheelMultiplier);
      osc2[voiceNum].frequency(baseFreq * osc2Range * osc2Fine * pitchWheelMultiplier);
      osc3[voiceNum].frequency(baseFreq * osc3Range * osc3Fine * pitchWheelMultiplier);
    }
    
    // Always trigger envelopes in poly mode
    ampEnv[voiceNum].noteOn();
    filtEnv[voiceNum].noteOn();
  }
}

void noteOff(int note) {
  if (playMode == 0) {
    // Mono mode - use note stack and retrigger envelope for next note
    removeFromMonoStack(note);
    
    // If the released note was the currently playing note
    if (voices[0].active && voices[0].note == note) {
      // Stop current envelope
      ampEnv[0].noteOff();
      filtEnv[0].noteOff();
      
      int nextNote = getTopMonoNote();
      if (nextNote != -1) {
        // Play the next note in the stack WITH envelope retrigger (mono behavior)
        voices[0].note = nextNote;
        float baseFreq = 440.0 * pow(2.0, (nextNote - 69) / 12.0);
        float pitchWheelMultiplier = pow(2.0, pitchWheelValue * 2.0 / 12.0);
        
        if (glideTime > 0.0) {
          // Start glide to the next note
          targetFreq[0] = baseFreq;
          gliding[0] = true;
          // currentFreq[0] keeps its current value to glide from
        } else {
          // No glide - set frequency directly
          currentFreq[0] = baseFreq;
          targetFreq[0] = baseFreq;
          gliding[0] = false;
          osc1[0].frequency(baseFreq * osc1Range * osc1Fine * pitchWheelMultiplier);
          osc2[0].frequency(baseFreq * osc2Range * osc2Fine * pitchWheelMultiplier);
          osc3[0].frequency(baseFreq * osc3Range * osc3Fine * pitchWheelMultiplier);
        }
        
        // Retrigger envelopes for the next note (mono behavior)
        ampEnv[0].noteOn();
        filtEnv[0].noteOn();
      } else {
        // No more notes - voice stays off
        voices[0].active = false;
      }
    }
  } 
  else if (playMode == 2) {
    // Legato mode - use note stack for proper priority
    removeFromMonoStack(note);
    
    // If the released note was the currently playing note
    if (voices[0].active && voices[0].note == note) {
      int nextNote = getTopMonoNote();
      if (nextNote != -1) {
        // Play the next note in the stack without retriggering envelopes
        voices[0].note = nextNote;
        float baseFreq = 440.0 * pow(2.0, (nextNote - 69) / 12.0);
        float pitchWheelMultiplier = pow(2.0, pitchWheelValue * 2.0 / 12.0);
        
        if (glideTime > 0.0) {
          // Start glide to the next note
          targetFreq[0] = baseFreq;
          gliding[0] = true;
          // currentFreq[0] keeps its current value to glide from
        } else {
          // No glide - set frequency directly
          currentFreq[0] = baseFreq;
          targetFreq[0] = baseFreq;
          gliding[0] = false;
          osc1[0].frequency(baseFreq * osc1Range * osc1Fine * pitchWheelMultiplier);
          osc2[0].frequency(baseFreq * osc2Range * osc2Fine * pitchWheelMultiplier);
          osc3[0].frequency(baseFreq * osc3Range * osc3Fine * pitchWheelMultiplier);
        }
      } else {
        // No more notes - turn off envelopes
        ampEnv[0].noteOff();
        filtEnv[0].noteOff();
        voices[0].active = false;
      }
    }
  } 
  else {
    // Poly mode - normal note off behavior
    int voiceNum = findVoiceForNote(note);
    if (voiceNum >= 0) {
      // Turn off envelopes
      ampEnv[voiceNum].noteOff();
      filtEnv[voiceNum].noteOff();
      voices[voiceNum].active = false;
    }
  }
}



void processMidiMessage(byte type, byte channel, byte data1, byte data2) {
  // Filter by MIDI channel (0 = omni, 1-16 = specific channel)
  if (midiChannel != 0 && channel != midiChannel) return;
  
  switch (type) {
    case 0x90: // Note On (or Note Off with velocity 0)
      if (data2 > 0) {
        noteOn(data1, data2);
      } else {
        noteOff(data1);
      }
      break;
      
    case 0x80: // Note Off
      noteOff(data1);
      break;
      
    case 0xB0: // Control Change
      // handleControlChange(data1, data2);
      break;
      
    case 0xC0: // Program Change
      // handleProgramChange(data1);
      break;
      
    // case 0xE0: // Pitch Bend
    //   {
    //     int pitchBendValue = (data2 << 7) | data1; // Combine MSB and LSB
    //     pitchWheelValue = (pitchBendValue - 8192) / 8192.0;
    //   }
    //   break;
  }
}