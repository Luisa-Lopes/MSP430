#include <msp430.h> 


#define TP10MS  3532 //Atraso de 10 ms
#define TP5MS  1766 //Atraso de 5 ms
#define TP2P5MS  883 //Atraso de 2,5 ms

#define TRUE 1
#define FALSE 0
#define ABERTA 1
#define FECHADA 0
#define DBC 350

void config(void);
void debounce(int valor);
int chaveS1(void);
int chaveS2(void);

void tempo(int valor);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	config();
	int contador = 0;
	int comparador = 0;
	int est1;
	int est2;

	while(1){

	    if(contador == 0){
	        P1OUT |= BIT0;
	    }
	    if(contador == comparador){
	        P1OUT &= ~BIT0;
	    }

	    tempo(contador);

	    if(chaveS1() == TRUE){
	        est1 = 1;
	        contador++;}
	    else{est1 = 0;}

	    if(chaveS2() == TRUE){
	        est2 = 1;
	        contador--;}
	    else{est2 = 0;}

	    if(est1 == 1){
	        est1 = 0;
	        if(comparador != 4){
	            comparador ++;
	        }
	    }

	    if(est2 == 1){
	        est2 = 0;
	        if(comparador != 0){
	            comparador --;
	        }
	    }

	    contador ++;

	    if(contador == 4){
	        contador = 0;}
	}

	return 0;
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


void tempo(int valor){
    volatile int x;

    switch(valor){
    case 0:
        break;
    case 1:
        for(x = 0; x<TP10MS;x++);
        break;
    case 2:
        for(x = 0; x < TP5MS;x++);
        break;
    case 3:
        for( x= 0; x<TP2P5MS;x++);
        break;
    }

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
}

void debounce(int valor){
    volatile int x;
    for(x=0 ; x< 50*valor;x++);
}

