#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define led_superloop (1<<PD5)
#define led_int (1<<PD4)

volatile uint8_t led_flagle = 0;

void setup()
{
   DDRD |= led_superloop | led_int ;

    // Configura o pino PD2 (INT0) como entrada com resistor pull-up ativado
    DDRD &= ~(1 << PD2);   // Define como entrada
    PORTD |= (1 << PD2);   // Ativa pull-up interno

    // Configura o pino PD3 (INT1) como entrada com resistor pull-up ativado
    DDRD &= ~(1 << PD3);   // Define como entrada
    PORTD |= (1 << PD3);   // Ativa pull-up interno

    // Configura a interrupção externa INT0 como transição de subida:
    EICRA |= (1 << ISC01); 
    EICRA |= (1 << ISC00);

    // Configura a interrupção externa INT1 como transição de descida:
    EICRA |= (1 << ISC11); 
    EICRA &= ~(1 << ISC10);

    EIMSK |= (1 << INT0) + (1<<INT1);  // Habilita a interrupção INT0 e INT1

    // Habilita as interrupções globais
    sei();
}

ISR (INT0_vect){
    led_flagle = 1;
}

ISR (INT1_vect){
    EIMSK ^= (1 << INT0);
}

int main()
{

  setup();

  while(1)
  {

    PORTD ^= led_superloop;
    _delay_ms(500);


    if(led_flagle){
      PORTD |= led_int;
      _delay_ms(1000);
      PORTD &= ~(led_int);
      led_flagle=0;
    }

  }
}
