#include <msp430.h> 

/* Uso o dam para fazer cópia de um vetor de inteiros
 * Teste 1: copia usando uma laço de programa
 * Teste 2: copia usando DMA Modos simples
 * Teste 3: copia usadno DMA Modo rejada
 * Teste 4: copia usando DMA Modo bloco
 */

#define TRUE 1
#define FALSE 0

#define QTD 1000

void configDMA(void); //Configura em Modo Simples
char compVET(void); //Compara vetores
void initVET(void);
void initLED(void);

int v0[QTD],v1[QTD];
int tp[4]; //Registra o tempo consumido

int main(void){

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	int i;
	initLED();

	//Teste 1 - Cópia com um laço de programa
	initVET();

	TA0CTL = TASSEL_2 | MC_2 | TACLR;

	for(i=0;i<QTD;i++)
	v1[i]=v0[i];

	TA0CTL = 0;
	tp[0] = TA0R;
	compVET();

	//Teste 2 - Cópia com DMA em Modo Simples
	initVET();
	configDMA();

	TA0CTL = TASSEL_2 | MC_2 | TACLR;

	for(i=0;i<QTD;i++)
	    DMA0CTL |= DMAREQ; //DISPARA dma

	TA0CTL = 0;
	tp[1] = TA0R;
	if((DMA0CTL&DMAIFG) == 0) while(1);
	compVET();

	//Teste 3 - Cópia com DMA em Modo Rajada
	initVET();
	configDMA();

	DMA0CTL |= DMADT_2;  // MODO RAJADA
	TA0CTL = TASSEL_2 | MC_2 | TACLR;
	DMA0CTL |= DMAREQ; //DISPARA DMA

	while((DMA0CTL&DMAIFG) == 0);
	TA0CTL = 0;
	tp[2] = TA0R;
	compVET();

	//Teste 4 - Cópia com DMA em Modo BLOCO
	initVET();
	configDMA();

	DMA0CTL |= DMADT_1;  // MODO BLOCO
	TA0CTL = TASSEL_2 | MC_2 | TACLR;
	DMA0CTL |= DMAREQ; //DISPARA DMA

	while((DMA0CTL&DMAIFG) == 0);
	TA0CTL = 0;
	tp[3] = TA0R;
	compVET();

	P4OUT |= BIT7;
	while(1){tp[3];};

	return 0;
}

void configDMA(void){
    DMA0CTL = DMADT_0 | DMADSTINCR_3 |DMASRCINCR_3; //SIMPLES, INCREMENTA DESTINO, INCREMENTA ENDEREÇO
    DMACTL0 = DMA0TSEL_0; //DREQ
    DMA0SA = v0; // ENDEREÇO FONTE
    DMA0DA = v1; // ENDEREÇO DESTINO
    DMA0SZ = QTD; // QUANTIDADE
    DMA0CTL |= DMAEN; // HABILITO
}

void initVET(void){
    int i;
    for (i=0; i<QTD; i++){
        v0[i] = i;
        v1[i] = 0;
    }
}

char compVET(void){
    int i;
    for(i=0; i<QTD; i++){
        if(v0[i] != v1[i]){P1OUT |= BIT0; return FALSE;} }
    return TRUE;
}
void initLED(void){
    P1DIR |= BIT0;P1OUT &= ~BIT0;
    P4DIR |= BIT7;P4OUT &= ~BIT7;
}
