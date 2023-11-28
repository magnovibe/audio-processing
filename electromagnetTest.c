#include </Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include <stdio.h>
/*
  Turns on an Electromagnet on for one second, then off for one second, repeatedly.
  This example code is in the public domain.
*/

uint16_t electromagnet = 2;

// PWM width will vary depending on loudness, for now it's constant
volatile int pwm_width = 4000;
uint16_t tempo = 0;
void timer1_init() {
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1B |= (1 << CS11);
  TCCR1A |= (1 << WGM11) | (WGM10);
  TCCR1A |= (1 << COM1B1);


  OCR1A = 40000;

  // PB2 (D10) = PULSE OUTPUT
  OCR1B = 300;

}

// the setup routine runs once when you press reset:
int main(void) {
    // initialize the digital pin as an output.
    // pinMode(Electromagnet, OUTPUT);

    // set OCR1B bit as an output
    DDRB |= (1 << PB2);

    // enable global interrupts
    sei();

    timer1_init();
    Serial.begin(9600);


    while (1) {
      if (Serial.available() > 0) {
        tempo = Serial.parseInt();
      } 
    }
}


// the loop routine runs over and over again forever:

ISR(TIMER1_COMPA_vect) { 
  //OCR1B = pwm_width;
}