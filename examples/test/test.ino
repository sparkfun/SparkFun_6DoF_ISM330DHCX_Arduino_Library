#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <SparkFun_MMC5983MA_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_MMC5983MA

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 


SFE_MMC5983MA myMag;


unsigned int currentX = 0;
unsigned int currentY = 0;
unsigned int currentZ = 0;
double normalizedX = 0;
double normalizedY = 0;
double normalizedZ = 0;


void setup(){

	Wire.begin();

	Serial.begin(115200);

	if( !myISM.begin() ){
//		Serial.println("Did not begin.");
		while(1);
	}

  if (myMag.begin() == false)
	{
//			Serial.println("MMC5983MA did not respond - check your wiring. Freezing.");
			while (1) ;
	}

	myISM.deviceReset();
	myMag.softReset();

	while( !myISM.getDeviceReset() ){ 
		delay(1);
	} 

//	Serial.println("Reset.");
//	Serial.println("Applying settings.");
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
//		Serial.print("Accelerometer: ");
//		Serial.print("AX: ");
//		Serial.print(accelData.xData);
//		Serial.print(" ");
//		Serial.print("AY: ");
//		Serial.print(accelData.yData);
//		Serial.print(" ");
//		Serial.print("AZ: ");
//		Serial.print(accelData.zData);
//		Serial.println();
//		Serial.print("Gyroscope: ");
		//Serial.print("GX: ");
		//Serial.print(gyroData.xData);
		//Serial.print(" ");
		//Serial.print("GY: ");
		//Serial.print(gyroData.yData);
		//Serial.print(" ");
		//Serial.print("GZ: ");
		//Serial.print(gyroData.zData);
		//Serial.println();

	}

	currentX = myMag.getMeasurementX();
	currentY = myMag.getMeasurementY();
	currentZ = myMag.getMeasurementZ();

	normalizedX = (double)currentX - 131072.0;
	normalizedX /= 131072.0;
	normalizedY = (double)currentY - 131072.0;
	normalizedY /= 131072.0;
	normalizedZ = (double)currentZ - 131072.0;
	normalizedZ /= 131072.0;

	//Serial.print("Magnetometer: ");
	Serial.print("MX: ");
	Serial.print(normalizedX * 8, 5);

	Serial.print(" MY: ");
	Serial.print(normalizedY * 8, 5);

	Serial.print(" MZ: ");
	Serial.print(normalizedZ * 8, 5);
	Serial.println();

	delay(100);
}

