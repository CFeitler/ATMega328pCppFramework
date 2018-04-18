/*
 * LCD.cpp
 *
 * Created: 17.04.2018 21:35:32
 *  Author: cfeit
 */ 


// Ansteuerung eines HD44780 kompatiblen LCD im 4-Bit-Interfacemodus
// http://www.mikrocontroller.net/articles/HD44780
// http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/LCD-Ansteuerung
//
// Die Pinbelegung ist über defines in lcd-routines.h einstellbar

#include <avr/io.h>
#include "LCD.h"
#include <util/delay.h>

void LcdDisplay::Init(void)
{
	// verwendete Pins auf Ausgang schalten (geht sicher auch eleganter)
	LCD_COMM_DDR |= ( (1<<LCD_RS)|(1<<LCD_EN) ); //RS, E
	LCD_DATA_DDR |= ( (1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7) ); //D4, D5, D6, D7
	
	// initial alle Ausgänge auf Null
	LCD_DATA_PORT &= 0x0F;	//Alle das LCD betreffenden Pins auf 0
	LCD_COMM_PORT &= 0xFC;
	
	// !Wichtig! bei SainSmart LCD
	//First set D10 (PB2) to LOW. (only need to that once)
	PORTB &= ~(1<<PORTB2);
	//Then set the D10 (PB2) to OUTPUT for off and INPUT for on.
	DDRB &=~ (1<<PORTB2);	//To Input for ON!
	
	// warten auf die Bereitschaft des LCD
	_delay_ms( LCD_BOOTUP_MS );
	
	// Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
	SendFourBit( LCD_SOFT_RESET );
	_delay_ms( LCD_SOFT_RESET_MS1 );
	
	SendEnable();
	_delay_ms( LCD_SOFT_RESET_MS2 );
	
	SendEnable();
	_delay_ms( LCD_SOFT_RESET_MS3 );
	
	// 4-bit Modus aktivieren
	SendFourBit( LCD_SET_FUNCTION |
	LCD_FUNCTION_4BIT );
	_delay_ms( LCD_SET_4BITMODE_MS );
	
	// 4-bit Modus / 2 Zeilen / 5x7
	SendCommand( LCD_SET_FUNCTION |
	LCD_FUNCTION_4BIT |
	LCD_FUNCTION_2LINE |
	LCD_FUNCTION_5X7 );
	
	// Display ein / Cursor aus / Blinken aus
	SendCommand( LCD_SET_DISPLAY |
	LCD_DISPLAY_ON |
	LCD_CURSOR_OFF |
	LCD_BLINKING_OFF);
	
	// Cursor inkrement / kein Scrollen
	SendCommand( LCD_SET_ENTRY |
	LCD_ENTRY_INCREASE |
	LCD_ENTRY_NOSHIFT );
	
	Clear();
}

void LcdDisplay::WriteCharacter( uint8_t data )
{
	LCD_COMM_PORT |= (1<<LCD_RS);    // RS auf 1 setzen
	
	SendFourBit( data );            // zuerst die oberen,
	SendFourBit( data<<4 );         // dann die unteren 4 Bit senden
	
	_delay_us( LCD_WRITEDATA_US );
}

void LcdDisplay::SendCommand( uint8_t data )
{
	LCD_COMM_PORT &= ~(1<<LCD_RS);    // RS auf 0 setzen
	
	SendFourBit( data );             // zuerst die oberen,
	SendFourBit( data<<4 );           // dann die unteren 4 Bit senden
	
	_delay_us( LCD_COMMAND_US );
}

void LcdDisplay::Clear( void )
{
	SendCommand( LCD_CLEAR_DISPLAY );
	_delay_ms( LCD_CLEAR_DISPLAY_MS );
}

void LcdDisplay::SetCursorHome( void )
{
	SendCommand( LCD_CURSOR_HOME );
	_delay_ms( LCD_CURSOR_HOME_MS );
}

void LcdDisplay::SetCursorToPosition( uint8_t x, uint8_t y )
{
	uint8_t data;
	
	switch (y)
	{
		case 1:    // 1. Zeile
		data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
		break;
		
		case 2:    // 2. Zeile
		data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
		break;
		
		case 3:    // 3. Zeile
		data = LCD_SET_DDADR + LCD_DDADR_LINE3 + x;
		break;
		
		case 4:    // 4. Zeile
		data = LCD_SET_DDADR + LCD_DDADR_LINE4 + x;
		break;
		
		default:
		return;                                   // für den Fall einer falschen Zeile
	}
	
	SendCommand( data );
}

void LcdDisplay::WriteString( const char *data )
{
	while( *data != '\0' )
	WriteCharacter( *data++ );
}

void LcdDisplay::GenerateIndividualCharacter( uint8_t code, const uint8_t *data )
{
	// Startposition des Zeichens einstellen
	SendCommand( LCD_SET_CGADR | (code<<3) );
	
	// Bitmuster übertragen
	for ( uint8_t i=0; i<8; i++ )
	{
		WriteCharacter( data[i] );
	}
}

void LcdDisplay::SendEnable( void )
{
	LCD_COMM_PORT |= (1<<LCD_EN);     // Enable auf 1 setzen
	_delay_us( LCD_ENABLE_US );		  // kurze Pause
	LCD_COMM_PORT &= ~(1<<LCD_EN);    // Enable auf 0 setzen
}

void LcdDisplay::SendFourBit( uint8_t data )
{
	data &= 0xF0;                       // obere 4 Bit maskieren
	
	LCD_DATA_PORT &= ~(0xF0>>(4-LCD_DB));    // Maske löschen
	LCD_DATA_PORT |= (data>>(4-LCD_DB));     // Bits setzen
	SendEnable();
}