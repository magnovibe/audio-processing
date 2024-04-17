#include <LiquidCrystal.h>

volatile int pwm_width = 4000;
volatile float myTempo = 200;
volatile float timePerBeat = 0;
volatile float timeOn = 0;
volatile float timeOff = 0;
volatile int readyToCook = 0;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  lcd.begin(16, 2);
  // put your setup code here, to run once:
  uint16_t electromagnet = 2;
  pinMode(LED_BUILTIN, OUTPUT);
  // set IN3 to 1, IN4 already grounded
    DDRD |= (1 << PD0);
    DDRD |= (1 << PD7);
    PORTD |= (1 << PD7);
    // turn on ENA bit (for PWM) initially
    DDRB |= (1 << PB2);
    PORTB |= (1 << PB2);
    Serial.begin(9600);
    lcd.clear();
}
void loop() {
  // put your main code here, to run repeatedly:
  int magnetPin = 10;
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial.available() > 0) { // catch tempo
    // PORTB &= ~(1 << PB2);
    String pwm = Serial.readString();
    Serial.print(pwm);
    lcd.print(pwm);
    Serial.flush();
    delay(5000);
    lcd.clear();
    timePerBeat = 60 / myTempo;
    timeOn = timePerBeat * 0.8;
    timeOff = timePerBeat - timeOn;
    
    
  }
   /* if (myTempo != 200) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(5000);
    }*/
//  }else{
//   lcd.print("here");
//  }
 //if (readyToCook == 0) { // tempo caught -- init tempo
 //}
 //if (readyToCook == 1) { // get that beat going baby!
 /*
  analogWrite(magnetPin, 255);
  digitalWrite(LED_BUILTIN, 255);
  delay(timeOn * 1000);
  analogWrite(magnetPin, 80);
  digitalWrite(LED_BUILTIN, 0);
  delay(timeOff * 1000);
 */
 //}
}
