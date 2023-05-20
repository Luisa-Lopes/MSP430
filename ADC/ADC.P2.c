/* Usar sequência de canais sem repetição
 * Converter entradas A0, A1, A2 e A3
 * Armazenar em ADC12MEM0, 1, 2 e 3
 * Disparar conversão por software (ADC12SC)
 */


#include <msp430.h> 

void configADC(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	volatile int adc0,adc1,adc2,adc3;

	configADC();

	ADC12CTL0 |= ADC12SC;

	while(1){

	    ADC12CTL0 &= ~ADC12SC;  //disp ADC
	    ADC12CTL0 |=  ADC12SC;
	    while( (ADC12IFG&ADC12IFG0) == 0);  //Esperar converter

	    ADC12CTL0 &= ~ADC12SC;  //disp ADC
	    ADC12CTL0 |=  ADC12SC;
	    while( (ADC12IFG&ADC12IFG1) == 0);  //Esperar converter

	    ADC12CTL0 &= ~ADC12SC;  //disp ADC
	    ADC12CTL0 |=  ADC12SC;
	    while( (ADC12IFG&ADC12IFG2) == 0);  //Esperar converter

	    ADC12CTL0 &= ~ADC12SC;  //disp ADC
	    ADC12CTL0 |=  ADC12SC;
	    while( (ADC12IFG&ADC12IFG3) == 0);  //Esperar converter


	    adc0=ADC12MEM0;
	    adc1=ADC12MEM1;
	    adc2=ADC12MEM2;
	    adc3=ADC12MEM3;

	}


	return 0;
}

void configADC(void){
    ADC12CTL0 &= ~ADC12ENC; //DESABILITAR PARA CONFIGURAR

    ADC12CTL0 = ADC12ON; // LIGAR ADC

    ADC12CTL1 = ADC12SHS_0 | //SELECIONA ADC12SC
                ADC12SSEL_3 | // SELECIONA SMCLK
                ADC12DIV_0 | // DIVISOR = 1
                ADC12CSTARTADD_0 | // ADC12MEM0
                ADC12CSTARTADD_1 |
                ADC12CSTARTADD_2 |
                ADC12CSTARTADD_3 |
                ADC12CONSEQ_1; // SELECIONA OS CANAIS

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_2;    // Resolução 12-bit

    ADC12MCTL0 = ADC12SREF_0 |   //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_0;
    ADC12MCTL1 = ADC12SREF_0 |   //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_1;
    ADC12MCTL2 = ADC12SREF_0 |   //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_2;
    ADC12MCTL3 =    ADC12EOS |   //ULTIMA POSIÇÃO
                ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_3;


        ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12

}
