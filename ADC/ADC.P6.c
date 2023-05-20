#include <msp430.h> 

/* CONVERSÕES DISPARADAS POR TA0.1 -> 10HZ
 * CANAL SEM REPETIÇÃO
 * ENTRADA A0 - P6.0
 * MEMÓRIA 0
 * P6.0 à terra ou ao 3,3 V
 */

void configTA0(void);
void configADC(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configADC();
	configTA0();

	volatile int adc;

	while(1){
	    while( (ADC12IFG&ADC12IFG0) == 0);
	   adc = ADC12MEM0;
	}

	return 0;
}

void configADC(void){  //Pulsar ADC12SC para iniciar uma conversão
    ADC12CTL0  &= ~ADC12ENC;    //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;       //Ligar ADC

    ADC12CTL1 =  ADC12SHS_1  |   // Selecionar ADC12SC bit
                 ADC12SSEL_3 |   //Clock ADC = SMCLK
                 ADC12DIV_0  |   //Clock ADC Divisor = 1
                 ADC12CSTARTADD_0 |  //Resultado em ADC12MEM0
                 ADC12CONSEQ_0 ;   //ÚNICO CANAL, UMA UNICA CONVERSÃO

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_0 ;    // Resolução 8-bit

    ADC12MCTL0 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_0 ;

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
}

void configTA0(void){
    TA0CTL = TASSEL_1 | MC_1;
    TA0CCTL1 = OUTMOD_6;
    TA0CCR0 = 32767/20; // 20 HZ (10 HZ POR CANAL)
    TA0CCR1 = TA0CCR0/2; // CARGA 50%
}
