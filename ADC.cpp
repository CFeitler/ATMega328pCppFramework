/*
* ADC.cpp
*
* Created: 18.04.2018 21:03:12
*  Author: cfeit
*/

#include "ADC.h"
#include <util/delay.h>
#include <avr/io.h>


AnalogDigitalConverter::AnalogDigitalConverter(unsigned char channel, bool freerunning, ReferenceVoltage referenceVoltage )
{
	this->_channel = channel;
	this->_freerunning = freerunning;
	this->_referenceVoltage = referenceVoltage;
}

void AnalogDigitalConverter::Init()
{
	this->_SetReferenceVoltage(_referenceVoltage);
	if (F_CPU == 16000000UL)	//Sets prescaler to 128 so that ADC frequency is set to 125kHz. According to datasheet the frequency should be beow 200kHz.
	{
		ADCSRA |= (1<<ADPS0) | (1<< ADPS1) | (1<<ADPS2);
	}
	if(_freerunning)
	{
		this->SetFreeRunning();
	}
	this->SetChannel(_channel);
	ADCSRA |= (1<<ADEN);
}

void AnalogDigitalConverter::SetFreeRunning()
{
	_freerunning = true;
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));
	ADCSRA |= (1<<ADATE);
	ADCSRA |= (1<<ADSC);                //! Start converting.
}

void AnalogDigitalConverter::SetChannel(unsigned char channel)
{
	this->_channel = channel;
	//! Sicherheitsbeschaltung damit Der eingestellte Kanal kleiner 7 ist.
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
}

unsigned int AnalogDigitalConverter::GetAdcValue10Bit()
{
	if (!_freerunning)
	{
		this->_StartConversionAndWait();
	}
	return ADC;
}

unsigned int AnalogDigitalConverter::GetAdcValue10BitFromChannel(unsigned char channel)
{
	this->SetChannel(channel);
	return this->GetAdcValue10Bit();
}

unsigned char AnalogDigitalConverter::GetAdcValue8Bit()
{
	this->_SetResultLeftAdjust();
	unsigned char result;
	if (!_freerunning)
	{
		this->_StartConversionAndWait();
		result = ADCH;
	}
	else
	{
		_delay_ms(1); //wait one millisecond for ADC to complete in freerunning mode
		result = ADCH;
	}
	this->_SetResultRightAdjust();
	return result;
}

unsigned char AnalogDigitalConverter::GetAdcValue8BitFromChannel(unsigned char channel)
{
	this->SetChannel(channel);
	return this->GetAdcValue8Bit();
}

//private

void AnalogDigitalConverter::_SetReferenceVoltage(ReferenceVoltage ref)
{
	switch(ref)
	{
		case undefined:
		break;
		case AVCC:
		ADMUX |=(1<<REFS0);
		break;
	}
}

void AnalogDigitalConverter::_SetResultLeftAdjust()
{
	ADMUX |= (1<<ADLAR);
}

void AnalogDigitalConverter::_SetResultRightAdjust()
{
	ADMUX &= ~(1<<ADLAR);
}

void AnalogDigitalConverter::_StartConversionAndWait()
{
	//! Konvertierung starten.
	ADCSRA |= (1<<ADSC);
	//! Warten bis Konvertierung fertig.
	while(ADCSRA & (1<<ADSC) );
}