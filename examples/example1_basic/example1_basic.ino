#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

void setup(){

	Wire.begin();

	Serial.begin(115200);
	//Serial.println("Hey.");

	if( !myISM.begin() ){
	//	Serial.println("Did not begin.");
		while(1);
	}


	myISM.setAccelDataRate(0);
	myISM.setGyroDataRate(8);
	//myISM.setAccelFullScale(2); 
	myISM.setBlockDataUpdate(true);

}

void loop(){

	if( myISM.checkStatus() ){
		//myISM.getAccel(&accelData);
		myISM.getGyro(&gyroData);
		//Serial.print("Accelerometer: ");
		//Serial.print("X: ");
		//Serial.print(accelData.xData);
		//Serial.print(" ");
		//Serial.print("Y: ");
		//Serial.print(accelData.yData);
		//Serial.print(" ");
		//Serial.print("Z: ");
		//Serial.print(accelData.zData);
		//Serial.println(" ");
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

