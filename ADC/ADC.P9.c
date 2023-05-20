#include <msp430.h> 


/* Posição X será dada pela média de 4 converções do canal A0
 * Posição Y será dada pela média de 3 converções do canal A1
 * TB0.1 - conversões de 11 ms, após 7 converções
 * Terminal serial:
 * Colunas 0, 1, 2: média do eixo Y em decimal (3 dígitos);
 * Colunas 4, 5, 6: média do eixo X em decimal (3 dígitos);
 * Colunas 10, 11, ..., 41: são 32 posições para indicar o valor do eixo X com um asterisco.
 * Colunas 9, 25, 26 e 42: barra vertical para marcar os limites e a posição central.
 */

void configADC(void);
void configTB0(void);
void configUART(void);

void ser_str(char *pt);
void ser_char(char x);
void HEXA(int x);
void serial(void);
void tabuleiro(int media);

static int adc0[4];
static int adc1[3];

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    configTB0();
    configADC();
    configUART();

    __delay_cycles(1000);

    ser_str("Prova A4");
    ser_str("\n");
    ser_str("\r");

    while(1){
        while( (ADC12IFG&ADC12IFG7) == 0);
        while( (ADC12IFG&ADC12IFG8) == 0);
        while( (ADC12IFG&ADC12IFG9) == 0);
        while( (ADC12IFG&ADC12IFG10) == 0);
        while( (ADC12IFG&ADC12IFG11) == 0);
        while( (ADC12IFG&ADC12IFG12) == 0);
        while( (ADC12IFG&ADC12IFG13) == 0);

                adc0[0] = ADC12MEM7;
                adc1[0] = ADC12MEM8;
                adc0[1] = ADC12MEM9;
                adc1[1] = ADC12MEM10;
                adc0[2] = ADC12MEM11;
                adc1[2] = ADC12MEM12;
                adc0[3] = ADC12MEM13;

                serial();
                ser_str("\n");
                ser_str("\r");
    }

    return 0;
}

void serial(void){
    int media = 0;
    int i;

    for(i = 0;i<3;i++){ media += adc1[i]; }
    media = media/3;
    HEXA(media);
    ser_str(" ");

    media = 0;

    for(i=0;i<4;i++){ media += adc0[i]; }
    media = media/4;
    HEXA(media);
    ser_str("   ");
    tabuleiro(media);
}

void tabuleiro(int media){
    int i , aux;

    aux = (32*media)/255;

    for( i = 0;i<34;i++){

        if( i == aux){ ser_str("*"); }
        else {
            switch (i){
                case 0:  ser_str("|"); break;
                case 17: ser_str("|"); break;
                case 18: ser_str("|"); break;
                case 33: ser_str("|"); break;
                default: ser_str(" ");
            }
        }

    }

}

void HEXA(int x){
    int quociente,mult,rest0,rest1;

    quociente =  x / 16;
    mult = 16*quociente;
    rest0 = x - mult;

    x = quociente;

    quociente = x / 16;

    if(quociente == 0){ rest1 = x;}
    else {
        mult = 16*quociente;
        rest1 =  x - mult;}


    if(rest1 > 9){
        switch(rest1){
        case 10: ser_char(0x41); break;
        case 11: ser_char(0x42); break;
        case 12: ser_char(0x43); break;
        case 13: ser_char(0x44); break;
        case 14: ser_char(0x45); break;
        case 15: ser_char(0x46); break;
        }
        }
    else{ ser_char(0x30 + (char)rest1);}

    if(rest0 > 9){
        switch(rest0){
                case 10: ser_char(0x41); break;
                case 11: ser_char(0x42); break;
                case 12: ser_char(0x43); break;
                case 13: ser_char(0x44); break;
                case 14: ser_char(0x45); break;
                case 15: ser_char(0x46); break;
                }}
    else{ ser_char(0x30 + (char)rest0);}
}

// Imprimir uma string na serial
void ser_str(char *pt){
    int i=0;
    while (pt[i] != 0){
        ser_char(pt[i]);
        i++;
    }
}

//Imprimir char
void ser_char(char x){
    while ( (UCA1IFG & UCTXIFG) == 0 );  //Esperar Tx
    UCA1TXBUF=x;
}
void configADC(void){  //Pulsar ADC12SC para iniciar uma conversão
    ADC12CTL0  &= ~ADC12ENC;    //Desabilitar para configurar

    ADC12CTL0 =  ADC12ON;       //Ligar ADC

    ADC12CTL1 =  ADC12SHS_3  |   // Selecionar TB0.1 bit
                 ADC12SSEL_3 |   //Clock ADC = SMCLK
                 ADC12DIV_0  |   //Clock ADC Divisor = 1
                 ADC12CSTARTADD_1 |  //Resultado em ADC12MEM1
                 ADC12CONSEQ_3 ;   //Modo Sequência de canais

    ADC12CTL2  =  ADC12TCOFF |   // Desligar sensor temperatura
                  ADC12RES_0 ;   // Resolução 8-bit

    ADC12MCTL7 =  ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_0 ;    //CANAL AO
    ADC12MCTL8 =  ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_1 ;    //CANAL A1
    ADC12MCTL9 =  ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_0 ;    //CANAL AO
    ADC12MCTL10 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_1 ;    //CANAL A1
    ADC12MCTL11 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_0 ;    //CANAL AO
    ADC12MCTL12 = ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_1 ;    //CANAL A1
    ADC12MCTL13 = ADC12EOS |    //É O FIM
                  ADC12SREF_0 |    //VR+ = AVCC e VR- = AVSS
                  ADC12INCH_1 ;    //CANAL A1

    ADC12CTL0 |= ADC12ENC;      //Habilitar ADC12
}

void configTB0(void){
    TB0CTL = TASSEL_2 | MC_1;
    TB0CCTL1 = OUTMOD_6;
    TB0CCR0 = 23068; // 200 HZ (100 HZ POR CANAL)
    TB0CCR1 = TB0CCR0/2; // CARGA 50%

    P4DIR |= BIT0; //P4.0 como saída
    P4SEL |= BIT0; //P4.0 saída alternativa
    PMAPKEYID = 0X02D52; //Liberar mapeamento
    P4MAP0 = PM_TB0CCR1A; //TB0.1 disponibilizado em P4.0
}

void configUART(void){
    // Modo baixa frequência
    UCA1CTL1 = UCSWRST;  //RST=1
    UCA1CTL0 = 0;
    UCA1BRW= 18;           //BRX= 18
    UCA1MCTL = UCBRS_1;     //BRS = 1

    P4DIR |=  BIT4;         //P4.4 saída
    P4DIR &= ~BIT5;         //P4.5 entrada
    P4SEL |= BIT5 | BIT4;   //Selecionar UART

    UCA1CTL1 = UCSSEL_2;     //RST=0 e SMCLK
}
