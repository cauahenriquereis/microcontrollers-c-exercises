#include <avr/io.h>
#include <util/delay.h>

#define MotorA (1<<PD0)
#define MotorF (1<<PD1)
#define SensorA (1<<PC0)
#define SensorF (1<<PC1)
#define Botao (1<<PC3)


int main(){

  DDRD = MotorA + MotorF;
  PORTD &= ~(MotorA + MotorF);

  DDRC &= ~(SensorA | SensorF | Botao);

  uint8_t estado_ant =0;
  uint8_t motor_abrindo =0;
  uint8_t motor_fechando =0;
  uint8_t botao_ant = 0;
  uint8_t motor_parado = 0;


  while(1){

    uint8_t TotalA = PINC & SensorA;
    uint8_t TotalF = PINC & SensorF;
    uint8_t botao = PINC & Botao;

    if(TotalA!=0 && motor_parado ==0) {
      PORTD &= ~(MotorA);
      PORTD |= MotorF;
      motor_fechando = 1;
      motor_abrindo = 0;
    }

    else if(TotalF!=0 && motor_parado ==0){
      PORTD &= ~(MotorF);
      PORTD |= MotorA;
      motor_abrindo = 1;
      motor_fechando = 0;
    }

    if((botao!=0) && (botao_ant == 0) && (motor_parado == 0)){


      if(motor_abrindo == 1){
        PORTD &= ~(MotorA);
        estado_ant = 0;
        motor_abrindo = 0;
        motor_parado = 1;
      } 
        
      else if(motor_fechando == 1){
         PORTD &= ~(MotorF);
         estado_ant = 1;
         motor_fechando = 0;
         motor_parado = 1;
      }

    }

      else if((PINC & Botao) != 0 && botao_ant == 0 && motor_parado == 1){

        if(estado_ant == 0){
          PORTD |= (MotorF);
          motor_fechando = 1;
          motor_parado = 0;
        } 

        else{
          PORTD |= (MotorA);
          motor_abrindo = 1;
          motor_parado = 0;
          
        } 
      }

       botao_ant = PINC & Botao;
     
    _delay_ms(50);

   

}

}