volatile int pwm_width = 4000;
uint16_t tempo = 0;
volatile float myTempo = 0;
volatile float timePerBeat = 0;
void setup() {
  // put your setup code here, to run once:
  // put your main code here, to run repeatedly:
  uint16_t electromagnet = 2;

  // PWM width will vary depending on loudness, for now it's constant

  // initialize the digital pin as an output.
    // pinMode(Electromagnet, OUTPUT);

    // set OCR1B bit as an output
    DDRB |= (1 << PB2) | (1 << PB5);

    Serial.begin(9600);

    // enable global interrupts
    sei();

    //timer1_init();
    PORTB |= (1 << PB2);
    PORTB |= (1 << PB5);
    
}

void loop() {
 if (Serial.available() > 0) {
    String pwm = Serial.readString();
    Serial.print(pwm);
    myTempo = pwm.toFloat();
    timePerBeat = 60 / myTempo;
    OCR1A = 16000000 * timePerBeat / 1024;
    OCR1B = OCR1A * 0.8;
 }
  
}


void timer1_init() {
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1B |= (1 << CS10) | (1 << CS12);
  TCCR1A |= (1 << WGM11) | (WGM10);
  TCCR1A |= (1 << COM1B1);
  OCR1A = 40000;

  // PB2 (D10) = PULSE OUTPUT

}
