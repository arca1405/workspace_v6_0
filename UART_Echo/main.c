#include "msp430g2553.h"
//#include "math.h"

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

const char string[] = { "  Hola mundo\r\n" };

unsigned int i; //Counter

unsigned char txData[6] = {0,0,0,0,'\r','\n'};

void binaryToASCII(unsigned int n, unsigned char * digits);

int main(void)
{
   WDTCTL = WDTPW + WDTHOLD; // Stop WDT

   if (CALBC1_1MHZ==0xFF){					// If calibration constant erased
     while(1);                               // do not load, trap CPU!!
   }

   DCOCTL = 0; // Select lowest DCOx and MODx settings
   BCSCTL1 = CALBC1_1MHZ; // Set DCO
   DCOCTL = CALDCO_1MHZ;

   /*Configuracion de puertos*/
   P2DIR |= 0xFF; // All P2.x outputs
   P2OUT &= 0x00; // All P2.x reset
   P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   P1DIR |= RXLED + TXLED;
   P1OUT &= 0x00;


   UCA0CTL1 |= UCSSEL_2; // SMCLK
   UCA0BR0 = 104; // 1MHz 9600
   UCA0BR1 = 0x00; // 1MHz 9600
   UCA0MCTL = UCBRS2 + UCBRS0; // Modulation UCBRSx = 5
   UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
   UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
   __bis_SR_register(CPUOFF + GIE); // Enter LPM0 w/ int until Byte RXed

   while (1)
   {

   }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	int numero = 254;
	P1OUT |= TXLED;
   UCA0TXBUF = UCA0RXBUF;
     //UCA0TXBUF = string[i++]; // TX next character
   binaryToASCII(numero, txData);          // convert to a string
   //*txData = numero;
   UCA0TXBUF = txData[i++];
    if (i == sizeof txData - 0) // TX over?
       UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    P1OUT &= ~TXLED;
       }

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   P1OUT |= RXLED;
    if (UCA0RXBUF == 'a') // 'a' received?
    {
       i = 0;
      UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
      //UCA0TXBUF = string[i++];
      UCA0TXBUF = txData[i++];
    }
    UCA0TXBUF = UCA0RXBUF;
    P1OUT &= ~RXLED;
}

void binaryToASCII(unsigned int n, unsigned char * digits) {
    __asm(" clr		R14");
    __asm(" rla		R12");
    __asm(" rla		R12");
    __asm(" rla		R12");
    __asm(" rla		R12");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" rla		R12");
    __asm(" dadd	R14, R14");
    __asm(" mov.b	R14, 3(R13)");
    __asm(" swpb	R14");
    __asm(" mov.b	R14, 1(R13)");
    __asm(" rra		R14");
    __asm(" rra		R14");
    __asm(" rra		R14");
    __asm(" rra		R14");
    __asm(" mov.b	R14, 0(R13)");
    __asm(" swpb	R14");
    __asm(" mov.b	R14, 2(R13)");
    __asm(" and		#0x0F0F, 0(R13)");
    __asm(" and		#0x0F0F, 2(R13)");
    __asm(" add.b	#0x30, 3(R13)");
    __asm(" tst.b	0(R13)");
    __asm(" jnz l1");
    __asm(" mov.b	#0x20, 0(R13)");
    __asm(" tst.b	1(R13)");
    __asm(" jnz l2");
    __asm(" mov.b	#0x20, 1(R13)");
    __asm(" tst.b	2(R13)");
    __asm(" jnz l3");
    __asm(" mov.b	#0x20, 2(R13)");
    __asm(" jmp l4");
    __asm("l1:");
    __asm(" add.b	#0x30, 0(R13)");
    __asm("l2:");
    __asm(" add.b	#0x30, 1(R13)");
    __asm("l3:");
    __asm(" add.b	#0x30, 2(R13)");
    __asm("l4:");
    return;
}


