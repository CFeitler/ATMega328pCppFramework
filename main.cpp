/*
 * UCINCPP2.cpp
 *
 * Created: 17.04.2018 21:27:16
 * Author : cfeit
 */ 

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include "LCD.h"
#include "ADC.h"
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
    LcdDisplay myDisplay;
	myDisplay.Init();
	
	AnalogDigitalConverter myADC(0, false, AVCC);
	myADC.Init();
	
	unsigned int adcValue;
	char displayLine[16] = "Hallo Welt";
    while (1) 
    {
		adcValue = myADC.GetAdcValue10Bit();
		snprintf(displayLine,16, "ADC: %d",adcValue);
		myDisplay.Clear();
		myDisplay.WriteString(displayLine);
		_delay_ms(500);
    }
}

