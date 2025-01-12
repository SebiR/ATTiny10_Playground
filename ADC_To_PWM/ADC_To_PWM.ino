// Simple ADC to PWM code
// LED on PB0
// LED on PB1
// Potentiometer on PB2
// One LED gets brighter, one gets darker with different potentiometer settings
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

void setup() {

  //Timer0 for PWM
  // PWM, Phase Correct, 8bit
  TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
  TCCR0B = (0 << ICNC0) | (0 << ICES0) | (0 << WGM03) | (1 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);

  // Initialize ADC
  // No clock divider, MUX set to ADC2 -> PB2
  ADMUX = (1 << MUX1) | (0 << MUX0);
  ADCSRA = (1 << ADEN) | (0 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);

  // LED Pins are outputs
  DDRB = (1 << DDB0) | (1 << DDB1) | (0 << DDB2);
  //PUEB  = ~(1<<PUEB0) & ~(1<<PUEB1) & ~(1<<PUEB2);
  //PORTB = (1<<PORTB0) | (1<<PORTB1) | (1<<PORTB2);
}

void loop() {
  unsigned char val;

  // Start conversion
  ADCSRA |= (1 << ADSC);

  // Wait for conversion to complete
  while (ADCSRA & (1 << ADSC))
    ;

  val = (unsigned char)ADCL;
  OCR0A = val;
  OCR0B = 255 - val;
}