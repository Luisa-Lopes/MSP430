#include <msp430.h> 

#define DBC 50000
/**
 * main.c
 *
 */

void config(void);
//void debounce(int valor);
void conta(int contador);

int main(void){

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	config();

	int contador = 0;

	while(1){

	   while ( (P2IN&BIT1) == 0){
	       debounce(DBC);
	       contador++;
	       conta(contador); }

	  while ((P2IN&BIT1) == BIT1){
	       debounce(DBC);
	      }

	}
	
	return 0;
}

void config(void){
    // PORTA S1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    // LED 1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // LED 2
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}

void debounce(int valor){
    volatile int x;
    for(x=0; x< valor; x++);

}

void conta(int contador){
    switch(contador&3){
    case (0):
        P1OUT &= ~BIT0;
        P4OUT &= ~BIT7;
        break;
    case (1):
        P1OUT &= ~BIT0;
        P4OUT |= BIT7;
        break;
    case (2):
        P1OUT |= BIT0;
        P4OUT &= ~BIT7;
        break;
    case (3):
        P1OUT |= BIT0;
        P4OUT |= BIT7;
        break;
            }
}
