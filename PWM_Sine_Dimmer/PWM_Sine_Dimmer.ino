#include <avr/io.h>
#include <util/delay.h>
#ifndef F_CPU
#define F_CPU 1000000UL
#endif

volatile byte sinepos = 0;

byte sine[] = { 1, 1, 2, 3, 5, 8, 11, 15, 20, 25, 30, 36, 43, 49, 56, 64, 72, 80, 88, 97, 105, 114, 123, 132, 141, 150, 158, 167, 175, 183, 191, 199, 206, 212, 219, 225, 230, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254, 253, 252, 250, 247, 244, 240, 235, 230, 225, 219, 212, 206, 199, 191, 183, 175, 167, 158, 150, 141, 132, 123, 114, 105, 97, 88, 80, 72, 64, 56, 49, 43, 36, 30, 25, 20, 15, 11, 8, 5, 3, 2, 1, 0 };

void setup() {
  DDRB = 1 << DDB0;

  //Timer0 for PWM
  // PWM, Phase Correct, 8bit
  TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
  TCCR0B = (0 << ICNC0) | (0 << ICES0) | (0 << WGM03) | (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);

  // LED Pins are outputs
  DDRB = (1 << DDB0) | (1 << DDB1);
}

void loop() {

  OCR0A = sine[sinepos];
  OCR0B = 255-sine[sinepos];

  sinepos++;

  if (sinepos >= sizeof(sine)) {
    sinepos = 0;
  }

  _delay_ms(50);
}