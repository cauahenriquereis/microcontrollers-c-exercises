#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define led_superloop (1<<PD5)
#define led_int0 (1<<PD7)
#define led_int2 (1<<PD6)
#define led_int20 (1<<PB1)

//PCINT 20 = PD4
//PCINT 2 = PB2
//PCINT 0 = PB0

volatile uint8_t flag0 = 0;
volatile uint8_t flag2 = 0;
volatile uint8_t flag20 = 0;

  ISR(PCINT0_vect)
{
  if((PINB & (1<<PB0)) == 0){
    flag0 = 1;
  } 

  if((PINB & (1<<PB2)) == 0){
    flag2 = 1;
  } 
}

ISR(PCINT2_vect)
{
  flag20 = 1;
}

void setup(){

  
  DDRD |= led_superloop | led_int0 | led_int2;
  DDRB |= led_int20;

  PORTB |= (1<<PB0) | (1<<PB2);
  PORTD |= (1<<PD4);

  PCICR |= (1<<PCIE2) | (1<<PCIE0);
  PCMSK0 |= (1<<PB0) | (1<<PB2);
  PCMSK2 |= (1<<PD4);


  sei();
}

int main(){

  setup();

  while (1){
    PORTD ^= led_superloop;
    _delay_ms(250);

    if (flag0 == 1)
    {
      PORTD |= led_int0;
      _delay_ms(1000);
      PORTD &= ~(led_int0);
      flag0 = 0;
    }

    if (flag2 == 1)
    {
      PORTD |= led_int2;
      _delay_ms(500);
      PORTD &= ~(led_int2);
      flag2 = 0;
    }

      if (flag20 == 1)
    {
      PORTB |= led_int20;
      _delay_ms(2000);
      PORTB &= ~(led_int20);
      flag20 = 0;
    }
  }
}

