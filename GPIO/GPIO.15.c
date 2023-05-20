#include <msp430.h> 

#define DBC 350

/**
 * main.c
 *
 */

void config(void);
void debounce(int valor);


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	config();

	while(1){
	    P1OUT &= ~BIT0;
	    P4OUT &= ~BIT7;

	    while((P2IN&BIT1) == 0){
	        debounce(DBC);
	        P1OUT |= BIT0;
	    }

	    while((P1IN&BIT1)==0){
	        debounce(DBC);
	        P4OUT |= BIT7;
	    }
	}
	return 0;
}

void debounce(int valor){
    volatile int x;
    for(x=0 ; x< 50*valor;x++);
}

void config(void){
    //PORTA S1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    //PORTA S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    //  LED1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    //  LED2
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}
