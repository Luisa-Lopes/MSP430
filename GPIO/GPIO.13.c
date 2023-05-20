#include <msp430.h> 

#define CTE 350
/**
 * main.c
 */

void config(void);
void inverte(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	config();

	while(1){
	    while((P2IN&BIT1) == 0){
	        debounce();
	        inverte();
	    }
	    while((P2IN&BIT1) == BIT1){
	        debounce();
	    }
	}

	return 0;
}

void config(void){
    //PORTA S1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    // LED1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
}

void debounce(void){
    volatile int x;
        for (x = 0; x < 50*CTE; x++);
}

void inverte(void){
    P1OUT ^= BIT0;
}
