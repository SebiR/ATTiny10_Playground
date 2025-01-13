// Simple frequency counter / frequency comparator
// LED on PB0
// Frequency input on T0 (PB2)

#ifndef F_CPU
#undef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define LED1_PIN PB0  // LED connected to PB0

void setup() {
  //Timer0 as Counter
  // Normal bin operation, normal Timer operation
  TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
  // External clock on T0 (PB2) pin, rising Edge
  TCCR0B = (0 << ICNC0) | (0 << ICES0) | (0 << WGM03) | (1 << WGM02) | (1 << CS02) | (1 << CS01) | (1 << CS00);

  // LED pin as output
  DDRB |= (1 << LED1_PIN);
}

void loop() {

  // 100 counts each 100ms: 1kHz
  // If frequency is above 1kHz: LED on
  if (TCNT0 > 100) {
    // LED on
    PORTB |= (1 << LED1_PIN);
  } else {
    // LED off
    PORTB &= ~(1 << LED1_PIN);
  }

  TCNT0 = 0;
  _delay_ms(100);
}
