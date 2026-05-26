#define PINO PC0
#define PINO_LED PD6

unsigned long int LeituraAD = 0;
unsigned long int tensao = 0;
uint8_t dutyCycle = 0;

int main() {
    Serial.begin(9600);

    DDRD |= (1 << PD6);
    TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 0;

    // Configuração do ADC
    ADMUX = (1 << REFS0); // Seleciona AVcc (5V) como tensão de referência
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilita o ADC e define prescaler de 128

    for(;;) {
        ADMUX = (ADMUX & 0b11111000) + PINO; // Seleciona o canal de entrada do ADC

        ADCSRA |= (1 << ADSC); // Inicia a conversão

        while((ADCSRA & (1 << ADSC)) == (1 << ADSC)); // Aguarda a conversão finalizar (ADSC será zerado automaticamente)

        // Lê o valor de 10 bits do registrador ADC (ADCL + ADCH)
        LeituraAD = ADCL;
        LeituraAD |= (ADCH << 8);

        // Converte o valor lido (0–1023) para milivolts (0–5000 mV)
        tensao = (LeituraAD * 5000) / 1023;

        dutyCycle = (tensao * 255) / 5000;
        OCR0A = dutyCycle;

        // Exibe o valor da tensão convertida no monitor serial
        Serial.print(tensao);
        Serial.println("[mV]");
    }
}