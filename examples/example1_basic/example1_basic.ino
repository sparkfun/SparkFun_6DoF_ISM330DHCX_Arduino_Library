#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 
sfe_raw_data_t myData; 

void setup(){

	Wire.begin();

	Serial.begin(115200);
	Serial.println("Hey.");

	Serial.print("Did we begin: "); 
	Serial.println(myISM.begin());

	//set ODR

	myISM.setAccelDataRate(6);
	myISM. 
}

void loop(){

//	if( myISM.checkAccelStatus() ){
//		myData = myISM.getAccel();
//		Serial.print("X: ");
//		Serial.print(myData.xAccelData);
//		Serial.print(" ");
//		Serial.print("Y: ");
//		Serial.print(myData.yAccelData);
//		Serial.print(" ");
//		Serial.print("Z: ");
//		Serial.print(myData.zAccelData);
//		Serial.println(" ");
//	}
//
//	delay(100);
}

