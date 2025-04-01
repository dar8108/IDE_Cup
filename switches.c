
#include "msp.h" 
#include "Common.h"
#include "switches.h"

void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	SWITCH_1_PORT->SEL0 &= ~SWITCH_1_PIN;
	SWITCH_1_PORT->SEL1 &= ~SWITCH_1_PIN;
	
	SWITCH_2_PORT->SEL0 &= ~SWITCH_2_PIN;
	SWITCH_2_PORT->SEL1 &= ~SWITCH_2_PIN;

	// configure as input
	SWITCH_1_PORT->DIR &= ~SWITCH_1_PIN;
	SWITCH_2_PORT->DIR &= ~SWITCH_2_PIN;
	
	// Resistor Enable for Pull-up Resistor
	SWITCH_1_PORT->REN |= SWITCH_1_PIN;
	SWITCH_2_PORT->REN |= SWITCH_2_PIN;
	SWITCH_1_PORT->OUT |= SWITCH_1_PIN;
	SWITCH_2_PORT->OUT |= SWITCH_2_PIN;
              
}

/*
/! Initialize switch 1
*/
void Switch1_Init()
{
    // configure PortPin for Switch 1 as port I/O 
	SWITCH_1_PORT->SEL0 &= ~SWITCH_1_PIN;
	SWITCH_1_PORT->SEL1 &= ~SWITCH_1_PIN;
    
    // configure as input
	SWITCH_1_PORT->DIR &= ~SWITCH_1_PIN;
    
    // Resistor Enable for Pull-up Resistor
	SWITCH_1_PORT->REN |= SWITCH_1_PIN;
	SWITCH_1_PORT->OUT |= SWITCH_1_PIN;
}

/*
/! Initialize switch 2
*/
void Switch2_Init()
{
    // configure PortPin for Switch2 as port I/O 	
	SWITCH_2_PORT->SEL0 &= ~SWITCH_2_PIN;
	SWITCH_2_PORT->SEL1 &= ~SWITCH_2_PIN;

	// configure as input
	SWITCH_2_PORT->DIR &= ~SWITCH_2_PIN;
	
	// Resistor Enable for Pull-up Resistor
	SWITCH_2_PORT->REN |= SWITCH_2_PIN;
	SWITCH_2_PORT->OUT |= SWITCH_2_PIN;
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

void Switch2_Interrupt_Init(void)
{
   // disable interrupts
   DisableInterrupts();
	
   // initialize the Switch as per previous lab
   Switch2_Init();
	
   // now set the pin to cause falling edge interrupt event
   // P1.4 is falling edge event
   SWITCH_2_PORT->IES |= SWITCH_2_PIN;
  
   // clear flag4 (reduce possibility of extra interrupt)
   SWITCH_2_PORT->IFG &= ~SWITCH_2_PIN; 
  
   // arm interrupt on P1.4 
   SWITCH_2_PORT->IE |= SWITCH_2_PIN;      
	
   // now set the pin to cause falling edge interrupt event
   NVIC_IPR8 = (NVIC_IPR8&0x00FFFFFF)|0x40000000; // priority 2
	
   // enable Port 1 - interrupt 35 in NVIC
   NVIC_ISER1 = 0x00000008;         
	
   // enable interrupts  (// clear the I bit	)
   EnableInterrupts();              
	
}

//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch1_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	
	// check if pressed
	if ( (SWITCH_1_PORT->IN & SWITCH_1_PIN) != SWITCH_1_PIN)
	{
		  retVal = TRUE;
	}
	
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch2_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	
	// check if pressed
	if ( (SWITCH_2_PORT->IN & SWITCH_2_PIN) != SWITCH_2_PIN)
	{
		  retVal = TRUE;
	}

	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
