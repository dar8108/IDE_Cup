#include "msp.h"

#include "i2c.h"
#include "Common.h"


/*
 * i2c.c
 *
 *
 *  Update to the new register notation
 *	LJBeato
 *	03/23/21
 *
 *  added   listener  09/04/21
 * LJBeato
 */


#
extern uint32_t SystemCoreClock;
char outputStr[] = "Hello World";


//static BOOLEAN OLED_Display_Active;
// This function writes the RECEIVER address to the i2c bus.
// If a RECEIVER chip is at that address, it should respond to
// this with an "ACK".   This function returns TRUE if an
// ACK was found.  Otherwise it returns FALSE.

//// --------------------- I2C0  ---------------------------------------------

void i2c0_Init(int listenerAddress)
{

	// make sure module is disabled (in reset mode) 
    // EUSCI_B0->CTLW0
	EUSCI_B0->CTLW0 |= BIT0;

	// set appropriate Port.Pins for SDA/SCL
    I2C0_SDA_PORT->SEL0 |= I2C0_SDA_PIN;
    I2C0_SDA_PORT->SEL1 &= ~I2C0_SDA_PIN;
    I2C0_SCL_PORT->SEL0 |= I2C0_SCL_PIN;
    I2C0_SCL_PORT->SEL1 &= ~I2C0_SCL_PIN;
    
    // configure EUSCI_B0->CTLW0 for: 0001 1111 1001 0001
    
    // bit15      UCA10   - 0 = Own address is a 7-bit address,      1 = Own address is a 10-bit address.
    // bit14      UCSLA10 - 0 = Address LISTENER with 7-bit address, 1 = Address LISTENER with 10-bit address
    // bit13      UCMM    - 0 = Single  MASTER environment,          1 = multi-MASTER environ 
    // bit12      reserved
    // ----------------------------------------------------------------

    // bit11      UCMST   -  0 = LISTENER mode, 1 = MASTER mode
    // bits10-9   UCMODEx - 00 = 3-pin SPI
    // 											01 = 4-pin SPI (MASTER or LISTENER enabled if STE = 1)
    // 											10 = 4-pin SPI (MASTER or LISTENER enabled if STE = 0)
    // 											11 = I2C mode
    // bit8       UCSYNC  - For eUSCI_B always read and write as 1.
	// -----------------------------------------------------------------
	// 
    // bits7-6    UCSSELx - (ignore in LISTENER mode)
    //											00 -  UCLKI
    //											01 - 	ACLK
    //											10 -  SMCLK
    //											11 -  SMCLK
    // bit5       UCTXACK  - 0 = do not acknowledge LISTENER, 1 = acknowledge LISTENER
    // bit4       UCTR     - 0 = LISTENER,                    1 = MASTER
	//

    // bit3       UCTXNACK - 0 = acknowledge normally,  1 = generate NACK
    // bit2       UCTXSTP  - 0 = DO NOT generate STOP,  1 = generate STOP
    // bit1       UCTXSTT  - 0 = DO NOT generate START, 1 = generate START
    // bit0       UCSWRST  - 0 = not RESET,             1 = RESET
	//
	//

	// 7 bit LISTENER and self, 
	// single MASTER 
	
	// MASTER mode  
	// I2C mode
	
	// SMCLK mode 
	// don/t acknowledge
	// MASTER 
	
	// ack normal
	// no STOP
	// no START
	// stay RESET
	// 
 	//EUSCI_B0->CTLW0
    EUSCI_B0->CTLW0 = CTLW0_CONFIG_MASK;

	// set clock: 400 KHz
	// EUSCI_B0->BRW 
    EUSCI_B0->BRW = SystemCoreClock / BIT_CLK_PRESCALER;
	
    // initialize RECEIVER address 
	// EUSCI_B0->I2CSA
    EUSCI_B0->I2CSA = listenerAddress;

	// release reset
	// EUSCI_B0->CTLW0
    EUSCI_B0->CTLW0 &= ~BIT0;
}

void i2c0_put(BYTE *data, unsigned int len)
{
    uint32_t i;
    
	// enable i2c module, (remove from RESET)
	// EUSCI_B0->CTLW0
    EUSCI_B0->CTLW0 &= ~BIT0;

    // generate start condition and wait for the bus 
    // EUSCI_B0->CTLW0
    EUSCI_B0->CTLW0 |= CTLW0_GEN_START;
    
	// BIT1 of IFG reg is 0 until character has been transmitted, then changes to 1
	// wait until it changes
    // EUSCI_B0->IFG
    while ( (EUSCI_B0->IFG & UCTXIFG0_B1) != UCTXIFG0_B1) 
    {
        // Wait
    }

	// write data byte by byte to i2c, use putchar
    for (i = 0; i < len; i++)
    {
        i2c0_putchar(data[i]);
    }

    // force stop
	// EUSCI_B0->CTLW0;
    EUSCI_B0->CTLW0 |= CTLW0_GEN_STOP;
    
    // wait for transmission to complete
	// EUSCI_B0->IFG
    while ( (EUSCI_B0->IFG & UCSSTPIFG_B3) != UCSSTPIFG_B3)    
    {
        // Wait
    }

	// transmission completed, disable the module (put it back in reset)
	//EUSCI_B0->CTLW0
    EUSCI_B0->CTLW0 |= BIT0;
}

/* write data on i2c bus */
static void i2c0_putchar(BYTE data)
{
    // write data to TXBUF
	// EUSCI_B0->TXBUF
    EUSCI_B0->TXBUF = data;
    
	// wait until byte is transmitted
	// EUSCI_B0->IFG
    while ( (EUSCI_B0->IFG & UCTXIFG0_B1) != UCTXIFG0_B1)
    {
        // Wait for UCTXIFG0 flag to be set
    }
}

