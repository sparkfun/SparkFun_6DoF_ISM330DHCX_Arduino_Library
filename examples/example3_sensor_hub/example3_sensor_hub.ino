#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

// 8 bit addressing for Sensor Hub
#define MAG_ADDR_READ 0x61 // (0x30 << 1) | 1)
#define MAG_ADDR_WRITE 0x60 // (0x30 << 1)

#define MAG_READ_REG 0x00 // Read from 0x00
#define MAG_READ_LEN 0x07 // Read seven times consecutively
#define MAG_WRITE_REG 0x09 // INT_CTRL0 - register to initiate measurements on Magnetometer
#define MAG_WRITE_DATA 0x01 // Value to write to INT_CTRL0 

SparkFun_ISM330DHCX myISM; 

sfe_ism_data_t accelData; 
sfe_ism_data_t gyroData; 

// Settings for sensor hub
sfe_hub_sensor_settings_t readMMC, writeMMC;

uint8_t shRawData[MAG_READ_LEN] = {};
unsigned int magXVal; 
unsigned int magYVal; 
unsigned int magZVal; 

double normalizedX;
double normalizedY;
double normalizedZ;

void setup(){


	writeMMC.address = MAG_ADDR_WRITE; 
	writeMMC.subAddress = MAG_WRITE_REG; 
	writeMMC.lenData = MAG_WRITE_DATA; 

	readMMC.address = MAG_ADDR_READ; 
	readMMC.subAddress = MAG_READ_REG; 
	readMMC.lenData = MAG_READ_LEN; 

	Wire.begin();

	Serial.begin(115200);

	if( !myISM.begin() ){
		Serial.println("Did not begin.");
		while(1);
	}

	myISM.deviceReset();
	myISM.resetSensorHub();

	while( !myISM.getDeviceReset() ){ 
		delay(1);
	} 

	Serial.println("Reset.");
	Serial.println("Applying settings.");
	delay(100);

	myISM.setDeviceConfig();
	myISM.setBlockDataUpdate();


	// Set the number of peripheral sensor to be read by the 6DoF
	myISM.setNumberHubSensors(0);
	// Enable pullup resistors on SDX/SCX
	myISM.setHubPullUps(); 
	// Output data rate for the external sensor
	myISM.setHubODR(ISM_SH_ODR_104Hz);


	// Send control bit to Magnetometer
	writeControlBit(writeMMC);

	// Set read settings for external sensor "zero" (0-3)
	myISM.setHubSensorRead(0, &readMMC);

	// Enable the 6DoF as a controller I2C
	// All configurations to the sensor hub must occur while the controller I2C 
	// bus is powered down.
	myISM.enableSensorI2C(true);
	
	// Apply acceleromter settings
	myISM.setAccelFullScale(ISM_4g); 
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Apply gyroscope settings
	myISM.setGyroFullScale(ISM_500dps); 
	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);

	// Enable Sensors
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);

}

void loop(){

	if( myISM.checkStatus() ){

		myISM.getAccel(&accelData);
		myISM.getGyro(&gyroData);

		if( myISM.getExternalSensorNack(0) )
			Serial.println("MMC Nacked...");

		if( myISM.getHubStatus() )
		{

			myISM.readPeripheralSensor(shRawData, (uint8_t)MAG_READ_LEN);

			// Shift raw data
			magXVal = (((uint32_t)shRawData[0]) << 10) | (((uint32_t)shRawData[1]) << 2) | ((((uint32_t)shRawData[6]) >> 6) & 0x03);
			magYVal = (((uint32_t)shRawData[2]) << 10) | (((uint32_t)shRawData [3]) << 2) | ((((uint32_t)shRawData[6]) >> 4) & 0x03);
			magZVal = (((uint32_t)shRawData[4]) << 10) | (((uint32_t)shRawData [5]) << 2) | ((((uint32_t)shRawData[6]) >> 2) & 0x03); 		

			// Convert raw data
			normalizedX = (double)magXVal - 131072.0;
			normalizedX = (normalizedX/131072.0) * 8;

			normalizedY = (double)magYVal - 131072.0;
			normalizedY = (normalizedY/131072.0) * 8;

			normalizedZ = (double)magZVal - 131072.0;
			normalizedZ = (normalizedZ/131072.0) * 8;

			// Z axis between magnetometer and acclerometer are opposite. 
			normalizedZ = normalizedZ * (-1); 

			Serial.print("Magnetometer: ");
			Serial.print("X: ");
			Serial.print(normalizedX);
			Serial.print(" ");
			Serial.print("Y: ");
			Serial.print(normalizedY);
			Serial.print(" ");
			Serial.print("Z: ");
			Serial.print(normalizedZ);
			Serial.println("\n");

			// Send another bit to take more measurements
			writeControlBit(writeMMC);
			myISM.setHubSensorRead(0, &readMMC);
			myISM.enableSensorI2C(true);

			myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
			myISM.setGyroDataRate(ISM_GY_ODR_104Hz);

		}


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


bool writeControlBit(sfe_hub_sensor_settings_t toWrite)
{

	// Sensors must be off for sensor hub configuration.
	myISM.setAccelDataRate(ISM_XL_ODR_OFF);
	myISM.setGyroDataRate(ISM_GY_ODR_OFF);
	myISM.enableSensorI2C(false);

	// 300us until 6DoF I2C is powered down
	delayMicroseconds(310);

	// Configure a write
	myISM.setHubSensorWrite(&toWrite);
	// Re-enable sensor for write
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.enableSensorI2C(true);

	// Wait for write to complete
	while( !myISM.getHubStatus() ){
//		Serial.print(".");
		delay(1);
	}
	
	
	// Turn off sensor
	myISM.enableSensorI2C(false);
	delayMicroseconds(310);

	return true; 

}
