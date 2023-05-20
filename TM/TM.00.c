#include <msp430.h> 

/* Usar interrupções do timer
 * Piscar led 1 em 10Hz
 * Piscar led 2 em 3Hz
 */

#define T_VM 1638
#define T_VD 5461

void configLED(void);
void configTM(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configLED();
	configTM();
	__enable_interrupt();

	while(1);

	return 0;
}

#pragma vector = 52
__interrupt void isrTM(void){
    int x;
    x = __even_in_range(TA0IV,14);

    switch(x){
    case 2:
        TA0CCR1 += T_VM;
        P1OUT ^= BIT0;
        break;
    case 4:
        TA0CCR2 += T_VD;
        P4OUT ^= BIT7;
        break;
    }
}

void configLED(void){
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}

void configTM(void){
    TA0CTL = TASSEL_1 | MC_2;
    // LED VERMELHO
    TA0CCR1 = T_VM;
    TA0CCTL1 = CCIE; //HABILITA A INTERRUPÇÃO
    //LED VERDE
    TA0CCR2 = T_VD;
    TA0CCTL2 = CCIE; //HABILITA A INTERRUPÇÃO
}


