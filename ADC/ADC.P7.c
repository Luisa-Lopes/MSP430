#include <msp430.h> 


/* CONVERSÕES DISPARADAS PELO TIMER TA0
 * P6.0 P6.1 P6.2 P6.3
 *
 * ADC12MEM0 = conversão da entrada A3 (P6.3);
 * ADC12MEM1 = conversão da entrada A2 (P6.2);
 * ADC12MEM2 = conversão da entrada A1 (P6.1);
 * ADC12MEM3 = conversão da entrada A0 (P6.0);
 */

void configTA0(void);
void configADC(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile int adc0,adc1,adc2,adc3;

	void confidTA0();
	void configADC();


	while(1){
	    while( (ADC12IFG&ADC12IFG0) == 0);

	    adc3 = ADC12MEM0;
	    adc2 = ADC12MEM1;
	    adc1 = ADC12MEM2;
	    adc0 = ADC12MEM3;
	}

	return 0;
}

void configADC(void){  //Pulsar ADC12SC para iniciar uma conversão
    ADC12CTL0  &= ~ADC12ENC;    //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;       //Ligar ADC

    ADC12CTL1 =  ADC12SHS_1  |   // Selecionar TA0  bit
                 ADC12SSEL_3 |   //Clock ADC = SMCLK
                 ADC12DIV_0  |   //Clock ADC Divisor = 1
                 ADC12CSTARTADD_0 |  //Resultado em ADC12MEM0
                 ADC12CONSEQ_1 ;   //SEQUÊNCIA DE CANAIS, UMA UNICA CONVERSÃO

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_0 ;    // Resolução 8-bit


    ADC12MCTL0 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_3 ;
    ADC12MCTL1 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_2 ;
    ADC12MCTL2 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1 ;
    ADC12MCTL3 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_0 ;

    ADC12MCTL3 |= ADC12EOS;

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
}

void configTA0(void){
    TA0CTL = TASSEL_1 | MC_1;
    TA0CCTL1 = OUTMOD_6;
    TA0CCR0 = 32767/20; // 20 HZ (10 HZ POR CANAL)
    TA0CCR1 = TA0CCR0/2; // CARGA 50%
}
