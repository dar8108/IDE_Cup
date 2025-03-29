/*
 * i2c.h
 *
 *  Created on: 9/4/21
 *      Author: LJBeato
 */

#ifndef I2C_H_
#define I2C_H_
#include "Common.h"

#define I2C0_SDA_PORT  P1
#define I2C0_SDA_PIN   BIT6
#define I2C0_SCL_PORT  P1
#define I2C0_SCL_PIN   BIT7

#define CTLW0_CONFIG_MASK   0x1F91
#define BIT_CLK_PRESCALER   400000
#define CTLW0_GEN_STOP      BIT2
#define CTLW0_GEN_START     BIT1
#define UCSSTPIFG_B3        BIT3
#define UCTXIFG0_B1         BIT1


void i2c0_Init(int listenerAddress);
void i2c0_put(BYTE *data, unsigned int len);

void i2c0_putchar(BYTE ch);

#endif /* I2C_H_ */
