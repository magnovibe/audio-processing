#include <Adafruit_LiquidCrystal.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define LEDPIN            6   // Pin for Neopixel strip
#define NUMPIXELS         60  // Number of Neopixels
#define BAUDRATE          9600
#define BRIGHTNESS        100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

unsigned long previousMillis = 0;
const unsigned long interval = 50; // Color change interval

int bigMagnet = 10;
int littleMagnet = 11;
float timePerBeat = 0;
float timeOn = 0;
float timeOff = 0;
int initFlag = 1;
int readyToCook = 0;
float LEDtempo = 0;
int *bigMagArray = nullptr;
int *littleMagArray = nullptr;
int volArrayLenNum = 0; // Declare globally

unsigned long lastBigMagnetUpdate = 0;
unsigned long lastLittleMagnetUpdate = 0;
float bigMagnetOnDuration = 500;  // Duration in milliseconds
float littleMagnetOnDuration = 1000;

void setup() {
  lcd.begin(16, 2);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(bigMagnet, OUTPUT);
  pinMode(littleMagnet, OUTPUT);
  
  Serial.begin(9600);
  strip.begin();
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(185, 36, 255));
  }
  strip.show();
}

void loop() {
   unsigned long currentMillis = millis();

   if (initFlag == 1) {
    initFlag = 0;
   } else {
    if (Serial.available() > 0) {
      String dataString = Serial.readStringUntil('\n');
      parseSerialData(dataString);
    }

    if (readyToCook == 1) {
      pulseMagnets(currentMillis);
    }
  }
}

void parseSerialData(String data) {
  int delimiterOne = data.indexOf(':');
  int delimiterTwo = data.indexOf(':', delimiterOne + 1);
  int delimiterThree = data.indexOf(':', delimiterTwo + 1);

  String tempoString = data.substring(0, delimiterOne);
  String volArrayLen = data.substring(delimiterOne + 1, delimiterTwo);
  String bigMagArrayString = data.substring(delimiterTwo + 1, delimiterThree);
  String littleMagArrayString = data.substring(delimiterThree + 1);

  volArrayLenNum = volArrayLen.toInt();
  float tempo = tempoString.toFloat();

  delete[] bigMagArray;  // Free previous arrays
  bigMagArray = new int[volArrayLenNum];
  delete[] littleMagArray;
  littleMagArray = new int[volArrayLenNum];

  parseArrayData(bigMagArrayString, bigMagArray, volArrayLenNum);
  parseArrayData(littleMagArrayString, littleMagArray, volArrayLenNum);

  timePerBeat = 60 / tempo;
  timeOn = timePerBeat * 0.8;
  timeOff = timePerBeat - timeOn;

  if (timePerBeat < 1) {
    readyToCook = 1;
  }
}

void parseArrayData(String arrayString, int* array, int length) {
  int start = 0;
  int end = arrayString.indexOf(';');
  while (end != -1) {
    array[start] = arrayString.substring(start, end).toInt();
    start = end + 1;
    end = arrayString.indexOf(';', start);
  }
}

void pulseMagnets(unsigned long currentMillis) {
  if (currentMillis - lastBigMagnetUpdate >= bigMagnetOnDuration) {
    lastBigMagnetUpdate = currentMillis;
    for (int i = 0; i < volArrayLenNum; i++) {
      digitalWrite(bigMagnet, HIGH);
      delayMicroseconds(16 * bigMagArray[i]);  // Simulate variable pulse strength
      digitalWrite(bigMagnet, LOW);
    }
  }

  if (currentMillis - lastLittleMagnetUpdate >= littleMagnetOnDuration) {
    lastLittleMagnetUpdate = currentMillis;
    for (int i = 0; i < volArrayLenNum; i++) {
      digitalWrite(littleMagnet, HIGH);
      delayMicroseconds(littleMagArray[i]);
      digitalWrite(littleMagnet, LOW);
    }
  }
}
