#include <SPI.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

byte chipSelect = 41;
byte reg = 0;
byte ret = 0;

void setup(){


	Serial.begin(115200);
	pinMode(chipSelect, OUTPUT);
	digitalWrite(chipSelect, HIGH);

	SPI.begin();

	SPISettings mySpiSettings(2000000, MSBFIRST, SPI_MODE0);

	SPI.beginTransaction(mySpiSettings);
	digitalWrite(chipSelect, LOW);

  reg = ISM330DHCX_WHO_AM_I | 0x80;	
	SPI.transfer(reg);

	ret = SPI.transfer(0x00);
	
	digitalWrite(chipSelect, HIGH);
	SPI.endTransaction();

	if( ret == ISM330DHCX_ID )
		Serial.println("Success!");
	else {
		Serial.print("Failure: ");
		Serial.println(ret, HEX);
	}


	while(1);
	

	



//	if( !myISM.begin() ){
//		Serial.println("Did not begin.");
//		while(1);
//	}
//
//	myISM.deviceReset();
//
//	while( !myISM.getDeviceReset() ){ 
//		delay(1);
//	} 

	
//	myISM.setDeviceConfig();
//	myISM.setBlockDataUpdate();
//	
//	myISM.setAccelDataRate(4);
//	myISM.setGyroDataRate(4);
//	myISM.setAccelFullScale(2); 
//	myISM.setGyroFullScale(2); 
//
//	myISM.setSlopeFilter(4);
//	myISM.setAccelFilterLP2();


}

void loop(){

//	if( myISM.checkStatus() ){
//		myISM.getAccel(&accelData);
//		myISM.getGyro(&gyroData);
//		Serial.print("Accelerometer: ");
//		Serial.print("X: ");
//		Serial.print(accelData.xData);
//		Serial.print(" ");
//		Serial.print("Y: ");
//		Serial.print(accelData.yData);
//		Serial.print(" ");
//		Serial.print("Z: ");
//		Serial.print(accelData.zData);
//		Serial.println(" ");
//		Serial.print("Gyroscope: ");
//		Serial.print("X: ");
//		Serial.print(gyroData.xData);
//		Serial.print(" ");
//		Serial.print("Y: ");
//		Serial.print(gyroData.yData);
//		Serial.print(" ");
//		Serial.print("Z: ");
//		Serial.print(gyroData.zData);
//		Serial.println(" ");
//	}
//
//	delay(100);
}

