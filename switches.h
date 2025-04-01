
#include "Common.h"
#include "CortexM.h"

// Switch 1
#define  SWITCH_1_PORT   P1
#define  SWITCH_1_PIN    BIT1

// Switch 2
#define  SWITCH_2_PORT   P1
#define  SWITCH_2_PIN    BIT4

/* Function prototypes */
extern void Switch1_Init(void);
extern void Switch2_Init(void);
void Switch1_Interrupt_Init(void);
void Switch2_Interrupt_Init(void);
extern BOOLEAN Switch1_Pressed(void);
extern BOOLEAN Switch2_Pressed(void);
