#include "i2c.h"

void USCIB0Init(void){
	 /*Configuracion de I2C*/
	   //IE2 &= ~UCB0RXIE;
	   //while (UCB0CTL1 & UCTXSTP);// Ensure stop condition got sent// Disable RX interrupt
	   UCB0CTL1 |= UCSWRST; //USCI en reset
	   UCB0CTL0 = UCMST+UCMODE_3+UCSYNC; //master mode, el módulo USCI como I2C, y modo sincrono.
	   UCB0CTL1 = UCSSEL_2+UCSWRST; //SMCLK como clock source, el USCI sigue en estado reset.

	   UCB0BR0 = 10;//Prescaler del clock del I2C para hacer fSCL = SMCLK/12 = ~100kHz
	   UCB0BR1 = 0;

	   //UCB0I2COA = 0; //Establecemos la dirección del dispositivo esclavo.
	   UCB0I2CSA = 0;
	   UCB0CTL1 &= ~UCSWRST; ////Ponemos al USCI en estado de operacion.
	   //IE2 |= UCB0TXIE;// Enable TX interrupt
}

void I2CStop(void)
{
   while((UCB0STAT & BUSY)!= 0); //Esperamos a que la trasmision termine

   UCB0CTL1 |= UCTXSTP; //Transmit STOP condition in master mode
}

unsigned char I2CStartWrite(unsigned int slave, unsigned char firstdata)
{ 
   while((UCB0STAT & BUSY)!= 0); //Esperamos a que la trasmision termin

   UCB0I2CSA = slave; //Direccion del dispositivo esclavo

   UCB0CTL1 |= UCTR;                      // I2C TX
   
   UCB0CTL1 |= UCTXSTT;  
    
   //Cargamos el primer dato en el buffer de TX antes
   //de que llegue el ACK.
   UCB0TXBUF = firstdata;     
   //Esperar mientras llega el ACK del START.
   //Luego de que el flag UCB0TXIFG se ponga a 1, se enviará el primer dato a bajo nivel. 
   //Mientras tanto se puede ir haciendo lo siguiente:
   //- Cargar el buffer de TX con un siguiente dato de escritura.
   //- Enviar señal STOP.
   //- Enviar otro START con UCTR=1(TX/escritura).
   //- Enviar otro START con UCTR=0 (RX/lectura).
   //(en este punto tambien el UCTXSTT se pone a 0).
   //while(IFG2_bit.UCB0TXIFG == 0);
   while(IFG2 & UCB0TXIFG == 0);
   //Si quieres saber sobre el ACK.
   if((UCB0STAT & UCNACKIFG)!=0){
      return 0;
   } else {
      return 1;
   }   
}

unsigned char I2CStartRead(unsigned int slave)
{
   while((UCB0STAT & BUSY)!= 0);

   UCB0I2CSA = slave;

   UCB0CTL1 &= ~UCTR; 
   
   UCB0CTL1 |= UCTXSTT;        
   
   while((UCB0CTL1 & UCTXSTT)!= 0);

   //Si quieres saber sobre el ACK.
   if((UCB0STAT & UCNACKIFG)!=0){
      return 0;
   } else {
      return 1;
   }   
}

unsigned char I2CWrite8(unsigned char c)
{
   while((UCB0STAT & BUSY)!= 0);

   UCB0TXBUF = c;
   while(IFG2 & UCB0TXIFG == 0);
  
   if((UCB0STAT & UCNACKIFG)!=0){
      return 0;
   } else {
      return 1;
   }
}

unsigned char I2CRead8(void)
{
   unsigned char dato;

   while((UCB0STAT & BUSY)!= 0);
 
   while(IFG2 & UCB0RXIFG == 0);
   dato = UCB0RXBUF;

   return dato;
}
