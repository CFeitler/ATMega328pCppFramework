/*
* ADC.h
*
* Created: 18.04.2018 20:56:49
*  Author: cfeit
*/


#ifndef ADC_H_
#define ADC_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif



enum ReferenceVoltage
{
	undefined,
	AVCC
};

class AnalogDigitalConverter
{
	public:
	void Init(void);
	void SetFreeRunning(void);
	void SetChannel(unsigned char);
	unsigned int GetAdcValue10Bit(void);
	unsigned int GetAdcValue10BitFromChannel(unsigned char);
	unsigned char GetAdcValue8Bit(void);
	unsigned char GetAdcValue8BitFromChannel(unsigned char);
	AnalogDigitalConverter(unsigned char, bool, ReferenceVoltage);
	
	private:
	char _channel;
	bool _freerunning;
	ReferenceVoltage _referenceVoltage;
	void _SetReferenceVoltage(ReferenceVoltage);
	void _SetResultLeftAdjust(void);
	void _SetResultRightAdjust(void);
	void _StartConversionAndWait(void);
	
};

#endif /* ADC_H_ */