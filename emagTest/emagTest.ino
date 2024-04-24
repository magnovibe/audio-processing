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
// if (Serial.available() > 0) {
//    PORTB &= !(1 << PB2);
//    String pwm = Serial.readString();
//    delay(4000);
//    Serial.print(pwm);
//    Serial.end();
//    myTempo = pwm.toFloat();
//    timePerBeat = 60 / myTempo;
//    timeOn = timePerBeat * 0.8;
//    timeOff = timePerBeat - timeOn;
//    readyToCook = 1;
// }

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
