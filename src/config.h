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
// #define USE_MIDI_HOST       // USB Host MIDI for external controllers connected to Teensy
// #define USE_DIN_MIDI // DIN MIDI support - UNTESTED (requires moving enc3 from pin 0)

// • AUDIO TYPE
// #define USE_TEENSY_DAC        // Use Teensy Audio Shield or other I2S DAC
#define USE_USB_AUDIO      // Use USB Audio output (default)

constexpr int VOICES = 4;
constexpr int OSC_PER_VOICE = 3;


#endif // CONFIG_H
