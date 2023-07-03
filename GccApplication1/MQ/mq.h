#define RL_Value				10 //Load resistance 10Kohm (this define at sensor data sheet), This is the board resistance
#define Ro_Clean_Air_Factor		9.9//This is the Clean air factor, Sensor resistance in the air

//Gases ID's, We can Include any Gas ID for Sensor will You use
#define LPG						0			//MQ2- Gas sensor
#define SMOKE					1			//Mq2- Gas senor
#define CO						2			//Mq9- Gas sensor


//These are the Function Prototypes
float ResistanceCalculation(int sensor_value);
float SensorCalibration(int sensor_value);
float ReadSensor(int sensor_value);
double GetGasPresentage(double Rs_Ro_ratio, int gas_id);
double GasPresentage(double Rs_Ro_ratio, float *curve);


/*Sample Code for Any gas sensor
	//firstly define Ro variable
	float Ro = 10; //Initial value is 10Khm
	
	int main(void){
		//before using this sensor we must initialize ADC initialize
		adc_init(16, AVCC, 0); //This says prescaler set 16 reference voltage is AVcc and aligned right
		
		//if you want to sensor value in LCD you must initialize LCD
		lcd_init(LCD_CURSOR_ON);
		
		//First use in sensor we calibrate it in testing
		lcd_puts("calibrating");
		Ro = SensorCalibration();    //This can takes few minutes 
		
		lcd_clesar();
		lcd_puts("Calibration Done...");
		
		//Then we can get resistance of the sensor in clean air
		//firstly we must convert float value into the string value
		//therefore we use dftoa() function included in stdlib.h
		dtostrf(Ro, 6, 2, RoStr);   //RoStr is a char Array
		lcd_clrscr();
		lcd_puts(RoStr);
		_delay_ms(1000);
		lcd_clrscr();
		
		while(1){
			lcd_puts("LPG: ");
			int gas_presentage = GetGasPresentage(ReadSensor()/Ro, LPG);	//This point is very important point
																			//We must put ReadSensor()/Ro in this function
																			//Then return GAS ID
																			//We can use a ratio variable also for Understanding
			//then we convert our integer value into string using itoa() function included in stdlib.h
			itoa(gas_presentage, gas_presentage_str, 10);
			
			lcd_puts(gas_presentage_str);
		}
	}
*/