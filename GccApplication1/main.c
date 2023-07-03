/*
 * GccApplication1.c
 *
 * Created: 5/20/2022 3:40:22 PM
 * Author : GROUP 02 -Coconut Charcoal Making Machine
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL

#include "ADC/adc.h"
#include "MQ/mq.h" 


int gas_presentage_CO=0;

int gas_presentage_Smoke=0;


int c=0;
int c2=0;
int main(void)
{
	int period=100;
	/* Replace with your application code */
	//output

	DDRD=0xFF;
	DDRC|=(1<<DDC0);
	DDRC|=(1<<DDC1);
	DDRC|=(1<<DDC3);

	DDRC|=(1<<DDC5);
	
	DDRC|=(1<<DDC7);

	//input
	DDRA&=~(1<<DDA0);
	DDRA&=~(1<<DDA1);
	DDRA&=~(1<<DDA2);
	DDRA&=~(1<<DDA3);
	DDRA&=~(1<<DDA4);
	DDRB&=~(1<<DDB7);
	DDRB&=~(1<<DDB6);
	
	//smoke
	DDRA&=~(1<<DDA5);
	//co
	DDRA&=~(1<<DDA6);
	
	//Initialize ADC
	adc_init(1,AVCC,0);
	float RoCO = 0;
	
	//Initialize ADC
	adc_init(1,AVCC,3);
	float RoSmoke = 0;
	

	_delay_ms(100);
	//Get the Resistance of the sensor in fresh Air
	RoCO = SensorCalibration(adc_read(PA0));
	
	//Get the Resistance of the sensor in fresh Air
	RoSmoke = SensorCalibration(adc_read(PA3));
	
	while (1)
	{
	 if(PINB & (1<<PB6) )
	 {	
		 if(c==0 && c2==0)
		 {
		// PIR1......................
		if(PINA & (1<<PINA1))
		{
			PORTC|=(1<<PC0);
			_delay_ms(1500);
			PORTC&=~(1<<PC0);
			
			for(int i=0;i<3;i++)
			{
				
					PORTC|=(1<<PC1);
					_delay_ms(300);
					PORTC&=~(1<<PC1);
					_delay_ms(300);
				
			}
		  }
			c=1;
			
		}
		
		
		if(c==1 && c2==0)
		{
			
			
			//enable flam sensor push toggle logic
			
			
			
			//enable intake fan
			if(PINA & (1<<PINA2))
			{
				PORTC&=~(1<<PC3);
				
				
			}	
			else
		{
				
			
			//again spary petroll
			PORTC|=(1<<PC0);
			_delay_ms(1000);
			PORTC&=~(1<<PC0);
			
			//again spark three times
			for(int i=0;i<3;i++)
			{
				
					PORTC|=(1<<PC1);
					_delay_ms(300);
					PORTC&=~(1<<PC1);
					_delay_ms(300);
				
			}
			
			PORTC|=(1<<PC3);
			_delay_ms(1000);
			PORTC&=~(1<<PC3);
			_delay_ms(1000);
			
			c2=1;
		  }
			
		
		if(c2==1)
		{	
			// enable gas sensor smoke push toggle logic
			
			
			
			//enable exhaust fan
			
			//Get the Gas percentage in Smoke gas
			gas_presentage_Smoke = GetGasPresentage(ReadSensor(adc_read(PA3))/RoSmoke, CO);
			
			if((gas_presentage_Smoke < 2000) && (PINA & (1<<PA6)))
			{
				_delay_ms(1000);
				PORTC|=(1<<PC5);
				_delay_ms(1000);
				
			}
			PORTC&=~(1<<PC5);
			

			// enable gas sensor2 CO push toggle logic
		
			
			
			
			//Get the Gas percentage in CO gas
			gas_presentage_CO = GetGasPresentage(ReadSensor(adc_read(PA0))/RoCO, CO);

			//stepper motor 1..........................
			if((gas_presentage_CO < 110) && (PINA & (1<<PA6)))
			{

				
				/* Rotate Stepper Motor clockwise with Half step sequence */
				for(int i=0;i<4;i++)
				{
					PORTD = 0x09;
					_delay_ms(period);
					PORTD = 0x08;
					_delay_ms(period);
					PORTD = 0x0C;
					_delay_ms(period);
					PORTD = 0x04;
					_delay_ms(period);
					PORTD = 0x06;
					_delay_ms(period);
					PORTD = 0x02;
					_delay_ms(period);
					PORTD = 0x03;
					_delay_ms(period);
					PORTD = 0x01;
					_delay_ms(period);
				}
				PORTD = 0x09;		/* Last step to initial position */
				_delay_ms(period);
				_delay_ms(1000);

				/* Rotate Stepper Motor Anticlockwise with Full step sequence */
				for(int i=0;i<4;i++)
				{
					PORTD = 0x09;
					_delay_ms(period);
					PORTD = 0x03;
					_delay_ms(period);
					PORTD = 0x06;
					_delay_ms(period);
					PORTD = 0x0C;
					_delay_ms(period);
				}
				PORTD = 0x09;
				_delay_ms(period);
				
				
				// enable pir sensor2 push toggle logic
			
				
				//pir sensor2...............................
				
				if(PINA & (1<<PINA4))
				{
					PORTC|=(1<<PC7);
					_delay_ms(2000);
					
					PORTC&=~(1<<PC7);
					
					
				
				
				
				// stepper motor 2.........................................
				
				
				/* Rotate Stepper Motor clockwise with Half step sequence */
				for(int i=0;i<4;i++)
				{
					PORTD = 0x90;
					_delay_ms(period);
					PORTD = 0x80;
					_delay_ms(period);
					PORTD = 0xC0;
					_delay_ms(period);
					PORTD = 0x40;
					_delay_ms(period);
					PORTD = 0x60;
					_delay_ms(period);
					PORTD = 0x20;
					_delay_ms(period);
					PORTD = 0x30;
					_delay_ms(period);
					PORTD = 0x10;
					_delay_ms(period);
				}
				PORTD = 0x90;		/* Last step to initial position */
				_delay_ms(period);
				

				/* Rotate Stepper Motor Anticlockwise with Full step sequence */
				for(int i=0;i<4;i++)
				{
					PORTD = 0x90;
					_delay_ms(period);
					PORTD = 0x30;
					_delay_ms(period);
					PORTD = 0x60;
					_delay_ms(period);
					PORTD = 0xC0;
					_delay_ms(period);
				}
				PORTD = 0x90;
				_delay_ms(period);
				PORTC&=~(1<<PC3);
				PORTC&=~(1<<PC5);
				_delay_ms(10000);
				
			}
		  }
	   	}
			
	}
	}
  }

}

