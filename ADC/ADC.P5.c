
// CAnal A1 (P6.1) convertido
// Disparo por timer TA0.1 = 100 Hz
// 100 conversões por seg

#include <msp430.h> 
#include <stdio.h>

void configADC(void);
void configTA0(void);
void configUART(void);
void ser_str(char *pt);
void ser_char(char x);
void ser_dec8(char x);
void ser12bit(int x);

int main(void){
    volatile int adc1, adc2, adc3;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    configADC();
    configTA0();
    configUART();

    __delay_cycles(100);

    while(1){

        //ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
        while( (ADC12IFG&ADC12IFG5) == 0);  //Esperar converter

        adc1 = ADC12MEM5;
        adc2 = ADC12MEM6;
        adc3 = ADC12MEM7;

        ser_str("Eixo x: ");
        ser12bit(adc1);
        ser_str("\n");
        ser_str("\r");

        ser_str("Eixo y: ");
        ser12bit(adc3);
        ser_str("\n");
        ser_str("\r");

        ser_str("SW: ");
        ser12bit(adc2);
        ser_str("\n");
        ser_str("\r");
    }

    return 0;
}

//imprime a resolução de bits
void ser12bit(int x){
    int aux;

    aux = x/1000;
    ser_char(0x30 + (char)aux);

    x = x - aux*1000;
    aux = x/100;
    ser_char(0x30 + (char)aux);

    x = x - aux*100;
    aux = x/10;
    ser_char(0x30 + (char)aux);

    x = x - aux*10;
    aux = x;
    ser_char(0x30 + (char)aux);
}

//imprime o contador de 8 bits
void ser_dec8(char x){
    char aux;

    // centena
    aux=x/100;
    ser_char(0x30 + aux);

    // dezena
    x=x-aux*100;
    aux=x/10;
    ser_char(0x30 + aux);

    // unidade
    x=x-aux*10;
    ser_char(0x30 + x);
}

// Imprimir uma string na serial
void ser_str(char *pt){
    int i=0;
    while (pt[i] != 0){
        ser_char(pt[i]);
        i++;
    }
}

// Imprime x na porta serial
void ser_char(char x){
    while ( (UCA1IFG & UCTXIFG) == 0 );  //Esperar Tx
    UCA1TXBUF=x;
}

void configADC(void){  //Pulsar ADC12SC para iniciar uma conversão
    ADC12CTL0  &= ~ADC12ENC;     //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;            //Ligar ADC

    ADC12CTL1 = ADC12SHS_1  |   // Disparo por TA0.1
                ADC12SSEL_3 |   //Clock ADC = SMCLK
                ADC12DIV_0  |   //Clock ADC Divisor = 1
                ADC12CSTARTADD_5 |    //Resultado em ADC12MEM5
                ADC12CONSEQ_3;   //Modo Sequência  de canais REPETIDO

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_2;    // Resolução 12-bit

    ADC12MCTL5 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1;     //(P6.1) A1 = Canal 1 (P6SEL |= BIT1)

    ADC12MCTL6 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_2;     //(P6.2) A2 = Canal 2 (P6SEL |= BIT2)

    ADC12MCTL7 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_5;     //(P6.5) A5 = Canal 5 (P6SEL |= BIT5)

    ADC12MCTL7 |= ADC12EOS;

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
}


void configTA0(void){
    TA0CTL = TASSEL_1 | MC_1;
    TA0CCR0 = 32768/100;    //100 Hz
    TA0CCTL1 = OUTMOD_6;
    TA0CCR1 = TA0CCR0/2;    //50%
}

void configUART(void){
    UCA1CTL1 = UCSWRST;  //RST=1
    UCA1CTL0 = 0;
    UCA1BRW= 109;           //BRX=109
    UCA1MCTL = UCBRS_2;     //BRS=2

    P4DIR |=  BIT4;         //P4.4 saída
    P4DIR &= ~BIT5;         //P4.5 entrada
    P4SEL |= BIT5 | BIT4;   //Selecionar UART

    UCA1CTL1 = UCSSEL_2;     //RST=0 e SMCLK
}
