#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
  pinMode(3, OUTPUT); // Output pin for OCR2B

   TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
   TCCR2B = _BV(WGM22) | _BV(CS21) | _BV(CS22);                     // prescaler to 256
   OCR2A = 128;                                                     // 485 hz
   OCR2B = 32;                                                      // duty cycle
}
