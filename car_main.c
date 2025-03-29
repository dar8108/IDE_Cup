
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
BOOLEAN startEngine = FALSE;

/**
 * Waits for a delay (in milliseconds)
 * 
 * del - The delay in milliseconds
 */
void dly(int del)
{
    volatile int i;
	for (i = 0; i < del*50000; i++)
    {
		;// Do nothing
	}
}

/**
 * Initialize camera
 */
void INIT_Camera(void)
{
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

void Switch1_Interrupt_Init(void)
{
   // disable interrupts
   DisableInterrupts();
    
   // initialize the Switch as per previous lab
   Switch1_Init();
		
   //7-0 PxIFG RW 0h Port X interrupt flag
   //0b = No interrupt is pending.
   //1b = Interrupt is pending.
   // clear flag1 (reduce possibility of extra interrupt)	
   SWITCH_1_PORT->IFG &= ~SWITCH_1_PIN;

   //7-0 PxIE RW 0h Port X interrupt enable
   //0b = Corresponding port interrupt disabled
   //1b = Corresponding port interrupt enabled	
   // arm interrupt on  P1.1	
   SWITCH_1_PORT->IE |= SWITCH_1_PIN;

   //7-0 PxIES RW Undefined Port X interrupt edge select
   //0b = PxIFG flag is set with a low-to-high transition.
   //1b = PxIFG flag is set with a high-to-low transition
   // now set the pin to cause falling edge interrupt event
   // P1.1 is falling edge event
   SWITCH_1_PORT->IES |= SWITCH_1_PIN; 
	
   // now set the pin to cause falling edge interrupt event
   NVIC_IPR8 = (NVIC_IPR8 & 0x00FFFFFF)|0x40000000; // priority 2
	
   // enable Port 1 - interrupt 35 in NVIC	
   NVIC_ISER1 = 0x00000008;  
	
   // enable interrupts  (// clear the I bit	)
   EnableInterrupts();              
}

// PORT 1 IRQ Handler
// LJBeato
// Will be triggered if any pin on the port causes interrupt
//
// Derived From: Jonathan Valvano
void PORT1_IRQHandler(void)
{
	int numSeconds = 0;

	// First we check if it came from Switch1 ?
   if(P1->IFG & SWITCH_1_PIN)  
   {
       // acknowledge P1.1 is pressed, by setting BIT1 to zero - remember P1.1 is switch 1   
	   // clear flag, acknowledge   
       SWITCH_1_PORT->IFG &= ~SWITCH_1_PIN;

       startEngine = TRUE;
   }
}

int main(void)
{
    /* Perform initializations */
    DisableInterrupts();
    uart0_init();
    Switch1_Interrupt_Init();
    
    // DC Motors
    TIMER_A0_PWM_Init(10000, 0, 1);  // PWM signal on P2.4
    TIMER_A0_PWM_Init(10000, 0, 2);  // PWM signal on P2.5
    
    // Servo
    TIMER_A2_PWM_Init(50, 0, 1);     // PWM signal on P5.6
    
    INIT_Camera();                   // ADC init inside
    EnableInterrupts();

    /* Car operations */
    while(1)
    {
        if (startEngine)
        {
            
        }
    }
}
