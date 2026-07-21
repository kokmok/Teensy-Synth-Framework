#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// Preferences - Multi-Teensy Synth Collection
// ============================================================================

// • DISPLAY TYPE
// Display Configuration (Choose ONE - comment out the other)
#define USE_LCD_DISPLAY
// #define USE_OLED_DISPLAY

// • MIDI TYPE
#define USE_USB_DEVICE_MIDI // USB Device MIDI for DAW/computer connection (default)
#define USE_MIDI_HOST       // USB Host MIDI for external controllers connected to Teensy
// #define USE_DIN_MIDI // DIN MIDI support - UNTESTED (requires moving enc3 from pin 0)

// • AUDIO TYPE
// #define USE_TEENSY_DAC        // Use Teensy Audio Shield or other I2S DAC
#define USE_USB_AUDIO      // Use USB Audio output (default)




//Mini-Teensy Parameters (31 total):
// 0: Osc1 Range (32' to LO)
// 1: Osc2 Range (32' to LO)
// 2: Osc3 Range (32' to LO)
// 3: Osc2 Fine (±12 semitones)
// 4: Osc3 Fine (±12 semitones)
// 5: Osc1 Wave (Triangle to Pulse)
// 6: Osc2 Wave (Triangle to Pulse)
// 7: Osc3 Wave (Triangle to Pulse)
// 8: Osc1 Volume (0.0-1.0)
// 9: Osc2 Volume (0.0-1.0)
// 10: Osc3 Volume (0.0-1.0)
// 11: Filter Cutoff (20Hz-20kHz)
// 12: Filter Resonance (0.0-3.0)
// 13: Filter Attack (1-3000ms)
// 14: Filter Decay (10-5000ms)
// 15: Filter Sustain (0.0-1.0)
// 16: Noise Volume (0.0-1.0)
// 17: Amp Attack (1-3000ms)
// 18: Amp Sustain (0.0-1.0)
// 19: Amp Decay (10-5000ms)
// 20: Osc1 Fine (±12 semitones)
// 21: Filter Strength (0.0-1.0)
// 22: LFO Rate (0.1-20Hz)
// 23: LFO Depth (0.0-1.0)
// 24: LFO Enable (0/1)
// 25: LFO Target (Pitch/Filter/Amp)
// 26: Play Mode (Mono/Poly/Legato)
// 27: Glide Time (0-1000ms)
// 28: Noise Type (White/Pink)
// 29: Macro Mode (0/1)
// 30: MIDI Channel (0-16)


// ============================================================================
// DIN MIDI Configuration (shared across all projects)
// ============================================================================

/*
 * DIN MIDI Setup Instructions:
 * 
 * HARDWARE REQUIRED:
 * - 6N138 optocoupler IC
 * - 220Ω resistor  
 * - 5-pin DIN MIDI connector
 * - Standard MIDI interface circuit (see MIDI specification)
 * 
 * WIRING:
 * 1. Build MIDI input circuit: DIN connector → 6N138 optocoupler → 220Ω resistor
 * 2. Connect MIDI circuit output to Teensy Serial1 RX (Pin 0)
 * 3. IMPORTANT: Move enc3 (Color/Range/etc) CLK wire from Pin 0 to surface mount pin (42-47)
 * 
 * USAGE:
 * - Install "MIDI Library" by Francois Best via Arduino Library Manager
 * - Uncomment #define USE_DIN_MIDI above
 * - Can work with both USB Device MIDI (default) and USB Host MIDI
 * - Supports USB and DIN MIDI simultaneously
 * - Uses same MIDI channel setting from Settings menu
 * - Receives Note On/Off, Control Change, and Pitch Bend
 */

#endif // CONFIG_H
