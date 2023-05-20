#include <msp430.h> 

#define TP50MS   17660 //Atraso de 50 ms
#define TP400MS  141280 //ATRASO DE 400 MS
#define TP60MS   21192  //ATRASO DE 60 MS
#define TP600MS   211920 //ATRASO DE 600 MS

void config(void);
void tempoLED1(int valor);
void tempoLED2(int valor);

int main(void)
{
    config();

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	int contador = 0;

	while(1){

	    tempoLED1(contador);
	    P1OUT ^= BIT0;

	    tempoLED2(contador);
	    P4OUT ^= BIT7;

	    contador++;
	}


	return 0;
}


void config(void){
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}

void tempoLED1(int valor){
    volatile int x;

    if(valor < 7){
        for( x=0; x<TP50MS;x++);
    }
    else{
        for( x=0; x<TP400MS;x++);
    }

}

void tempoLED2(int valor){
    volatile int x;

    if(valor < 4){
        for( x=0; x<TP60MS;x++);
    }
    else{
        for( x=0; x<TP600MS;x++);
    }

}
