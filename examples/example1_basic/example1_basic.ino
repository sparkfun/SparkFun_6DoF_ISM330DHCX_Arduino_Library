#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 

void setup(){

	Wire.begin();

	Serial.begin(115200);
	Serial.println("Hey.");

	if( !myISM.begin() ){
		Serial.println("Did not begin.");
		while(1);
	}


	if( !myISM.setAccelDataRate(8) ){
		Serial.println("Could not set output data rate for the accelerometer.");
		while(1);
	}
	//myISM.setAccelFullScale(2); 
}

void loop(){

	if( myISM.checkAccelStatus() ){
		myISM.getAccel(&accelData);
		Serial.print("X: ");
		Serial.print(accelData.xData);
		Serial.print(" ");
		Serial.print("Y: ");
		Serial.print(accelData.yData);
		Serial.print(" ");
		Serial.print("Z: ");
		Serial.print(accelData.zData);
		Serial.println(" ");
	}

	delay(100);
}

