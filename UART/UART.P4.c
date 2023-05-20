// UART-Ex3

// USCI_A1 <== PC
// SMCLK ==> 9.600 (UCOS=0)

#include <msp430.h> 

void ser_dec8(char x);
void ser_str(char *pt);
void ser_char(char x);
void uart_config(void);


int main(void){
    char letra='A';
    char cont=0;
    char vet[20];
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    uart_config();
    __delay_cycles(100);

    ser_str("Tecle algo!\n");

    while (1){
        while ( (UCA1IFG & UCRXIFG) == 0 );  //Esperar Tx
        vet[cont]=UCA1RXBUF;
        cont++;
    }

    return 0;
}

// Imprimir contador 8 bits ( 0, 1, ..., 255)
void ser_dec8(char x){
    char aux;

    // centena
    aux=x/100;
    ser_char(0x30 + aux);

    // dezena
    x=x-aux*100;
    aux=x/10;
    ser_char(0x30 + aux);

    // unidade
    x=x-aux*10;
    ser_char(0x30 + x);
}

// Imprimir uma string na serial
void ser_str(char *pt){
    int i=0;
    while (pt[i] != 0){
        ser_char(pt[i]);
        i++;
    }
}

// Imprime x na porta serial
void ser_char(char x){
    while ( (UCA1IFG & UCTXIFG) == 0 );  //Esperar Tx
    UCA1TXBUF=x;
}

void uart_config(void){
    UCA1CTL1 = UCSWRST;  //RST=1
    UCA1CTL0 = 0;
    UCA1BRW= 109;           //BRX=109
    UCA1MCTL = UCBRS_2;     //BRS=2

    P4DIR |=  BIT4;         //P4.4 saída
    P4DIR &= ~BIT5;         //P4.5 entrada
    P4SEL |= BIT5 | BIT4;   //Selecionar UART

    UCA1CTL1 = UCSSEL_2;     //RST=0 e SMCLK

}
