#include <LiquidCrystal.h>

#define LEDPIN            6   // Define the pin to which the Neopixel strip is connected
#define NUMPIXELS      60  // Define the number of Neopixels in your strip
#define BAUDRATE 9600 // Define BAUDRATE

#include <Adafruit_NeoPixel.h>

#define BRIGHTNESS  100   // Set brightness (0 to 255)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
const unsigned long interval = 50; // Interval between color changes


volatile int bigMagnet = 10;
volatile int littleMagnet = 11;

volatile float timePerBeat = 0;
volatile float timeOn = 0;
volatile float timeOff = 0;
volatile int initFlag = 1;
volatile int readyToCook = 0;
volatile int lightLevel = 0;
volatile float LEDtempo = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
volatile int *bigMagArray = nullptr;
volatile int *littleMagArray = nullptr;



void setup() {
  lcd.begin(16, 2);
  // put your setup code here, to run once:
  uint16_t electromagnet = 2;
  pinMode(LED_BUILTIN, OUTPUT);
  // set IN3 to 1, IN4 already grounded
    DDRD |= (1 << PD7);
    PORTD |= (1 << PD7);
    // turn on ENA bit (for PWM) initially
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);
    DDRB |= (1 << PB3);
    PORTB |= (1 << PB3);
    delay(1000);
    lcd.clear();
    Serial.begin(9600);
    strip.begin();
    for(int i = 0; i < strip.numPixels();i++) {
      strip.setPixelColor(i, strip.Color(185,36,255));
      strip.show();
    }
    strip.show();  // Initialize all pixels to 'off'
}
void loop() {
   int magnetPin = 10;
   String tempoString;
   String volArrayLen;
   String bigMagArrayString;
   String littleMagArrayString;
   float tempo;
   int volArrayLenNum;
   if (initFlag == 1) {
    initFlag = 0;
    tempoString = "";
    volArrayLen = "";
    bigMagArrayString = "";
    littleMagArrayString = "";
    tempo = 0;
    volArrayLenNum = 0;
    magnetPin = 10;
   } else {
  
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial.available() > 0) { // catch tempo
    String dataString = Serial.readString();
    Serial.print(dataString);
    Serial.flush();
    Serial.print("yay");
    Serial.flush();

    int delimiterOne = 0;
    int delimiterTwo = 0;
    int delimiterThree = 0;

    
    while (dataString[delimiterOne] != ':') {
      tempoString = tempoString + dataString[delimiterOne];
      delimiterOne++;
    }

    delimiterTwo = delimiterOne + 1;

    while(dataString[delimiterTwo] != ':') {
      volArrayLen = volArrayLen + dataString[delimiterTwo];
      delimiterTwo++;
    }

    delimiterThree = delimiterTwo + 1;

    while(dataString[delimiterThree] != ':') {
      bigMagArrayString = bigMagArrayString + dataString[delimiterThree];
      delimiterThree++;
    }

    dataString.remove(0, delimiterThree);
    littleMagArrayString = dataString;

    tempo = tempoString.toFloat(); 
    volArrayLenNum = volArrayLen.toFloat();
    
   // initialize array & set all vals to 0
    bigMagArray = new int[volArrayLenNum];
    for (int i = 0; i < volArrayLenNum; i++) {
      bigMagArray[i] = 0;
    }

    littleMagArray = new int[volArrayLenNum];
    for (int i = 0; i < volArrayLenNum; i++) {
      littleMagArray[i] = 0;
    }

    int arrayIndexCounter = 0;
    String currVolume = "";
    int currVolumeNum = 0;

    // BIG MAG ARRAY PROCESSING
    for (int i = 0; i < bigMagArrayString.length(); i++) { // go thru string vol array 
      if (bigMagArrayString[i] != ';') { // if delimiter not hit, add to currVolume string
        currVolume = currVolume + bigMagArrayString[i];
      } else { // delimiter hit, turn currVolume to float, add to array, increment array index & clear string
        currVolumeNum = currVolume.toFloat();
        bigMagArray[arrayIndexCounter] = currVolumeNum;
        currVolume = "";
        arrayIndexCounter++;
      }
    }

    // LITTLE MAG ARRAY PROCESSING
    arrayIndexCounter = 0;
    for (int i = 0; i < littleMagArrayString.length(); i++) { // go thru string vol array 
      if (littleMagArrayString[i] != ';') { // if delimiter not hit, add to currVolume string
        currVolume = currVolume + littleMagArrayString[i];
      } else { // delimiter hit, turn currVolume to float, add to array, increment array index & clear string
        currVolumeNum = currVolume.toFloat();
        littleMagArray[arrayIndexCounter] = currVolumeNum;
        currVolume = "";
        arrayIndexCounter++;
      }
    }

    
    Serial.print(tempoString);
    Serial.print("volLen");
    Serial.print(volArrayLen);
    Serial.print(" ");
    Serial.print(littleMagArrayString);
    Serial.flush();

    timePerBeat = 60 / tempo;
    timeOn = timePerBeat * 0.8;
    timeOff = timePerBeat - timeOn;
    

    if (timePerBeat < 1) {
      readyToCook = 1;
      Serial.print("valid");
    }
    
  }
  if (readyToCook == 1) {
    for (int i = 0; i < volArrayLenNum; i++) {
      Serial.print(String(bigMagArray[i]));
      if (i%2 == 0) { // "on" beat, short delay
        unsigned long zeroTime = millis();
        analogWrite(bigMagnet, bigMagArray[i]);
        analogWrite(littleMagnet, littleMagArray[i]);
        digitalWrite(LED_BUILTIN, HIGH);
        while (millis() - zeroTime < timeOff*1000) {  }
      } else {
        unsigned long zeroTime = millis();
        analogWrite(bigMagnet, bigMagArray[i]);
        analogWrite(littleMagnet, littleMagArray[i]);
        digitalWrite(LED_BUILTIN, LOW);
        while (millis() - zeroTime < timeOn*1000) {  }
        }
      }
    }
  }
}

