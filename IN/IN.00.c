#include <msp430.h> 

// A cada acionamento de S1 inverte Led vermelho

void configLED(void);
void configIN(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configLED();
	configIN();
	__enable_interrupt();
	while (1);

	return 0;
}

#pragma vector =  42;
__interrupt void isrS1(void){
    P2IV; // Apaga pedido
    P1OUT ^= BIT0;
}

void configLED(void){
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
}

void configIN(void){
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1; //HABILITA PULL UP

    P2IES |= BIT1;  // SELECIONA O FLANCO == DECIDA
    P2IFG = 0;  // APAGA FLAGS ANTIGAS
    P2IE |= BIT1;   // HABILITANDO A INTERRUPÇÃO
}
