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
#include <util/delay.h>

int main(void)
{
    LcdDisplay myDisplay;
	myDisplay.Init();
	
    while (1) 
    {
		myDisplay.Clear();
		myDisplay.WriteString("Hallo Welt");
		_delay_ms(500);
    }
}

