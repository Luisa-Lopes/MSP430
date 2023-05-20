#include <msp430.h> 

#define CTE 350 // 1 milisegundo

//AUMENTO O VALOR DIMINUO A FREQUÊNCIA
//DIMINUO O VALOR AUMENTO A FREQUÊNCIA

/*
  f = 1HZ --> 1 SEGUNDO
  f = 2HZ -->  0.5 SEGUNDOS

  0.5 segundos  = 500 milisegundos

 */

void config(void);
void pisca(void);
void atraso(void);


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	config();

	while(1){
	   	atraso();
	    pisca();
	}
	return 0;
}

void config(void){
    // LED 1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    //LED 2
    P4DIR |= BIT7;
    P4OUT |= BIT7;
}

void pisca(void){
    P1OUT ^= BIT0;
    P4OUT ^= BIT7;
}

void atraso(void){
    volatile int x;
    for (x = 0; x < 50*CTE; x++);

}
