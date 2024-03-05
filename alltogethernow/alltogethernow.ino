volatile int pwm_width = 4000;
uint16_t tempo = 0;
volatile float timePerBeat = 0;
volatile float timeOn = 0;
volatile float timeOff = 0;
int state = 0;

#include <Adafruit_NeoPixel.h>

#define LEDPIN            6   // Define the pin to which the Neopixel strip is connected
#define MAGNETPIN 10 // Define magnet pin
#define NUMPIXELS      60  // Define the number of Neopixels in your strip
#define BAUDRATE 9600 // Define BAUDRATE

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

float tempo = 0.0;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;

void setup() {
  // put your setup code here, to run once:
  // put your main code here, to run repeatedly:
  uint16_t electromagnet = 2;

  // PWM width will vary depending on loudness, for now it's constant

    // set IN3 to 1, IN4 already grounded
    DDRD |= (1 << PD7);
    PORTD |= (1 << PD7);

    // turn on ENA bit (for PWM) initially
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);
    Serial.begin(9600);

    // Wait for tempo information in the setup
  while (!Serial.available()) {
    // Wait for serial data
  }

  // Read the tempo value from the serial port
  tempo = Serial.parseFloat()*1000; // NOTE: Depends if it returns in ms or s
  timePerBeat = 60 / tempo;
  timeOn = timePerBeat * 0.8;
  timeOff = timePerBeat - timeOn;

  // Print the received tempo for debugging
  Serial.print("Received Tempo: ");
  Serial.println(tempo);

  strip.begin();
  strip.show();  // Initialize all pixels to 'off'
}

void loop() {

    unsigned long currentMillis2 = millis();
    if (((currentMillis2 - previousMillis2) <= timeOn*1000) && (state == 0)){
    analogWrite(MAGNETPIN, 255);
    state = 1;
    }
    else if (((currentMillis2 - previousMillis2) <= (timeOn*1000 + timeOff*1000)) && (state == 1)){
    analogWrite(MAGNETPIN, 80);
    state = 2;
    }
    else {
    previousMillis2 = currentMillis2;
    state = 0;
    }
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= tempo) {
      // Save the current time
      previousMillis = currentMillis;

      int lightLevel = ((currentMillis - previousMillis) / tempo) * 255;

      if (lightLevel <= 255) {
        color(strip.Color(lightLevel, 0, 0));
      }
    }
}