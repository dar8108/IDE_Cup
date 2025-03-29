
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "uart.h"
#include "switches.h"
#include "ADC14.h"
#include "Timer32.h"
#include "TimerA.h"
#include "SysTickTimer.h"
#include "ControlPins.h"
#include "CortexM.h"

uint16_t line[128];
BOOLEAN g_sendData;

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void delay(int del)
{
    volatile int i;
	for (i=0; i<del*50000; i++)
    {
		;// Do nothing
	}
}

int main(void)
{
    /* Perform initializations */
    uart0_init();
    
    

    /* Car operations */
    while(1)
    {
          
    }
}
