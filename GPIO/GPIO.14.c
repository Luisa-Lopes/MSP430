#include <msp430.h> 

#define  TRUE 1
#define  FALSE 0
#define  ABERTA 1
#define  FECHADA 0
#define  DBC 350
/**
 * main.c
 */

void config(void);
int chaveS1(void);
int chaveS2(void);
void leds(int contador);
void debounce (int valor);


int main(void)
{   int contador = 0;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	config();

	while(1){

	    if(chaveS1() == TRUE){
	        contador++;}

	    if(chaveS2() == TRUE){
	        contador--;}

	    leds(contador);
	}
	return 0;
}

void config(void){
    //PORTA S1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    //PORTA S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;


    //  LED1
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    //  LED2
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;
}

int chaveS1(void){
    static int ps1 = ABERTA;

    if((P2IN&BIT1) == 0){
        if(ps1 == ABERTA){
           debounce(DBC);
            ps1 = FECHADA;
            return TRUE;
        }
    }
    else{
        if(ps1 == FECHADA){
            debounce(DBC);
            ps1 = ABERTA;
            return FALSE;
        }
     }
    return FALSE;
}

int chaveS2(void){
    static int ps2 = ABERTA;

    if((P1IN&BIT1) == 0){
        if(ps2 == ABERTA){
            debounce(DBC);
            ps2 = FECHADA;
            return TRUE;
        }
    }
    else{
        if(ps2 == FECHADA){
            debounce(DBC);
            ps2 = ABERTA;
            return FALSE;
        }
     }
    return FALSE;
}

void leds(int contador){
    switch(contador&3){
        case (0):
            P1OUT &= ~BIT0;
            P4OUT &= ~BIT7;
            break;
        case (1):
            P1OUT &= ~BIT0;
            P4OUT |= BIT7;
            break;
        case (2):
            P1OUT |= BIT0;
            P4OUT &= ~BIT7;
            break;
        case (3):
            P1OUT |= BIT0;
            P4OUT |= BIT7;
            break;
      }
}

void debounce(int valor){
    volatile int x;
    for(x=0 ; x< 50*valor;x++);
}
