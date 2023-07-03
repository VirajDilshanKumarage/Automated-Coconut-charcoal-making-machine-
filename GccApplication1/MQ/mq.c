#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include "mq.h"

//Gas Curves
//We get two points of the gas curves
//firstly we must watch the gas curve is linear or not(This formula can only use linear gases)
//First we select one Gas Curve and get two points as example (200,1.6) and (10000, 0.26)
//then we get the log value of the each x and y (log(200),log(1.6)) and (log(10000), log(0.26))
//the log points of the curve is(2.3, 0,2) and (4, -0.58)
//then calculate the slope of the curve m = =0.45
//Calculate this process in All gases using data sheet gas curves
float COcurve[3]    = {2.3, 0.176, -0.471};	//x = 200, log(200) = 2.30 , x = 1000, log(1000) = 3
											//y = 1.5, log(1.5) = 0.176, y = 0.7 ,log(0.7) = -0.154
											// m = y-y1/x-x1 = -0.471

//This is the resistance calculation function(This is a internal function we cannot declare this in a main function)

float ResistanceCalculation(int sensor_value){
	float sensor_voltage;
	float Rs_Gas_or_Air;
	
	sensor_voltage = (float)((float)sensor_value/1024) * 5;			//Why we divide 1024 because we have 10 bit ADC then wen can get 1024 values
														//why we multiply 5 because our Vcc is 5 volt
														
	Rs_Gas_or_Air = ((float)(5 - sensor_voltage )/(float)sensor_voltage)* RL_Value;	//This is the resistance calculation formula
																		//Rs = (Vcc * Rl)/sensor_vlotage + Rl
																		
	return Rs_Gas_or_Air;
}

//Sensor Calibration Function
//Before get sensor values we must calibrate it on clean air
//This function calibrates the sensor

float SensorCalibration(int sensor_value){
	float Rs_Air = 0;
	float Ro;
	int i;
	for(i=0; i<100; i++){
		Rs_Air += ResistanceCalculation(sensor_value);	//firstly we read ADC value coming from sensor
	}
	Rs_Air = Rs_Air/100;
	
	Ro = Rs_Air/Ro_Clean_Air_Factor;	//Ro is defined in a data sheet resistance of the sensor in 1000ppm Hydrogen
										//Nearly Ro value is 0.91 it depend on the AIR_clean
	return Ro;
}

//This sensor reads ADC value in a GAS We call this function Also
float ReadSensor(int sensor_value){
	float Rs_Gas = 0;
	float Rs;
	int i;
	for(i=0; i<100; i++){
		Rs_Gas += ResistanceCalculation(sensor_value); //Read the ADC value of the sensor in GAS
	}
	Rs_Gas = Rs_Gas/100;
	Rs = Rs_Gas;
	
	Rs = sensor_value;
	return Rs;
}

//This function returns Gas presentage 
//Rs_Ro ratio means Rs/Ro value (Our given graph y-Axis is Rs/Ro) because we use ratio

double GetGasPresentage(double Rs_Ro_ratio, int gas_id){ 
	if(gas_id == CO){
		return GasPresentage(Rs_Ro_ratio,COcurve);
	}
	else{
		return 0;
	}
}

//This function calculates the presentage in ppm using gas curve points
//x = (y - y1)/m + x1 this use for find the x value depend on Rs/Ro ratio
//We use log of Rs/Ro because we firstly get all the points in log format
//then we get power of the x we can get real value

double GasPresentage(double Rs_Ro_ratio, float *curve){
	double x;
	double presentage;
	
	//x = ((log(Rs_Ro_ratio) - curve[1])/curve[2]) + curve[0];
	x = log10(Rs_Ro_ratio);
	x = x - curve[1]; 
	x = (x/curve[2]);
	x = x + curve[0];
	presentage = pow(10,x);
	return presentage;
}



