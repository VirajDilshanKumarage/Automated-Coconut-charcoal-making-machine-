#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

//****************************************************************************
//This function enables ADC module
void adc_init(unsigned char prescaler, unsigned char vref, unsigned char location) //(Location means left justified or vice versa)
{
	ADCSRA |= (1<<ADEN);				//enable ADC(ADCSRA - ADC Control and Status resistor)
	ADCSRA |= (1<<ADATE);				//enable Auto trigerring again and again pin
	adc_clkPrescaler(prescaler);		//Set prescaler
	adc_voltageReference(vref);			//Set voltage reference
	location ? (ADMUX |= (1<<ADLAR)) : (ADMUX &= ~(1<<ADLAR));	// If true location is left-justified and vice versa
																// if location = 1 - left justified location = 0 right justified
																//we normally use right justified(because left one must shift value)
	sei();  //Enable the global Interrupt Enable bit(interrupts.h)
	return;
}
//****************************************************************************
//This function selects the conversion time of the ADC
//NB: for no prescaler enter "1"
void adc_clkPrescaler(unsigned char prescaler)
{
	ADCSRA &= (~(1<<ADPS0)) & (~(1<<ADPS1)) & (~(1<<ADPS2)); // clear ADPS2:ADPS0 for initialization

	switch(prescaler){
		case 2:
			ADCSRA |= (1<<ADPS0);							//Prescaler 2
			break;
		case 4:
			ADCSRA |= (1<<ADPS1);							//Prescaler 4
			break;
		case 8:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1);				//Prescaler 8
			break;
		case 16:
			ADCSRA |= (1<<ADPS2);							//Prescaler 16
			break;
		case 32:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS2);				//Prescaler 32
			break;
		case 64:
			ADCSRA |= (1<<ADPS1) | (1<<ADPS2);				//Prescaler 64
			break;
		case 128:
			ADCSRA |= (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);	//Prescaler 128
			break;
		default:
			ADCSRA |= 0x00;			//don't change any thing if no prescalar
	}
	return;
}
//****************************************************************************
//This function selects the voltage reference
void adc_voltageReference(unsigned char Vref){
	ADMUX &= (~(1<<REFS0)) & (~(1<<REFS1)); // clear REFS1:REFS0 for initialization

	switch(Vref){
		case AREF:
			ADMUX |= 0x00;						//AREF reference, Internal Vref turned off
			break;
		case AVCC:
			ADMUX |= (1<<REFS0);				//AVCC with external capacitor at AREF PIN(We normally use this one)
			break;
		case INTERNAL:
			ADMUX |= (1<<REFS0) | (1<<REFS1);	//Internal 2.56V as voltage reference with external capacitor at AREF PIN
			break;
		default:
			ADMUX |= 0x00;
	}
	return;
}
//****************************************************************************
//This function tells the ADC module to start conversion
void adc_inputChannel(unsigned char input_channel){
	ADMUX &= 0xC0; // clear MUX4:MUX0 and ADLAR bit for initialization(We can easily use this one also)
	
	//ADMUX &= (~(1<<MUX4)) & (~(1<<MUX3)) & (~(1<<MUX2)) & (~(1<<MUX1)) & (~(1<<MUX0)); //Clear MUX4 to MUX0 
	//ADMUX &= ~(1<<ADLAR); //Clear ADLAR(this use to justify left or right)

	switch(input_channel)
	{
		case ADC0:
			ADMUX |= 0x00;							//PA0 pin in the board ADC0 = (0) = 00000(MUx4 to MUX0)
			break;
		case ADC1:
			ADMUX |= (1<<MUX0);						//PA1 pin in the board ADC1 = (1) = 00001(MUx4 to MUX0)
			break;
		case ADC2:
			ADMUX |= (1<<MUX1);						//PA2 pin in the board ADC2 = (2) = 00010(MUx4 to MUX0)
			break;
		case ADC3:
			ADMUX |= (1<<MUX0) | (1<<MUX1);			//PA3 pin in the board ADC3 = (3) = 00011(MUx4 to MUX0)
			break;
		case ADC4:
			ADMUX |= (1<<MUX2);						//PA4 pin in the board ADC4 = (4) = 00100(MUx4 to MUX0)
			break;
		case ADC5:
			ADMUX |= (1<<MUX0) | (1<<MUX2);			//PA5 pin in the board ADC5 = (5) = 00101(MUx4 to MUX0)
			break;
		case ADC6:
			ADMUX |= (1<<MUX1) | (1<<MUX2);			//PA6 pin in the board ADC6 = (6) = 00111(MUx4 to MUX0)
			break;
		case ADC7:
			//ADMUX |= 0x07;
			ADMUX |= (1<<MUX0) | (1<<MUX1) | (1<<MUX2);//PA7 pin in the board ADC7 = (7) = 00000(MUx4 to MUX0)
			break;
		default:
			ADMUX |= (0x1F & input_channel);
	}
	return;
}
//****************************************************************************
unsigned int adc_read(unsigned char input_channel){
	int analogReading = 0;
	unsigned char LSB;
	unsigned char MSB;
	adc_inputChannel(input_channel);
	ADCSRA |= (1<<ADSC);    //start conversion
	while((ADCSRA & (1<<ADIF)) == 0); //Wait until conversion is done
	LSB = ADCL;
	MSB = ADCH;
	analogReading |= MSB;
	analogReading <<= 8;
	analogReading |= LSB;
	return analogReading;
}
