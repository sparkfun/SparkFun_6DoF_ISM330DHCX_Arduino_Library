#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

#define MAG_ADDR 0x30
#define MAG_X_READ_REG 0x00
#define MAG_X_READ_LEN 0x07
#define MAG_WRITE_REG 0x09
#define MAG_WRITE_LEN 0x01

SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

sfe_hub_sensor_settings_t readsh;

readsh.address = MAG_ADDR; 
readsh.subAddress = MAG_X_READ_REG; 
readsh.length = MAG_X_READ_LEN; 

sfe_hub_sensor_settings_t writesh;

writesh.address = MAG_ADDR; 
writesh.subAddress = MAG_WRITE_REG; 
writesh.length = MAG_WRITE_LEN; 

uint8_t shRawData[MAG_X_READ_LEN] = 0;
uint32_t magXVal; 
uint32_t magYVal; 
uint32_t magZVal; 

void setup(){

	Wire.begin();

	Serial.begin(115200);

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

	// Output data rate for the external sensor
	myISM.setHubODR(ISM_SH_ODR_52Hz);
	// Which sensor (0-3) and settings to apply to sensor?
	myISM.setHubSensor(0, &sensor_hub);
	// Enable the ISM330DHCX as controller I2C
	myISM.enableSensorI2C(true);
	delay(1);
	myISM.setHubPullUps(); 
	
	// Apply ISM330DHCX settings
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
		myISM.readPeripheralSensor(&shRawData, MAG_X_READ_LEN);
		magXVal = shRawData[0] | shRawData [1] | (shRawData[6] & 0x20); 
		magXVal = shRawData[2] | shRawData [3] | (shRawData[6] & 0x20); 
		magXVal = shRawData[4] | shRawData [5] | (shRawData[6] & 0x20); 
	}

	delay(100);
}


void writeControlBit(sfe_hub_sensor_settings_t *toWrite)
{

	myISM.enableSensorI2C(true);

	myISM.setAccelDataRate(ISM_XL_ODR_OFF);
	myISM.setGyroDataRate(ISM_GY_ODR_OFF);
	myISM.setHubSensorWrite(&toWrite);

}
