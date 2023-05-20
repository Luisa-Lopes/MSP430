#include <msp430.h> 
#include <string.h>

void configUART(void);
void condigGPIO(void);

void ser_str(char *pt);
void ser_char(char x);
void imprime(char *vet,int tamanho);
void configura(char *vet);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	char vet[20];
	char contador = 0;

	configUART();
	configGPIO();

	__delay_cycles(100);

	ser_str("Pronto!\n\r");
	ser_str("Tecle algo!\n\r");

	while (1){
	    while ( (UCA1IFG & UCRXIFG) == 0 );  //Esperar Tx

	    vet[contador]=UCA1RXBUF;

	    if(vet[contador] == 0x0D){
	        imprime(vet,contador);
	        contador = 0;}

	    contador++;
	}
	return 0;
}

void imprime(char *vet,int tamanho){
    int i;
    char aux[2];

    for(i = 0; i < tamanho; i++){
       if(vet[i] == 'v'){
           if(i != (tamanho - 1)){
               aux[0] = vet[i];
               aux[1] = vet[i + 1];
               configura(aux); }
       }
       if(vet[i] == 'V'){
           aux[0] = vet[i];
           aux[1] = vet[i + 1];
           configura(aux);
       }
     }
 }


void configura(char *vet){

    int valor = 0;
    if(vet[0] == 'V') { valor = 10; }
    else { valor = 20; }

   switch(vet[1]){
   case 'd': valor += 2; break;
   case 'D': valor += 4; break;
   case 'm': valor += 6; break;
   case 'M': valor += 8; break;
   default : valor = 0; break;
   }

   switch(valor){
       case 12:   P4OUT ^= BIT7;  break;  //Vd
       case 14:   P4OUT |=  BIT7; break;  //VD
       case 16:   P1OUT ^= BIT0;  break;  //Vm
       case 18:   P1OUT |=  BIT0; break;  //VM
       case 22:   P4OUT &= ~BIT7; break;  //vd
       case 24:   P4OUT ^= BIT7;  break;  //vD
       case 26:   P1OUT &= ~BIT0; break;  //vm
       case 28:   P1OUT ^= BIT0;  break;  //vM
       default: break;
   }

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

void configUART(void){
    UCA1CTL1 = UCSWRST;  //RST=1
    UCA1CTL0 = 0;
    UCA1BRW= 109;           //BRX=109
    UCA1MCTL = UCBRS_2;     //BRS=2

    P4DIR |=  BIT4;         //P4.4 saída
    P4DIR &= ~BIT5;         //P4.5 entrada
    P4SEL |= BIT5 | BIT4;   //Selecionar UART

    UCA1CTL1 = UCSSEL_2;     //RST=0 e SMCLK
}

void configGPIO(void){
          // LED 1
       P1DIR |= BIT0;
       P1OUT &= ~BIT0;

        // LED 2
       P4DIR |= BIT7;
       P4OUT &= ~BIT7;
}

