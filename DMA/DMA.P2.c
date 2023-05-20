/* UCA0 - TRANSMITE POR SOFTWARE
 * UCA1 - RECEBE POR DMA1
 *
 * USCI A0: P3.3 = TX E P3.4 = RX
 * USC1 A1: P4.2 = TX E P4.1 = RX
 */

#include <msp430.h> 

#define TRUE 1
#define FALSE 0
#define QTD 200

#define UCA1RXBUF_ADR (__SFR_FARPTR) 0x060C

void configDMA1(void);   // CONFIGURA DMA
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
	
	int i;

	initLED();
	usciA0CONFIG();
	usciA1CONFIG();

	if(testaSERIAL() == FALSE)
	    while(TRUE);

	initVET();
	configDMA1();

	for(i=0; i<QTD; i++){
	    while((UCA0IFG&UCTXIFG) == 0);

	    UCA0TXBUF = v0[i];
	}
	while((DMA1CTL&DMAIFG) == 0);

	if(compVET() == TRUE) P4OUT |= BIT7;
	else { P1OUT |= BIT0;}

	while(TRUE){
	v1[200];
	}

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
//CONFIG USCI A0
// P3.3 = TX E P3.4 = RX
void usciA0CONFIG(void){
    UCA0CTL1 = UCSSEL_2 |UCSWRST;
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
    UCA1CTL1 = UCSSEL_2 |UCSWRST;
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

void configDMA1(void){
    DMACTL0 |= DMA1TSEL_20; //DISPARA = UAC1RXIFG
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
