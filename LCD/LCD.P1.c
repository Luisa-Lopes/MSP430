#include <msp430.h> 

#define TRUE 1
#define FALSE 0

#define BR10K 105
#define BR100K 11


void i2cWRITE(char dado);
char i2cTEST(char adr);
void uscib0CONFIG(void);
void ledsCONFIG(void);
void atraso(long int x);
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	char adr;
	ledsCONFIG();
	uscib0CONFIG();
	
	if(i2cTEST(0x27) == TRUE) adr = 0x27;
	else if (i2cTEST(0x3F) == TRUE) adr = 0x3F;
	else{
	    P1OUT |= BIT0;
	    while(TRUE);
	}
	P4OUT |= BIT7;
	UCB0I2CSA = adr;
	i2cWRITE(0);

	while(TRUE){
	    i2cWRITE(8);
	    atraso(20000);
	    i2cWRITE(0);
	    atraso(20000);
	}

	return 0;
}

void i2cWRITE(char dado){
    UCB0CTL1 |= UCTR
             |  UCTXSTT;
    while((UCB0IFG&UCTXIFG)==0);
    UCB0TXBUF = dado;
    while((UCB0CTL1&UCTXSTT) == UCTXSTT);
    if((UCB0IFG&UCNACKIFG) == UCNACKIFG){
        P1OUT |= BIT0;
        while(1);
    }
    UCB0CTL1 |= UCTXSTP;
    while((UCB0CTL1&UCTXSTP)==UCTXSTP);
}

char i2cTEST(char adr){
    UCB0I2CSA = adr;
    UCB0CTL1 |= UCTR;
    UCB0CTL1 |= UCTXSTT;
    while((UCB0IFG&UCTXIFG)==0);
    UCB0CTL1 |= UCTXSTP;
    while((UCB0CTL1&UCTXSTP) == UCTXSTP);
    if((UCB0IFG&UCNACKIFG)==0) return TRUE;
    else{ return FALSE;}
}

void uscib0CONFIG(void){
    UCB0CTL1 = UCSWRST;
    UCB0CTL0 = UCMST   |
               UCMODE_3 |
               UCSYNC;

    UCB0BRW = BR100K;
    UCB0CTL1 = UCSSEL_3;
    P3SEL |= BIT1 | BIT0;
    P3REN |= BIT1 | BIT0;
    P3OUT |= BIT1 | BIT0;
}

void ledsCONFIG(void){
    P1DIR |= BIT0;P1OUT &= ~BIT0;
    P4DIR |= BIT7;P4OUT &= ~BIT7;
}

void atraso(long int x){
    volatile long int y;
    for(y =0 ; y<x;y++);
}
