#include <msp430.h> 

void configUART(void);
void configGPIO(void);

static letraTX,letraRX;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configUART();
	configGPIO();

	__enable_interrupt();

	UCA0TXBUF = 'A';

	while(1){
	    if(letraTX == letraRX){ P1OUT &= ~BIT0; P4OUT |= BIT7;}
	    else{ P1OUT |= BIT0; P4OUT &= ~BIT7; }
	}

	return 0;
}

#pragma vector = 56
__interrupt void UART_A0(void){
     volatile static letra='A';
     UCA0IV; //Apagar RXIFG
     letraRX = UCA0RXBUF; //Letra recebida
     letraTX = letra; //Letra transmitida
     if (++letra == 'Z' + 1) //Próxima letra
     letra = 'A'; //Voltar para letra A
     UCA0TXBUF = letra; //Transmitir
}

void configUART(void){
    UCA0CTL1 = UCSWRST;   // ATIVAMOS O RESET
    UCA0CTL0 = 0;        // SEM PARIDADE, 8 BITS, 1 STOP, MODO UART
    UCA0STAT = UCLISTEN; // LOOP BACK
    UCA0BRW = 3;         // DIVISOR
    UCA0MCTL = UCBRS_3;  // MODULADOR = 3 E UCOS = 0
    UCA0CTL1 = UCSSEL_1; // DESATIVAMOS O RESET E SELECIONAMOS O ACLK
    UCA0IE = UCRXIE;
}
void configGPIO(void){

    // LED 1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    // LED 2
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}
