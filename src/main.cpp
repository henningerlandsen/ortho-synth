#include <Audio.h>
#include <Wire.h>

#include "arp.h"

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     noise1;         //xy=113,469
AudioSynthWaveform       wave2;       //xy=114,419
AudioSynthWaveform       wave1;      //xy=115,373
AudioSynthWaveformDc     dc1;            //xy=202.22222222222223,542.2222222222222
AudioMixer4              mixer1;         //xy=245.55556106567383,423.8888969421387
AudioEffectEnvelope      filterEnv;      //xy=360,541.11110496521
AudioEffectEnvelope      ampEnv;      //xy=382.7777633666992,356.6666507720947
AudioFilterStateVariable filter1;        //xy=519.2221984863281,363.66668128967285
AudioOutputAnalog        dac;            //xy=645.4444389343262,350.88887882232666
AudioConnection          patchCord1(noise1, 0, mixer1, 2);
AudioConnection          patchCord2(wave2, 0, mixer1, 1);
AudioConnection          patchCord3(wave1, 0, mixer1, 0);
AudioConnection          patchCord4(dc1, filterEnv);
AudioConnection          patchCord5(mixer1, ampEnv);
AudioConnection          patchCord6(filterEnv, 0, filter1, 1);
AudioConnection          patchCord7(ampEnv, 0, filter1, 0);
AudioConnection          patchCord8(filter1, 0, dac, 0);
// GUItool: end automatically generated code

float notes[85] = {
  32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55, 58.27, 61.735,
  65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.826, 110, 116.541, 123.471,
  130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942,
  261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883,
  523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767,
  1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533,
  2093.005, 2217.461, 2349.318, 2489.016, 2637.021, 2793.826, 2959.955, 3135.964, 3322.438, 3520, 3729.31, 3951.066,
  4186.009
};

float channels[4] = {1.0, 1.0, 1.0, 1.0};
float main_volume = 0.02;
int current_note_index = 0;
int counter = 0;

struct OscData {
  float octave = 0;
  float tune = 0;
};
OscData osc[2];

Arp arp;


float getNoteFreq(int index) {
  if (index < 0) {
    return notes[0];
  } else if (index > 84) {
    return notes[84];
  } else {
    return notes[index];
  }
}

void updateMixerVolume() {
  for (int i=0; i < 4; i++) {
    mixer1.gain(i, channels[i] * main_volume);
  }
}

void updateOscFreq() {
  float osc1_freq = getNoteFreq(current_note_index + osc[0].octave * 12);
  Serial.print("Osc 1 Hz: ");
  Serial.println(osc1_freq);

  wave1.frequency(osc1_freq);
  wave2.frequency(getNoteFreq(current_note_index + osc[1].octave * 12) + osc[1].tune);  
}

void onNoteOn(byte channel, byte note, byte velocity) {
  Serial.print("Note on: ");
  Serial.println(note);

  arp.noteOn(note - 24);

  // updateOscFreq();

  // ampEnv.noteOn();
  // filterEnv.noteOn();
}

void onNoteOff(byte channel, byte note, byte velocity) {
  Serial.print("Note off: ");
  Serial.println(note);
  // ampEnv.noteOff();
  // filterEnv.noteOff();

  arp.noteOff(note - 24);
}

void onControlChange(byte channel, byte control, byte midi_value) {
  Serial.print("Midi CC: ");
  Serial.print(control);
  Serial.print(" Val: ");
  Serial.println(midi_value);
  const float value = float(midi_value) / 127;
  switch (control) {
    case 95:
      osc[0].octave = midi_value;
      updateOscFreq();
      break;
    case 96:
      osc[0].tune = (value - 0.5) * 64;
      updateOscFreq();
      break;
    case 97:
      osc[1].octave = midi_value;
      updateOscFreq();
      break;
    case 98:
      osc[1].tune = (value - 0.5) * 64;
      updateOscFreq();
      break;
    case 99:
      main_volume = value;
      updateMixerVolume();
      break;
    case 100:
    case 101:
    case 102:
      channels[control-100] = value;
      updateMixerVolume();
      break;
    case 103:
      ampEnv.attack(1000 * value);
      break;
    case 104:
      ampEnv.decay(1000 * value);
      break;
    case 105:
      ampEnv.sustain(value);
      break;
    case 106:
      ampEnv.release(1000 * value);
      break;
    case 107:
      filterEnv.attack(1000 * value);
      break;
    case 108:
      filterEnv.decay(1000 * value);
      break;
    case 109:
      filterEnv.sustain(value);
      break;
    case 110:
      filterEnv.release(1000 * value);
      break;
    case 111:
      filter1.frequency(value * 2500);
      break;
    case 112:
      filter1.resonance(5 * value);
      break;
    case 113:
      arp.setTicksPerStep(50'000 * (1 + value));
      break;
  }
}

void setup() {
  Serial.begin(38400);
  // Register MIDI handler
  usbMIDI.setHandleControlChange(onControlChange);
  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleNoteOff(onNoteOff);
  // Setup audio system
  AudioMemory(20);
  // Turn on the full voltage amplifier
  //dac.analogReference(EXTERNAL);
  delay(50); // Give voltage time to stabilize
  
  // Turn on the amp
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  delay(10); // Allow time to wake

  // Setup audio sources
  wave1.begin(WAVEFORM_SAWTOOTH);
  wave1.amplitude(1.0);
  wave1.frequency(440);

  wave2.begin(WAVEFORM_SQUARE);
  wave2.amplitude(1.0);
  wave2.frequency(220);

  noise1.amplitude(1.0);

  // Set default mixer level
  updateMixerVolume();

  filter1.octaveControl(2.5);
  dc1.amplitude(1.0); // Feed the filter control value
}

void loop() { 
  usbMIDI.read();

  const Arp::State state = arp.getNextState();
  if (state.note == -1) {
    ampEnv.noteOff();
    filterEnv.noteOff();
  }
  else if (state.cycle == Arp::State::Cycle::Trigger)
  {
    current_note_index = state.note;
   updateOscFreq();
   ampEnv.noteOn();
   filterEnv.noteOn();   
  }
}  