#include <msp430.h> 

#define TP50MS  17660
#define TRUE    1
#define FALSE   0
#define ABERTA  1
#define FECHADA 0
#define DBC     350

static int est1;
static int est2;

void config(void);
int chaveS1(void);
int chaveS2(void);
void debounce(int valor);

int main(void){
    config();

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	while(1){

	   if(chaveS1() == TRUE){ est1 = 1;}
	      else{est1 = 0;}

	   if(chaveS2() == TRUE){ est2 = 1;}
	      else{est2 = 0;}

	   debounce(DBC);
	   ligaLED();

	}
	return 0;
}

void ligaLED(void){

    int soma = est1 + est2;

    if ( (soma == 2) || (soma == 0)){
        P1OUT &= ~BIT0;
        P4OUT &= ~BIT7;
    }
    if(est1 == 1){
       P1OUT |= BIT0;}

    if(est2 == 1){
       P4OUT |= BIT7;}

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

void config(void){
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

void debounce(int valor){
    volatile int x;
    for(x=0 ; x<50*valor;x++);
}
