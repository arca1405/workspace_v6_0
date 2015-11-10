/*
 * i2c.h
 *
 *  Created on: 09/11/2015
 *      Author: Carlos
 */

#ifndef I2C_H_
#define I2C_H_

#include "msp430g2553.h"

void USCIB0Init(void);

void I2CStop(void);

unsigned char I2CStartWrite(unsigned int slave, unsigned char firstdata);

unsigned char I2CStartRead(unsigned int slave);

unsigned char I2CWrite8(unsigned char c);

unsigned char I2CRead8(void);



#endif /* I2C_H_ */
