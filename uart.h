/*
 * File:        uart.h
 * Purpose:     
 *
 * Notes:		
 *
 */

#include "Common.h"

#define LS_BYTE_MASK      0x00FF
#define NUM_DIGITS_32BITS 10

#define UART0_RX_PORT     P1
#define UART0_RX_PIN      BIT2
#define UART0_TX_PORT     P1
#define UART0_TX_PIN      BIT3

#define UCSWRST_MASK      BIT0
#define SMCLK_SEL_MASK    UCSSEL__SMCLK
#define MCTLW_OS16_MASK   EUSCI_A_MCTLW_OS16
#define UCTXCPTIE_MASK    BIT3
#define UCSTTIE_MASK      BIT2
#define UCTXIE_MASK       BIT1
#define UCRXIE_MASK       BIT0
#define UCRXIFG_MASK      BIT0
#define UCTXIFG_MASK      BIT1

/* Public function prototypes */
extern void uart0_init(void);
extern BYTE uart0_getchar(void);
extern void uart0_putchar(char ch);
extern void uart0_put(char *ptr_str);
extern void putnumU(int num);
extern void putnumHex(int num);
