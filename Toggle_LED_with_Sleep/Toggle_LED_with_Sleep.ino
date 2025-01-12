// Toggle an LED with a button
// Debounced, with sleep and pin change interrupt

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED_PIN PB0     // LED connected to PB0
#define BUTTON_PIN PB2  // Button connected to PB2

volatile uint8_t button_pressed = 0;  // Flag to track button press

void setup() {
  // Configure PB0 as output for LED
  DDRB |= (1 << LED_PIN);
  PORTB &= ~(1 << LED_PIN);  // Start with LED off

  // Configure PB2 as input with internal pull-up for button
  DDRB &= ~(1 << BUTTON_PIN);  // Set PB2 as input
  PUEB |= (1 << BUTTON_PIN);   // Enable pull-up resistor on PB2

  // Enable pin change interrupt for PB2

  PCICR |= (1 << PCIE0);       // Enable pin change interrupt
  PCMSK |= (1 << BUTTON_PIN);  // Enable interrupt on PB2


  // Set up sleep mode to power down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Enable global interrupts
  sei();
}

ISR(PCINT0_vect) {
  // Debounce the button
  _delay_ms(500);
  if (!(PINB & (1 << BUTTON_PIN))) {  // Check if button is still pressed
    button_pressed = 1;               // Set flag to toggle LED
  }
}

void loop() {
  // Check if button was pressed
  if (button_pressed) {
    button_pressed = 0;  // Clear the flag

    // Toggle LED
    PORTB ^= (1 << LED_PIN);

    // Add a small delay to debounce further presses
    _delay_ms(50);
  }

  // Enter sleep mode
  sleep_mode();
  // Execution resumes here after waking up

}
