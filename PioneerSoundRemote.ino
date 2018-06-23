// Simple remote to control the sound on a Pioneer A/V receiver by Philip Fourie
// Date: 2018/06/23

// Bill of materials:
// ------------------
// Arduino Uno (5V)
// PD333-3B/H0/L2 
//   - 5mm photodiode  (used to record the original IR data code for each button)
//   - http://www.everlight.com/file/ProductFile/PD333-3B-H0-L2.pdf
// 
// IR333C/H0/L10  
//   - 5mm Infrared LED,T-1 3/4 (used to send a IR data code)
//   - http://www.everlight.com/file/ProductFile/IR333C-H0-L10.pdf

// Pinout:
// -------
// Rotary encoder:
// CLK  -> A3
// DT   -> A2
// SW   -> Not connected
// GND  -> GND
// VCC  -> VC (5V)
//
// IR LED:
// GND  -> GND
// Data -> D10


// 3rd pary libraries
// ------------------
// To the libary authors and contributors, thank you very much, without you I wouldn't have
// been able to do this.
//
// This library enables you to send and receive using infra-red signals on an Arduino.
// https://github.com/z3t0/Arduino-IRremote
#include <IRremote.h>

// RotaryEncoder library by Matthias Hertel
// https://github.com/mathertel/RotaryEncoder
// http://www.mathertel.de/Arduino/RotaryEncoderLibrary.aspx
// This library decodes the signals from a rotary encoder and translates them into a counter position
#include <RotaryEncoder.h>

// Setup a RotaryEncoder for pins A2 and A3:
RotaryEncoder encoder(A2, A3);

IRsend irsend;

void setup() {
   Serial.begin (9600);
   Serial.println("Started");
   pinMode (13, OUTPUT);

  // You may have to modify the next 2 lines if using other pins than A2 and A3
  PCICR  |= (1 << PCIE1);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);  // This enables the interrupt for pin 2 and 3 of Port C.      
}

ISR(PCINT1_vect) {
  encoder.tick(); // just call tick() to check the state.
}

void loop() {
  static int pos = 0;

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    Serial.print(newPos);
    Serial.println();

    if (newPos > pos) {
      volumeUp();
    } else {
      volumeDown();
    }
    
    pos = newPos;

  } 
  delay(1);
}

void volumeDown() {
  Serial.println("Volume down");
  sendSignal(0xA55AD02F);
}

void volumeUp() {
  Serial.println("Volume up");
  sendSignal(0xA55A50AF);
}

void sendSignal(unsigned long value) {
  digitalWrite(13, HIGH);
	
	for (int i = 0; i < 3; i++) {
    irsend.sendNEC(value, 32);
    delay(40);
	}
 
  digitalWrite(13, LOW);
}

