#include <msp430.h> 

/* P1.2 --> apaga led vermelho
 * P1.5 --> acende led vermelho
 * P1.4 --> apaga led verde
 * P1.3 --> acende led verde
 * P1.2 --> inverte estado dos dois leds
 */

#define BIT65432 (BIT6|BIT5|BIT4|BIT3|BIT2)


void configLED(void);
void configIN(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	configLED();
	configIN();
	__enable_interrupt();
	
	while(1);

	return 0;
}

void configLED(void){
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}

void configIN(void){
    P1DIR &= ~BIT65432;
    P1REN |= BIT65432;
    P1OUT |= BIT65432; //HABILITA PULL UP

    P1IES |= BIT65432;  // SELECIONA O FLANCO == DECIDA
    P1IFG &= BIT65432;  // APAGA FLAGS ANTIGAS
    P1IE |= BIT65432;   // HABILITANDO A INTERRUPÇÃO
}

#pragma vector = 47;
__interrupt void isrP1(void){
    int x;
    x = __even_in_range(P1IV,16);

    switch(x){
    case 6: P1OUT &= ~BIT0; break;
    case 8: P1OUT |= BIT0; break;
    case 10: P4OUT &= ~BIT7; break;
    case 12: P4OUT |= BIT7; break;
    case 14: P1OUT ^= BIT0; P4OUT ^= BIT7; break;
    default : break;
    }
}
