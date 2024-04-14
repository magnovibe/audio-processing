volatile float myTempo = 0.5;
volatile float timePerBeat = 0.5;
volatile float timeOn = 0.4;
volatile float timeOff = 0.1;
volatile int readyToCook = 0;
int magnetPin1 = 10;
int magnetPin2 = 11;

void setup() {
  // Set pin 10 and pin 11 as output pins
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // Initialize Serial communication
  Serial.begin(9600);
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
    // Control electromagnet 1
    analogWrite(magnetPin1, 150);
    analogWrite(magnetPin2, 80);

    while (millis() - zeroTime < timeOn*1000)
    {

    }
    
    analogWrite(magnetPin1, 60);
    analogWrite(magnetPin2, 255);
    
    while (millis() - zeroTime < timePerBeat*1000)
    {

    }

    zeroTime = zeroTime + timePerBeat*1000;
  }
}
