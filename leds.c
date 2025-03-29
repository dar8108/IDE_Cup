
#include "leds.h"

/*
/! Initialize LED1
*/
void LED1_Init(void)
{
   // configure PortPin for LED1 as port I/O 
   LED1_PORT->SEL0 &= ~LED1_PIN;
   LED1_PORT->SEL1 &= ~LED1_PIN;

   // make built-in LED1 LED high drive strength
   LED1_PORT->DS |= LED1_PIN;
    
   // make built-in LED1 out	 
   LED1_PORT->DIR |= LED1_PIN;

   // turn off LED
   LED1_PORT->OUT &= ~LED1_PIN;

}

/*
/! Initialize LED2
*/
void LED2_Init(void)
{
   // configure PortPin for LED2 as port I/O 
   LED2_PORT->SEL0 &= ~(LED2_RED_PIN | LED2_GREEN_PIN | LED2_BLUE_PIN);
   LED2_PORT->SEL1 &= ~(LED2_RED_PIN | LED2_GREEN_PIN | LED2_BLUE_PIN);

   // make built-in LED2 LEDs high drive strength
   LED1_PORT->DS |= (LED2_RED_PIN | LED2_GREEN_PIN | LED2_BLUE_PIN);

   // make built-in LED2 out	 
   LED2_PORT->DIR |= (LED2_RED_PIN | LED2_GREEN_PIN | LED2_BLUE_PIN);

   // turn off LED
   LED2_PORT->OUT &= ~LED2_RED_PIN;
   LED2_PORT->OUT &= ~LED2_GREEN_PIN;
   LED2_PORT->OUT &= ~LED2_BLUE_PIN;
}

/*
/! Turn on LED1 (RED)
*/
void LED1_On(void)
{
    LED1_PORT->OUT |= LED1_PIN;
}

/*
/! Turn off LED1
*/
void LED1_Off(void)
{
    LED1_PORT->OUT &= ~LED1_PIN;
}

/*
/! Returns the state of the LED
/! \return TRUE if LED is on,
/!         FALSE otherwise
*/
BOOLEAN LED1_State(void)
{
    return (LED1_PORT->OUT & LED1_PIN);
}

void LED2_Off(void)
{
   LED2_PORT->OUT &= ~LED2_RED_PIN;
   LED2_PORT->OUT &= ~LED2_GREEN_PIN;
   LED2_PORT->OUT &= ~LED2_BLUE_PIN;
}

/*
/! Turn on RED LED2
*/
void LED2_RED(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on RED LED
    LED2_PORT->OUT |= LED2_RED_PIN;
}

/*
/! Turn on GREEN LED2
*/
void LED2_GREEN(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on GREEN LED
    LED2_PORT->OUT |= LED2_GREEN_PIN;
}

/*
/! Turn on BLUE LED2
*/
void LED2_BLUE(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on BLUE LED
    LED2_PORT->OUT |= LED2_BLUE_PIN;
}

/*
/! Turn on CYAN LED2
*/
void LED2_CYAN(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on GREEN LED
    LED2_PORT->OUT |= LED2_GREEN_PIN;
    // Turn on BLUE LED
    LED2_PORT->OUT |= LED2_BLUE_PIN;
}

/*
/! Turn on MAGENTA LED2
*/
void LED2_MAGENTA(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on RED LED
    LED2_PORT->OUT |= LED2_RED_PIN;
    // Turn on BLUE LED
    LED2_PORT->OUT |= LED2_BLUE_PIN;
}

/*
/! Turn on YELLOW LED2
*/
void LED2_YELLOW(void)
{
    // Turn off any other colors
    LED2_Off();
    
    // Turn on RED LED
    LED2_PORT->OUT |= LED2_RED_PIN;
    // Turn on GREEN
    LED2_PORT->OUT |= LED2_GREEN_PIN;
}

/*
/! Turn on WHITE LED2
*/
void LED2_WHITE(void)
{    
    LED2_PORT->OUT |= LED2_RED_PIN;
    // Turn on GREEN LED
    LED2_PORT->OUT |= LED2_GREEN_PIN;
    // Turn on BLUE LED
    LED2_PORT->OUT |= LED2_BLUE_PIN;
}

