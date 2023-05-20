#include <msp430.h> 

/* Use a chave S2 para comandar o pino P2.0
 * Quando acontecer o acionamento por meio de interrupção por flanco de decida
 * Com a interrupção o led vermelho vai acender
 * */

#define DBC 500

void configLED(void);
void configIN(void);
void debounce(int valor);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configLED();
	configIN();
	__enable_interrupt();

	while(1){
	  P1OUT &= ~BIT0;

	  while((P1IN&BIT1) == 0){
	      debounce(DBC);
	  }

	}

	return 0;
}

#pragma vector = 42
__interrupt void isrS2(void){
    P1OUT ^= BIT0;
    P2IFG &= ~BIT0;
}

void debounce(int valor){
    volatile int x;
        for (x = 0; x < 10*valor; x++);
}

void configLED(void){
    // LED 1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // CHAVE S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1; //HABILITA PULL UP
}

void configIN(void){
    P2DIR &= ~BIT0;
    P2REN |= BIT0;
    P2OUT |= BIT0; //HABILITA PULL UP

    P2IFG &= ~BIT0;  // APAGA FLAGS ANTIGAS
    P2IES |= BIT0;  // SELECIONA O FLANCO == DECIDA
    P2IE |= BIT0;   // HABILITANDO A INTERRUPÇÃO
}
