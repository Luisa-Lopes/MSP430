// Canal A1 (P6.1) convertido
// Disparo por SW (bit ADC12SC)
// Uma única conversão por vez

#include <msp430.h> 

void config_adc(void);

int main(void){
    volatile int adc1;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    config_adc();

    ADC12CTL0 |= ADC12SC;

    while(1){

        ADC12CTL0 &= ~ADC12SC;  //disp ADC
        ADC12CTL0 |=  ADC12SC;
        while( (ADC12IFG&ADC12IFG5) == 0);  //Esperar converter
        adc1=ADC12MEM5;
    }

    return 0;
}


void config_adc(void){  //Pulsar ADC12SC para iniciar uma conversão
    ADC12CTL0  &= ~ADC12ENC;     //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;            //Ligar ADC

    ADC12CTL1 = ADC12SHS_0  |   // Selecionar ADC12SC bit
                ADC12SSEL_3 |   //Clock ADC = SMCLK
                ADC12DIV_0  |   //Clock ADC Divisor = 1
                ADC12CSTARTADD_5 |  //Resultado em ADC12MEM5
                ADC12CONSEQ_0;   //Modo Single

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_2;    // Resolução 12-bit

    ADC12MCTL5 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1;      //(P6.1) A1 = Canal 1 (P6SEL |= BIT1)

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
}
