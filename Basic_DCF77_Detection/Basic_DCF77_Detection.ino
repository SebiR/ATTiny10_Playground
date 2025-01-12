#ifdef F_CPU
#undef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED1_PIN PB1  // LED for high time <= 150ms
#define LED2_PIN PB2  // LED for high time > 150ms

volatile uint16_t high_time = 0;             // Stores high time in milliseconds
volatile uint8_t signal_detected = 0;        // Flag to indicate signal detection
volatile uint16_t timer_overflow_count = 0;  // Counts timer overflows for timeout

void setup() {
  // Configure PB1 and PB2 as outputs for LEDs
  DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN);
  PORTB &= ~((1 << LED1_PIN) | (1 << LED2_PIN));  // Turn LEDs off initially

  // Configure PB0 (PCINT0) as input for signal
  DDRB &= ~(1 << PB0);  // PB0 as input

  // Timer/Counter1 setup for input capture
  TCCR0B = (1 << CS01) | (0 << ICNC0);                    // Prescaler 8 (1 tick = 1 µs at 8 MHz clock)
  TIMSK0 |= (1 << ICIE0) | (1 << TOIE0);  // Enable input capture and overflow interrupts

  CCP = 0xD8;
  CLKMSR = (0 << CLKMS1) | (0 << CLKMS0);
  
  CCP = 0xD8;                                                              // Unprotect CLKPSR reg
  CLKPSR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);  // Set clock to 31.25kHz
  //OSCCAL = 0x96;    // Adjust clock if you want to

  // Enable global interrupts
  sei();
}

// Timer/Counter0 Input Capture Interrupt
ISR(TIM0_CAPT_vect) {
  PORTB ^= (1 << LED1_PIN);
    static uint16_t start_time = 0; // Tracks the start of the high signal

    if (PINB & (1 << PB0)) { // Signal rising edge
        start_time = ICR0; // Record start time
    } else { // Signal falling edge
        uint16_t end_time = ICR0; // Record end time
        if (end_time >= start_time) {
            high_time = end_time - start_time; // Calculate high time in microseconds
        } else {
            high_time = (0xFFFF - start_time + end_time + 1); // Handle timer overflow
        }
        high_time /= 1000; // Convert to milliseconds
        signal_detected = 1; // Set signal detected flag
        timer_overflow_count = 0; // Reset timeout counter
    }
}

// Timer/Counter0 Overflow Interrupt (handles no signal timeout)
ISR(TIM0_OVF_vect) {
  PORTB ^= (1 << LED2_PIN);
  if (++timer_overflow_count > 1500) {  // ~1.5 seconds timeout (1 overflow = 1 ms)
    signal_detected = 2;                // Timeout condition
    timer_overflow_count = 0;           // Reset overflow counter
  }
}

void light_led(uint8_t led_pin, uint16_t duration_ms) {
  PORTB |= (1 << led_pin);   // Turn LED on
  _delay_ms(duration_ms);    // Wait for the specified duration
  PORTB &= ~(1 << led_pin);  // Turn LED off
}

void loop() {
  if (signal_detected == 1) {  // High time measured
    if (high_time <= 150) {
      light_led(LED1_PIN, 200);  // Light LED1 for 200ms
    } else {
      light_led(LED2_PIN, 200);  // Light LED2 for 200ms
    }
    signal_detected = 0;              // Reset signal detected flag
  } else if (signal_detected == 2) {  // Timeout condition
    light_led(LED1_PIN, 200);         // Light both LEDs for 200ms
    light_led(LED2_PIN, 200);
    signal_detected = 0;  // Reset signal detected flag
  }
}
