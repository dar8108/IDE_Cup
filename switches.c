
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
