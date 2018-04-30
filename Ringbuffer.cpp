/*
* Ringbuffer.cpp
*
* Created: 21.04.2018 15:35:12
*  Author: cfeit
*/

#include "Ringbuffer.h"
#include "ErrorCodes.h"
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void Ringbuffer::Init(unsigned char size)
{
	if (size > MAX_BUFFER_SIZE)
	{
		size = MAX_BUFFER_SIZE;
	}
	this->_bufferSize = size;
	this->_ringbuffer = (unsigned char*) malloc(size*sizeof(unsigned char));
	Flush();
}

void Ringbuffer::Flush(void)
{
	this->_read = _ringbuffer;
	this->_write = _ringbuffer+1;
}

void Ringbuffer::WriteCharacterToRingbuffer(unsigned char character)
{
	if(_getFreeSpace()<1)
	{
		throw BUFFER_TO_SMALL;
		return;
	}
	*_write = character;
	try
	{
		increasePointer(_write);
	}
	catch (int er)
	{
		throw er;
	}
}

void Ringbuffer::WriteStringToRingbuffer(char* string)
{
	if (_getFreeSpace() < strnlen(string,MAX_BUFFER_SIZE))
	{
		throw BUFFER_FULL;
		return;
	}
	
	try
	{
		for (char* stringIterator = string;
		stringIterator<string+strnlen(string,MAX_BUFFER_SIZE);
		stringIterator++)
		{
			*_write = *stringIterator;
			increasePointer(_write);
		}
	}
	catch (int ex)
	{
		throw ex;
	}
}

unsigned char Ringbuffer::ReadCharacterFromRingbuffer(void)
{
	unsigned char temp = *_read;
	try
	{
		increasePointer(_read);
		return temp;
	}
	catch (int ex)
	{
		throw ex;
	}
}


void Ringbuffer::increasePointer(unsigned char* pointer)
{
	pointer++;
	if (pointer == _ringbuffer+_bufferSize+1)
	{
		pointer = _ringbuffer;
	}
	if (_read == _write)
	{
		throw BUFFER_OVERFLOW;
	}
}

unsigned char Ringbuffer::_getFreeSpace()
{
	if (_read == _write
	||  _read == _write-1
	||  (_read == _ringbuffer+_bufferSize && _write == _ringbuffer))
	{
		return _bufferSize-1;
	}
	if (_read < _write)
	{
		return _write-_read-1;
	}
	return _bufferSize-(_read-_write)-1;
}