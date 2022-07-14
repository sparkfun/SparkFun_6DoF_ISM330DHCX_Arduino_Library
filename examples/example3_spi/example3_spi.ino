#include <SPI.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX_SPI myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

byte chipSelect = 41;

void setup(){

	SPI.begin();

	Serial.begin(115200);
	pinMode(chipSelect, OUTPUT);
	digitalWrite(chipSelect, HIGH);


	//SPISettings mySpiSettings(2000000, MSBFIRST, SPI_MODE0);

	if( !myISM.begin(chipSelect) ){
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
	
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_4g); 

	myISM.setGyroFullScale(ISM_500dps); 
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);

	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);



}

void loop(){

	if( myISM.checkStatus() ){
		myISM.getAccel(&accelData);
		myISM.getGyro(&gyroData);
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

