#include <msp430.h> 

/* Controla o brilho do led1
 *  PWM = 100Hz - período de 10ms
 * Usar TA0.1 --> timer A0 comparador 1
 * S1 aumenta em 10% o ciclo de carga
 * S2 diminui em 10% o ciclo de carga
 */

#define T_PWM 10480
#define PASSO 1048
#define  TRUE 1
#define  FALSE 0
#define  ABERTA 1
#define  FECHADA 0
#define  DBC 350

void configLED(void);
void configTM(void);
int chaveS1(void);
int chaveS2(void);
void debounce (int valor);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configLED();
	configTM();
	__enable_interrupt();

	while(1){

	    if(chaveS1() == TRUE){
	        if(TA0CCR1 < TA0CCR0){ TA0CCR1 += PASSO; }}

	    if(chaveS2() == TRUE){
	       if(TA0CCR1 > 0){ TA0CCR1 -= PASSO; }}
	}

	return 0;
}

void configLED(void){
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    //PORTA S1
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |=BIT1;
    //PORTA S2
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

}
void configTM(void){
    TA0CTL = TASSEL_2 | MC_1;
    TA0CCTL1 = OUTMOD_6;

    TA0CCR0 = T_PWM; //COLOCO O TOTAL
    TA0CCR1 = PASSO*5; // COLOCO EM 50%

    P1DIR |= BIT2; //COLOCO COM SAÍDA
    P1SEL |= BIT2; //DEDICADO AO OUT
}
void debounce(int valor){
    volatile int x;
    for(x=0 ; x< 50*valor;x++);
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

