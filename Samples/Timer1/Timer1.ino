
// Example of modulating a 38 KHz frequency duty cycle by reading a potentiometer
// Author: Nick Gammon
// Date: 24 September 2012

const byte POTENTIOMETER = A0;
const byte LED = 10;  // Timer 1 "B" output: OC1B

// 16 MHz clock divided by 38 KHz frequency desired
const long timer1_OCR1A_Setting = 16000000L / 38000L;

void setup()
{
 pinMode (LED, OUTPUT);

 // set up Timer 1 - gives us 38.005 KHz
 // Fast PWM top at OCR1A
 TCCR1A = _BV (WGM10) | _BV (WGM11) | _BV (COM1B1); // fast PWM, clear OC1B on compare
 TCCR1B = _BV (WGM12) | _BV (WGM13) | _BV (CS10);   // fast PWM, no prescaler
 OCR1A =  timer1_OCR1A_Setting - 1;                 // zero relative  
 }  // end of setup

void loop()
 {
  // alter Timer 1 duty cycle in accordance with pot reading
  OCR1B = (((long) (analogRead (POTENTIOMETER) + 1) * timer1_OCR1A_Setting) / 1024L) - 1;
 
 // do other stuff here
 }


