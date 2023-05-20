// LCD_Base
// Rotinas básicas para usar o LCD

// P3.0 ==> SDA
// P3.1 ==> SCL
#include <msp430.h> 

#define TRUE    1
#define FALSE   0

// Definição do endereço do PCF_8574
#define PCF_ADR1 0x3F
#define PCF_ADR2 0x27
#define PCF_ADR  PCF_ADR2

#define BR_100K    11  //SMCLK/100K = 11
#define BR_50K     21  //SMCLK/50K  = 21
#define BR_10K    105  //SMCLK/10K  = 105

void lcd_inic(void);
void lcd_aux(char dado);
int pcf_read(void);
void pcf_write(char dado);
int pcf_teste(char adr);
void led_vd(void);
void led_VD(void);
void led_vm(void);
void led_VM(void);
void i2c_config(void);
void gpio_config(void);
void delay(long limite);

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer]

    gpio_config();
    i2c_config();

    if (pcf_teste(PCF_ADR)==FALSE){
        led_VM();           //Indicar que não houve ACK
        while(TRUE);        //Travar
    }
    else    led_VD();       //Houve ACK, tudo certo

    lcd_inic();     //Inicializar LCD
    pcf_write(8);   //Acender Back Light

    delay(200);

    pcf_write(0x68);
    pcf_write(0x04);
    pcf_write(0x2E);
    pcf_write(0x15);
    pcf_write(0x04);
    pcf_write(0x04);
    pcf_write(0x04);
    pcf_write(0x04);
    pcf_write(0x04);

         delay(200);

    pcf_write(0x09);
    pcf_write(0x0D);
    pcf_write(0x09);
    pcf_write(0x59);
    pcf_write(0x5D);
    pcf_write(0x59);


/*
 * int i;
 * for(i=0;i<16;i++){
        //DESLOCAR DISPLAY
         pcf_write(0x18);
         pcf_write(0x1C);
         pcf_write(0x18);
         pcf_write(0xC8);
         pcf_write(0xCC);
         pcf_write(0xC8);
         delay(20000);
    }
 *
 * Escrever  letra A = 0x41
    pcf_write(0x49);
    pcf_write(0x4D);
    pcf_write(0x49);
    pcf_write(0x19);
    pcf_write(0x1D);
    pcf_write(0x19);


    //Escrever  letra B = 0x42
    pcf_write(0x49);
    pcf_write(0x4D);
    pcf_write(0x49);
    pcf_write(0x29);
    pcf_write(0x2D);
    pcf_write(0x29);

    //Escrever  letra A = 0x41
    pcf_write(0x49);
    pcf_write(0x4D);
    pcf_write(0x49);
    pcf_write(0x19);
    pcf_write(0x1D);
    pcf_write(0x19);
 * //Escrever  ESPAÇO = 0x20
    pcf_write(0x29);
    pcf_write(0x2D);
    pcf_write(0x29);
    pcf_write(0x09);
    pcf_write(0x0D);
    pcf_write(0x09);
 *
     //MUDEI O CURSOR DE LUGAR
     pcf_write(0xC8);
     pcf_write(0xCC);
     pcf_write(0xC8);
     pcf_write(0xD8);
     pcf_write(0xDC);
     pcf_write(0xD8);

     //APAGA O LCD
     pcf_write(0x08);
     pcf_write(0x0C);
     pcf_write(0x08);
     pcf_write(0x18);
     pcf_write(0x1C);
     pcf_write(0x18);

     //RETORNA CURSO PARA 1 LINHA 1 COLUNA
     pcf_write(0x08);
     pcf_write(0x0C);
     pcf_write(0x08);
     pcf_write(0x28);
     pcf_write(0x2C);
     pcf_write(0x28);

     //MODO ENTRADA
      pcf_write(0x08);
      pcf_write(0x0C);
      pcf_write(0x08);
      pcf_write(0x78);
      pcf_write(0x7C);
      pcf_write(0x78);

     //DESLOCAR DISPLAY
     pcf_write(0x18);
     pcf_write(0x1C);
     pcf_write(0x18);
     pcf_write(0xC8);
     pcf_write(0xCC);
     pcf_write(0xC8);
*/
    while(TRUE);      //Travar execução


    return 0;
}

// Incializar LCD modo 4 bits
void lcd_inic(void){

    // Preparar I2C para operar
    UCB0I2CSA = PCF_ADR;    //Endereço Escravo
    UCB0CTL1 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB0TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    lcd_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(3);     //3
    delay(10000);
    lcd_aux(2);     //2

    // Entrou em modo 4 bits
    lcd_aux(2);     lcd_aux(8);     //0x28
    lcd_aux(0);     lcd_aux(8);     //0x08
    lcd_aux(0);     lcd_aux(1);     //0x01
    lcd_aux(0);     lcd_aux(6);     //0x06
    lcd_aux(0);     lcd_aux(0xF);   //0x0F

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}

// Auxiliar inicialização do LCD (RS=RW=0)
// *** Só serve para a inicialização ***
void lcd_aux(char dado){
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3;            //PCF7:4 = dado;
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3 | BIT2;     //E=1
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT3;            //E=0;
}

// Ler a porta do PCF
int pcf_read(void){
    int dado;
    UCB0I2CSA = PCF_ADR;                //Endereço Escravo
    UCB0CTL1 &= ~UCTR;                  //Mestre RX
    UCB0CTL1 |= UCTXSTT;                //Gerar START
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);
    UCB0CTL1 |= UCTXSTP;                //Gerar STOP + NACK
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    while ( (UCB0IFG & UCRXIFG) == 0);  //Esperar RX
    dado = UCB0RXBUF;
    return dado;
}

// Escrever dado na porta
void pcf_write(char dado){
    UCB0I2CSA = PCF_ADR;        //Endereço Escravo
    UCB0CTL1 |= UCTR    |       //Mestre TX
                UCTXSTT;        //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0TXBUF = dado;                              //Escrever dado
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT)   ;   //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)       //NACK?
    while(1);                          //Escravo gerou NACK
    UCB0CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
}

// Testar endereço I2C
// TRUE se recebeu ACK
int pcf_teste(char adr){
    UCB0I2CSA = adr;                            //Endereço do PCF
    UCB0CTL1 |= UCTR | UCTXSTT;                 //Gerar START, Mestre transmissor
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar pelo START
    UCB0CTL1 |= UCTXSTP;                        //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //Esperar pelo STOP
    if ((UCB0IFG & UCNACKIFG) == 0)     return TRUE;
    else                                return FALSE;
}

// Configurar UCSB0 e Pinos I2C
// P3.0 = SDA e P3.1=SCL
void i2c_config(void){
    UCB0CTL1 |= UCSWRST;    // UCSI B0 em ressete
    UCB0CTL0 = UCSYNC |     //Síncrono
               UCMODE_3 |   //Modo I2C
               UCMST;       //Mestre
    UCB0BRW = BR_100K;      //100 kbps
    P3SEL |=  BIT1 | BIT0;  // Use dedicated module
    UCB0CTL1 = UCSSEL_2;    //SMCLK e remove ressete
}

void led_vd(void)   {P4OUT &= ~BIT7;}   //Apagar verde
void led_VD(void)   {P4OUT |=  BIT7;}   //Acender verde
void led_vm(void)   {P1OUT &= ~BIT0;}   //Apagar vermelho
void led_VM(void)   {P1OUT |=  BIT0;}   //Acender vermelho

// Configurar leds
void gpio_config(void){
    P1DIR |=  BIT0;      //Led vermelho
    P1OUT &= ~BIT0;      //Vermelho Apagado
    P4DIR |=  BIT7;      //Led verde
    P4OUT &= ~BIT7;      //Verde Apagado
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}
