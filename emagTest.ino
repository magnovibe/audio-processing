volatile int pwm_width = 4000;
uint16_t tempo = 0;
volatile float myTempo = 0;
volatile float timePerBeat = 0;
volatile float timeOn = 0;
volatile float timeOff = 0;
volatile int readyToCook = 0;

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

    
}

void loop() {
 int magnetPin = 10;

 // code for serial... not working rn for some reason
if (Serial.available() > 0) {
   PORTB &= ~(1 << PB2);

   String pwm = Serial.readString();
   delay(4000);

   String volArrayLen = Serial.readString();

   String volArrayString = Serial.readString();

   Serial.end();

    // vol array len string to int
   int volArrayNum = volArrayLen.toFloat();
   // initialize array & set all vals to 0
    float volArray[volArrayNum];
    for (int i = 0; i < volArrayNum; i++) {
      volArray[i] = 0;
    }

    // parse vol string into values for array
    int arrayIndexCounter = 0;
    String currVolume = "";
    
    for (int i = 0; i < volArrayString.len(); i++) { // go thru string vol array 
      if (volArrayString[i] != ';') { // if delimiter not hit, add to currVolume string
        currVolume = currVolume + volArrayString[i];
      } else { // delimiter hit, turn currVolume to float, add to array, increment array index & clear string
        float currVolumeNum = currVolume.toFloat();
        volArray[arrayIndexCounter] = currVolumeNum;
        currVolume = "";
        arrayIndexCounter++;
      }
    }

    // set up tempo on/off
   myTempo = pwm.toFloat();

   timePerBeat = 60 / myTempo;
   timeOn = timePerBeat * 0.8;
   timeOff = timePerBeat - timeOn;
   readyToCook = 1;
}

if (readyToCook == 0) {
  myTempo = 117.453835;
  timePerBeat = 60 / myTempo;
  timeOn = timePerBeat * 0.8;
  timeOff = timePerBeat - timeOn;
  readyToCook = 1;
 }
 if (readyToCook == 1) {
  analogWrite(magnetPin, 255);
  delay(timeOn * 1000);
  analogWrite(magnetPin, 80);
  delay(timeOff * 1000);
}

}