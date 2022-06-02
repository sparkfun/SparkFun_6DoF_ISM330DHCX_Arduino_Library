#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 


byte interrupt_pin = D1; 

void setup(){

	pinMode(interrupt_pin, INPUT);
	Serial.begin(115200);

	Wire.begin();

	if( !myISM.begin() ){
		Serial.println("Did not begin.");
		while(1);
	}

	myISM.deviceReset();

	while( !myISM.getDeviceReset() ){ 
		delay(1);
	} 

	Serial.println("Reset.");
	Serial.println("Applying settings.");
	delay(100);
	
	myISM.setDeviceConfig();
	myISM.setBlockDataUpdate();
	
	// Accelerometer config and enable
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_4g); 

	/// Accelerometer filter settings
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Accelerometer Interrupt Settings
	myISM.setAccelStatustoInt1();
	//myISM.setAccelStatustoInt2();

	// Gyro config and enable
	//myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
	//myISM.setGyroFullScale(ISM_500dps); 

	// Gyro Interrupt Settings
	//myISM.setGyroStatustoInt1();
	//myISM.setGyroStatustoInt2();



}

void loop(){

	if( digitalRead(interrupt_pin) == HIGH ){
		myISM.getAccel(&accelData);
	//	myISM.getGyro(&gyroData);
		Serial.print("Accelerometer: ");
		Serial.print("X: ");
		Serial.print(accelData.xData);
		Serial.print(" ");
		Serial.print("Y: ");
		Serial.print(accelData.yData);
		Serial.print(" ");
		Serial.print("Z: ");
		Serial.print(accelData.zData);
		Serial.println(" ");
		Serial.print("Gyroscope: ");
		Serial.print("X: ");
		Serial.print(gyroData.xData);
		Serial.print(" ");
		Serial.print("Y: ");
		Serial.print(gyroData.yData);
		Serial.print(" ");
		Serial.print("Z: ");
		Serial.print(gyroData.zData);
		Serial.println(" ");
	}

	delay(100);
}

