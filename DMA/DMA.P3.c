#include <msp430.h> 

#define TRUE 1
#define FALSE 0

#define UCA0TXBUF_ADR (__SFR_FARPTR) 0x05CE
#define UCA1RXBUF_ADR (__SFR_FARPTR) 0x060C
#define QTD 200

/*
 * USCI A0: P3.3 = TX E P3.4 = RX
 * USCI A1: P4.2 = TX E P4.1 = RX
 *
 */

void configDMA0(void);   // CONFIGURA DMA0
void configDMA1(void);   // CONFIGURA DMA1
void usciA0CONFIG(void); // CONFIGURA A0
void usciA1CONFIG(void); // CONFIGURA A1
char testaSERIAL(void);  // TESTA SERIAL
char compVET(void);      // COMPARA VETORES
void initVET(void);      // INICIALIZA VETORES
void initLED(void);      // INICIALIZA LEDS

volatile char v0[QTD], v1[QTD];

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	initLED();
	usciA0CONFIG();
	usciA1CONFIG();

	if(testaSERIAL() == FALSE)
	        while(TRUE);

	initVET();
    configDMA1();
    configDMA0();

    UCA0TXBUF = v0[0]; // ENVIA O PRIMEIRO BYTE
    while((DMA1CTL&DMAIFG) == 0);

    if(compVET() == TRUE) P4OUT |= BIT7;
    else                  P1OUT |= BIT0;
    while(TRUE);

	return 0;
}

char testaSERIAL(void){
    char i;
    for(i=0; i<QTD; i++){
        while((UCA0IFG & UCTXIFG) == 0);
        UCA0TXBUF = i;
        while((UCA1IFG&UCRXIFG) == 0);
        if(UCA1RXBUF != i){
            P1OUT |= BIT0;
            return FALSE;
        }
    }
    return TRUE;
}

void configDMA0(void){
    DMACTL0 |= DMA0TSEL_17; //UCA0TXIFG
    DMA0CTL =    DMADT_0 |  //MODO SIMPLES
            DMADSTINCR_0 |  //DESTINO FIXO
            DMASRCINCR_3 |  //INCREMENTA A FONTE
              DMADSTBYTE |  //DESTINO EM BYTE
              DMASRCBYTE ;  //FONTE EM BYTE
    DMA0SA = &v0[1] ;
    DMA0DA = UCA0TXBUF_ADR;
    DMA0SZ = QTD-1;
    DMA0CTL |= DMAEN; // HABILITA DMA
}
void configDMA1(void){
    DMACTL0 |= DMA1TSEL_20; //UCA0TXIFG
    DMA1CTL =    DMADT_0 |  //MODO SIMPLES
            DMADSTINCR_3 |  //INCREMENTA O DESTINO
            DMASRCINCR_0 |  //FONTE FIXA
              DMADSTBYTE |  //DESTINO EM BYTE
              DMASRCBYTE ;  //FONTE EM BYTE
    DMA1SA = UCA1RXBUF_ADR;
    DMA1DA = v1;
    DMA1SZ = QTD;
    DMA1CTL |= DMAEN; // HABILITA DMA
}

//CONFIG USCI A0
// P3.3 = TX E P3.4 = RX
void usciA0CONFIG(void){
    UCA0CTL1 = UCSSEL_2 | UCSWRST;
    UCA0IFG = 0;
    UCA0BRW = 6;
    UCA0MCTL = UCBRF_13 | UCBRS_0 | UCOS16;
    UCA0CTL0 = 0;
    P3SEL |= BIT3;
    P3SEL |= BIT4;
    UCA0CTL1 &= ~UCSWRST;
}

//CONFIG USCI A1
// P4.1 = RX E P4.2 = TX
void usciA1CONFIG(void){
    UCA1CTL1 = UCSSEL_2 | UCSWRST;
    UCA1IFG = 0;
    UCA1BRW = 6;
    UCA1MCTL = UCBRF_13 | UCBRS_0 | UCOS16;
    UCA1CTL0 = 0;
    P4SEL |= BIT2;
    P4SEL |= BIT1;
    PMAPKEYID = 0X02D52;
    P4MAP1 = PM_UCA1RXD;
    P4MAP2 = PM_UCA1TXD;
    UCA1CTL1 &= ~UCSWRST;
}
void initVET(void){
    char i;
    for (i = 0; i < QTD; i++){
        v0[i] = i;
        v1[i] = 0;
    }
}
char compVET(void){
    char i;
    for(i=0; i<QTD; i++){
        if(v0[i] != v1[i]){P1OUT |= BIT0; return FALSE;} }
    return TRUE;
}
void initLED(void){
    P1DIR |= BIT0;P1OUT &= ~BIT0;
    P4DIR |= BIT7;P4OUT &= ~BIT7;
}
