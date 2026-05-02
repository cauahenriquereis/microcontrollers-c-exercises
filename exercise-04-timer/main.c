#define intervalo 24999
#define ledvermelho (1<<PD0)
#define ledverde (1<<PD1)

volatile uint8_t contTicks  = 0;
volatile uint8_t contSegundos = 0;
volatile uint8_t ledVerde=0;

void configTimer1(){
  
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<CS11) | (1<<CS10); 
  OCR1A = intervalo;
  TIMSK1 |= (1<<OCIE1A);
}

void disableTimer1(){
  TCCR1B = 0;
}

void setup(){
  DDRD |= ledvermelho | ledverde;
  PORTD &= ~(ledverde | ledvermelho);
  configTimer1();
  sei();
}

ISR(TIMER1_COMPA_vect){
  contTicks++;

  if(contTicks == 1){
    PORTD |= ledvermelho;   
  }
  if(contTicks == 2){
    PORTD &= ~ledvermelho;  
  }

  if(contTicks >= 10){
    contTicks = 0;
    contSegundos++;

    if(contSegundos >= 60){
      contSegundos = 0;
      PORTD |= ledverde;  
      ledVerde = 1;
    }
  }

   if(ledVerde == 1 && contTicks == 1){
      PORTD &= ~ledverde; 
      ledVerde = 0;
  }
   
}

int main(){
  setup();
  while(1){
  }
}