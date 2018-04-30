/*
* Ringbuffer.h
*
* Created: 21.04.2018 15:26:20
*  Author: cfeit
*/


#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_


class Ringbuffer
{
	public:
	///Inits char Ringbuffer with size
	void Init(unsigned char);
	/// Flushes Ringbuffer
	void Flush(void);
	/// Writes a single character to Ringbuffer. Can throw exceptions.
	void WriteCharacterToRingbuffer(unsigned char);
	/// Writes a character string to Ringbuffer. Can throw exceptions.
	void WriteStringToRingbuffer(char*);
	/// reads a character string to Ringbuffer. returns 0 im empty. Can throw exceptions.
	unsigned char ReadCharacterFromRingbuffer(void);
	
	protected:
	private:
	unsigned char* _ringbuffer;
	unsigned char* _read;
	unsigned char* _write;
	unsigned char _bufferSize;
	unsigned char _getFreeSpace(void);
	void increasePointer(unsigned char*);
};



#endif /* RINGBUFFER_H_ */