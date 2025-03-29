
#include "calcPeriod.h"
#include "msp.h"

///////////////////////////////////////////////////////
//
// Helper function
//
///////////////////////////////////////////////////////
unsigned long  CalcPeriodFromFrequency (double Hz)
{
	double period = 0.0;
	period = (double)SystemCoreClock/Hz;
	return (unsigned long) period;
}
