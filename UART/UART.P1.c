#include <msp430.h> 


void configUART(void);
void configGPIO(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	char letra = 'A' , r[26];
	int i = 0;

	configGPIO();
	configUART();

	while(1){
	    while((UCA0IFG&UCTXIFG) == 0);  //ESPERO PERMITIR PASSAR UMA LETRA
	    UCA0TXBUF = letra; // COLOCO A LETRA

	    while((UCA0IFG&UCRXIFG) == 0); // ESPERO DIZER QUE RECEBEU
	    r[i] = UCA0RXBUF; // COLOCO A LETRA RECEBIDA NO VETOR

	    if(r[i] == letra){ P1OUT &= ~BIT0; P4OUT |= BIT7;}
	    else{ P1OUT |= BIT0; P4OUT &= ~BIT7; }

	    i++;
	    letra++;

	    if(letra == 'Z' + 1){ i = 0; letra = 'a';}
	    if(letra == 'z' + 1){ i = 0; letra = 'A';}
	}

	return 0;
}

void configUART(void){
    UCA0CTL1 = UCSWRST;   // ATIVAMOS O RESET
    UCA0CTL0 = 0;        // SEM PARIDADE, 8 BITS, 1 STOP, MODO UART
    UCA0STAT = UCLISTEN; // LOOP BACK
    UCA0BRW = 3;         // DIVISOR
    UCA0MCTL = UCBRS_3;  // MODULADOR = 3 E UCOS = 0
    UCA0CTL1 = UCSSEL_1; // DESATIVAMOS O RESET E SELECIONAMOS O ACLK
}
void configGPIO(void){

    // LED 1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // LED 2
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}
