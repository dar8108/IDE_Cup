/*
 * File:        uart.c
 * Purpose:     Provide UART routines for serial IO
 *
 * Notes:		
 *
 */

/*
Prelab:
  - CTLW0
	- BRW
	- MCTLW
	- SEL0 = 1, SEL1 = 0
	- IE
*/

#include "msp.h"
#include "uart.h"  // you need to create this file with the function prototypes
#include "Common.h"  // from Lab1 code

#define BAUD_RATE 9600      //default baud rate 

extern uint32_t SystemCoreClock;  // clock rate of MCU


void uart0_init()
{
	//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1')
	EUSCI_A0->CTLW0 |= UCSWRST_MASK;

	// bit15=0,      no parity bits
	// bit14=x,      not used when parity is disabled
	// bit13=0,      LSB first
	// bit12=0,      8-bit data length
	// bit11=0,      1 stop bit
	// bits10-8=000, asynchronous UART mode
	// bits7-6=11,   clock source to SMCLK
	// bit5=0,       reject erroneous characters and do not set flag
	// bit4=0,       do not set flag for break characters
	// bit3=0,       not dormant
	// bit2=0,       transmit data, not address (not used here)
	// bit1=0,       do not transmit break (not used here)
	// bit0=1,       hold logic in reset state while configuring

	// set CTLW0 - hold logic and configure clock source to SMCLK
	EUSCI_A0->CTLW0 |= SMCLK_SEL_MASK;

	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	EUSCI_A0->BRW = SystemCoreClock / BAUD_RATE;

	 // clear first and second modulation stage bit fields
	// MCTLW register;  
	EUSCI_A0->MCTLW &= ~MCTLW_OS16_MASK;
	

	// P1.3 = TxD
	// P1.2 = RxD
	// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
    UART0_RX_PORT->SEL0 |= UART0_RX_PIN;
    UART0_TX_PORT->SEL0 |= UART0_TX_PIN;
	UART0_RX_PORT->SEL1 &= ~UART0_RX_PIN;
    UART0_TX_PORT->SEL1 &= ~UART0_TX_PIN;


	// CTLW0 register - release from reset state
	EUSCI_A0->CTLW0 &= ~UCSWRST_MASK;

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A0->IE &= (~UCTXCPTIE_MASK | ~UCSTTIE_MASK | ~UCTXIE_MASK | ~UCRXIE_MASK);

}

BYTE uart0_getchar()
{
    BYTE inChar;
	
	 // Wait for data
	 // IFG register
	  while ((EUSCI_A0->IFG & UCRXIFG_MASK) != UCRXIFG_MASK)
    {
        // Wait until RXBUF has received a complete char
    }

	  // read character and store in inChar variable
	  // RXBUF register
	  inChar = (EUSCI_A0->RXBUF & LS_BYTE_MASK);

	  //Return the 8-bit data from the receiver 
	  return(inChar);

}

void uart0_putchar(char ch)
{
	// Wait until transmission of previous bit is complete 
	// IFG register
	while ((EUSCI_A0->IFG & UCTXIFG_MASK) != UCTXIFG_MASK)
    {
        // Wait until TXBUF is empty
    };
	
	  // send ch character to uart
	  // TXBUF register 
	  EUSCI_A0->TXBUF = ch;
}

void uart0_put(char *ptr_str)
{
	while(*ptr_str != 0)
    {
        uart0_putchar(*ptr_str++);
    }
}


void putnumU(int num)
{
    char ascii;
    char digitArr[NUM_DIGITS_32BITS];
    uint8_t digit;
    int i = NUM_DIGITS_32BITS;
    
    if (num == 0)
    {
        uart0_putchar('0');
        return;
    }
    
    // Loop to get each individual digit
    while (num != 0)
    {
        i--;
        digit = num % 10;     // Get last digit
        ascii = '0' + digit;  // Calculate its ASCII value
        digitArr[i] = ascii;  // Add to end of array
        num /= 10;            // Shift num to the right by a digit
    }
    
    // Print the characters in correct order
    for (i = i; i < NUM_DIGITS_32BITS; i++)   
    {
        uart0_putchar(digitArr[i]);
    }
}

void putnumHex(int num)
{
    int i;
    int hexNum = 0;
    char hexArr[8];
    
    for (i = 0; i < 8; i++)
    {
        hexNum = (num & 0x0000000F);
        num = num >> 4;
        if (hexNum > 9)
        {
            // Add ASCII value of 7 to get correct hex letter
            hexArr[i] = '7' + hexNum;
        }
        else
        {
            // Add ASCII value of 0 to get correct hex number
            hexArr[i] = '0' + hexNum;
        }
    }
    
    // Print out hex digits in correct order (MSB-->LSB)
    uart0_put("0x");
    for (i = 7; i >= 0; i--)
    {
        uart0_putchar(hexArr[i]);
    }
}

