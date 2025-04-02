
#include <stdio.h>
#include <stdlib.h>
#include "msp.h"
#include "uart.h"
#include "switches.h"
#include "leds.h"
#include "ADC14.h"
#include "Timer32.h"
#include "TimerA.h"
#include "i2c.h"
#include "oled.h"
#include "SysTickTimer.h"
#include "ControlPins.h"
#include "CortexM.h"

typedef enum steerDirection 
{
    STRAIGHT,
    LEFT,
    RIGHT
} STEER;

uint16_t line[128];
double lineDeriv[128]; 
BOOLEAN g_sendData;
BOOLEAN startEngine = FALSE;
STEER direction = STRAIGHT; 
double turnRate;

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

void turnOffMotors(void)
{
    TIMER_A0_PWM_DutyCycle(0, 1);
    TIMER_A0_PWM_DutyCycle(0, 2);
    TIMER_A0_PWM_DutyCycle(0, 3);
    TIMER_A0_PWM_DutyCycle(0, 4);   
}

// PORT 1 IRQ Handler
// LJBeato
// Will be triggered if any pin on the port causes interrupt
//
// Derived From: Jonathan Valvano
void PORT1_IRQHandler(void)
{   
	// First we check if it came from Switch1 ?
   if (Switch1_Pressed())  
   {
       // acknowledge P1.1 is pressed, by setting BIT1 to zero   
       SWITCH_1_PORT->IFG &= ~SWITCH_1_PIN;
       startEngine = TRUE;
   }
   else if (Switch2_Pressed())
   {
       // acknowledge P1.4 is pressed, by setting BIT4 to zero   
       SWITCH_1_PORT->IFG &= ~SWITCH_2_PIN;
       startEngine = FALSE;
       turnOffMotors();
   }
}

void Port3Init(void)
{
    // Set P3.6 and P3.7 as output
    P3->DIR |= (BIT6 | BIT7);
    
    // Configure Port 3 Pins 6 and 7 for GPIO
    P3->SEL0 &= ~(BIT6 | BIT7);
    P3->SEL0 &= ~(BIT6 | BIT7);
    
    // Disable motors
    P3->OUT &= ~(BIT6 | BIT7);
}

void Timer32_2_ISR(void)
{
    int i;
    uint16_t lineMax = 0;
    uint16_t leftMin, rightMin = 16384;
    int maxInd, leftMinInd, rightMinInd = 0;
    int leftDelta, rightDelta;
    
    // Find max, and left and right local mins, and where they occur
    for(i = 0; i < 128; i++)
    {
        if (lineMax < line[i])
        {
            lineMax = line[i];
            maxInd = i;
        }
        
        if ((leftMin > line[i]) && (i < 64))
        {
            leftMin = line[i];
            leftMinInd = i;
        }
        else if ((rightMin > line[i]) && (i > 64))
        {
            rightMin = line[i];
            rightMinInd = i;
        }
    }
    
    // Calculate distance from left min and the max, and the right min and the max
    leftDelta = maxInd - leftMinInd;
    rightDelta = rightMinInd - maxInd;
    
    // Check which direction the camera data is skewed 
    // Edge of track is right-center --> steer left
    if ( (leftDelta - rightDelta) > 20)  // Allow for some tolerance
    {
        direction = LEFT;
        turnRate = 0.075 + ((64-rightDelta)/64.0)*0.025;
    }
    // Edge of track is left-center --> steer right
    else if ( (rightDelta - leftDelta) > 20) // Allow for some tolerance
    {
        direction = RIGHT;
        turnRate = 0.075 - ((64-leftDelta)/64.0)*0.025;
    }
    else  // The deltas are about equal --> car in the center of track
    {
        direction = STRAIGHT;
    }
    
}

int main(void)
{   
    /* Perform initializations */
    DisableInterrupts();
    
    uart0_init();
    Switch1_Interrupt_Init();
    Switch2_Interrupt_Init();
    LED1_Init();
    LED2_Init();
    
    // DC Motors
    TIMER_A0_PWM_Init(10000, 0, 1);  // PWM signal on P2.4
    TIMER_A0_PWM_Init(10000, 0, 2);  // PWM signal on P2.5
    TIMER_A0_PWM_Init(10000, 0, 3);  // PWM signal on P2.6
    TIMER_A0_PWM_Init(10000, 0, 4);  // PWM signal on P2.7
    
    // Servo
    TIMER_A2_PWM_Init(50, 0, 1);     // PWM signal on P5.6
    
    // Initialize Timer32-2
    Timer32_2_Init(&Timer32_2_ISR, SystemCoreClock/1000, T32DIV1);
    
    INIT_Camera();                   // ADC init inside
    
    // Configure P3.6 and P3.7, initialize to off
    // Left wheel: reverse = 1, forward = 2
    // Right wheel: reverse = 3, forward = 4
    Port3Init();
    
    // Start Timer32-2
    Timer32_2_Enable();
    
    // Initialize OLED for camera debugging
    OLED_Init();
	OLED_display_on();
	OLED_display_clear();
    OLED_ClearTextArr();
    OLED_display_on(); 
    
    EnableInterrupts();

    /* Car operations */
    while(1)
    {
       
        if (startEngine)
        {
            OLED_DisplayCameraData(line);
            
            // Enable motors
            P3->OUT |= (BIT6 | BIT7);
            
            // Change servo direction depending on camera data
            switch (direction)
            {
                case STRAIGHT:
                  TIMER_A2_PWM_DutyCycle(0.075, 1);  
                  TIMER_A0_PWM_DutyCycle(0.2, 2);
                  TIMER_A0_PWM_DutyCycle(0.2, 4);
                  break;
                
                case LEFT:
                  TIMER_A2_PWM_DutyCycle(turnRate, 1);
                  TIMER_A0_PWM_DutyCycle(0.2, 2);  //(0.25-(turnRate*0.025/0.1))
                  TIMER_A0_PWM_DutyCycle(0.2, 4);
                  break;
                
                case RIGHT:
                  TIMER_A2_PWM_DutyCycle(turnRate, 1);
                  TIMER_A0_PWM_DutyCycle(0.2, 2);
                  TIMER_A0_PWM_DutyCycle(0.2, 4);
                  break;
                
                default:
                    break;                   
            }; 
            
        } 
        
    }
}
