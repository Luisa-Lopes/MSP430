#include <msp430.h> 

// Ler sinais de um joystick
// Armazenar em ADC12MEM1 e 2
// Convers�o em 100 Hz por canal
// Usar TA0.1

#define QTD 500 //Nr de amostras por canal

void configTA0(void);
void configADC(void);

volatile int vrx[QTD]; //Vetor x
volatile int vry[QTD]; //Vetor y
volatile int ix, iy; //Indexadores

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configTA0();
	configADC();

	 __enable_interrupt();

	 while(1){
	 while ((P3IN&BIT4) == BIT4); //Esperar SW

	 ix=iy=0; //Zerar indexadores

	 ADC12CTL0 |= ADC12ENC; //Habilitar ADC

	 while (iy<QTD); //Esperar QTD convers�es

	 ADC12CTL0 &= ~ADC12ENC; //Parar as convers�es

	 }

	return 0;
}


#pragma vector = 54
__interrupt void adc_int(void){
 vrx[ix++]= ADC12MEM1;
 vry[iy++]= ADC12MEM2;
 ADC12IV;
}

void configADC(void){  //Pulsar ADC12SC para iniciar uma convers�o
    ADC12CTL0  &= ~ADC12ENC;    //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;       //Ligar ADC

    ADC12CTL1 =  ADC12SHS_1  |   // Timer A0
                 ADC12SSEL_3 |   //Clock ADC = SMCLK
                 ADC12DIV_0  |   //Clock ADC Divisor = 1
                 ADC12CSTARTADD_1 |  //Resultado em ADC12MEM1
                 ADC12CONSEQ_3 ;   //Modo Single

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_0 ;    // Resolu��o 8-bit

    ADC12MCTL1 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1 ;
    ADC12MCTL2 =    ADC12EOS |  //� O FIM
                 ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_2 ;

    P6SEL |= BIT2|BIT1;

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12

    ADC12IE |= ADC12IE2; //Hab interrup��o MEM2
}

void configTA0(void){
    TA0CTL = TASSEL_1 | MC_1;
    TA0CCTL1 = OUTMOD_6;
    TA0CCR0 = 32767/200; // 200 HZ (100 HZ POR CANAL)
    TA0CCR1 = TA0CCR0/2; // CARGA 50%
}
