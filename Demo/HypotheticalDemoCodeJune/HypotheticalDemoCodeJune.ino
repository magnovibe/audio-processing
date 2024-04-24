#include <Adafruit_NeoPixel.h>

#define PIN        6   // NeoPixel pin
#define NUMPIXELS  56  // Number of NeoPixels

volatile float myTempo = 0.5;
volatile float timePerBeat = 0.5;
volatile float timeOn = 0.4;
volatile float timeOff = 0.1;
volatile int readyToCook = 0;
int magnetPin1 = 10;
int magnetPin2 = 11;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
volatile int rainbowStep = 0;

void setup() {
  // Set pin 10 and pin 11 as output pins
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);

  pixels.begin();
}

void loop() {

  // For testing purposes, use fixed values
  while (readyToCook == 0) {
    myTempo = 117.453835;
    timePerBeat = 60 / myTempo;
    timeOn = timePerBeat * 0.5;
    timeOff = timePerBeat - timeOn;
    readyToCook = 1;
  }

  unsigned long zeroTime = millis();
  while (readyToCook == 1) {
    rainbowCycle();  // Call the rainbow cycle function

    // Control electromagnet 1
    analogWrite(magnetPin1, 255);
    analogWrite(magnetPin2, 80);

    while (millis() - zeroTime < timeOn*1000)
    {

    }
    
    analogWrite(magnetPin1, 80);
    analogWrite(magnetPin2, 255);
    
    while (millis() - zeroTime < timePerBeat*1000)
    {

    }

    zeroTime = zeroTime + timePerBeat*1000;
  }
}

// Function to generate rainbow colors across the entire strip
void rainbowCycle() {
  for(uint16_t i=0; i< pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + rainbowStep) & 255));
  }
  pixels.show();
  rainbowStep++;
  if (rainbowStep >= 256 * 5) rainbowStep = 0; // Reset rainbowStep
}

// Function to generate colors from the color wheel
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
