/*
  example5-faster_I2C

  This example shows how to retrieve accelerometer and gyroscopic data as fast
  as the I2C bus will allow.

  If you want to run the accel and gyro at 416Hz, you need to use 400kHz I2C.
  See the Wire.setClock(400000); at the start of setup().

  This example also allows you to run the accel and gyro at different rates,
  should you want to.

  Tested on ATmega328P (16MHz). Please see this issue for more detail:
  https://github.com/sparkfun/SparkFun_6DoF_ISM330DHCX_Arduino_Library/issues/7#issuecomment-1295767690
  
	Please refer to the header file for more possible settings, found here:
	..\SparkFun_6DoF_ISM330DHCX_Arduino_Library\src\sfe_ism330dhcx_defs.h

  Written by Paul Clark @ SparkFun Electronics, October 2022
  Based on original work by Elias Santistevan

	Product:

		https://www.sparkfun.com/products/19764

  Repository:

		https://github.com/sparkfun/SparkFun_6DoF_ISM330DHCX_Arduino_Library

  SparkFun code, firmware, and software is released under the MIT
	License	(http://opensource.org/licenses/MIT).
*/

#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Structs for X,Y,Z data
sfe_ism_data_t accelData;
sfe_ism_data_t gyroData;

void setup()
{

	Wire.begin();

  // Uncomment the next line to use 400kHz I2C. Essential when running the accel and gyro at 416Hz or faster.
	//Wire.setClock(400000);

	Serial.begin(115200);

	while (!myISM.begin())
	{
		Serial.println("ISM did not begin. Please check the wiring...");
		delay(1000);
	}

	// Reset the device to default settings. This if helpful is you're doing multiple
	// uploads testing different settings.
	myISM.deviceReset();

	// Wait for it to finish reseting
	while (!myISM.getDeviceReset())
	{
		delay(1);
	}

	Serial.println("Reset.");
	Serial.println("Applying settings.");
	delay(100);

	myISM.setDeviceConfig();
	myISM.setBlockDataUpdate();

	// Set the output data rate and precision of the accelerometer
  // Note: we're using 208Hz for accel and 104Hz for gyro
	myISM.setAccelDataRate(ISM_XL_ODR_208Hz);
	myISM.setAccelFullScale(ISM_4g);

	// Set the output data rate and precision of the gyroscope
  // Note: we're using 208Hz for accel and 104Hz for gyro
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
	myISM.setGyroFullScale(ISM_250dps);

	// Turn on the accelerometer's filter and apply settings.
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Turn on the gyroscope's filter and apply settings.
	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
}

void loop()
{
  float totalAccelX = 0; // Sum the accel readings
  float totalAccelY = 0;
  float totalAccelZ = 0;
  float countA = 0; // Count the readings
  float totalGyroX = 0; // Sum the gyro readings
  float totalGyroY = 0;
  float totalGyroZ = 0;
  float countG = 0; // Count the readings

  Serial.println("Recording accel and gyro data for 10 seconds...");
  Serial.println();
  delay(10); // Wait for the Serial.print to complete

  unsigned long startTime = millis(); // Keep track of time

  // Note: we can't do prints inside the while loop. They slow things down too much...
  
  while (millis() < (startTime + 10000)) // Loop for 10 seconds
  {
    // Check if accel data is available.
    if (myISM.checkAccelStatus())
    {
      myISM.getAccel(&accelData); // Read the accel data
      totalAccelX += accelData.xData; // Sum it
      totalAccelY += accelData.yData;
      totalAccelZ += accelData.zData;
      countA += 1; // Increment the count
    }

    // Check if gyro data is available.
    if (myISM.checkGyroStatus())
    {
      myISM.getGyro(&gyroData); // Read the gyro data
      totalGyroX += gyroData.xData; // Sum it
      totalGyroY += gyroData.yData;
      totalGyroZ += gyroData.zData;
      countG += 1; // Increment the count
    }
  }

  Serial.print("Read ");
  Serial.print(countA, 0);
  Serial.println(" Accel values.");
  Serial.println("Accel averages are:");
  Serial.println(totalAccelX / countA);
  Serial.println(totalAccelY / countA);
  Serial.println(totalAccelZ / countA);
  Serial.println();
  Serial.print("Read ");
  Serial.print(countG, 0);
  Serial.println(" Gyro values.");
  Serial.println("Gyro averages are:");
  Serial.println(totalGyroX / countG);
  Serial.println(totalGyroY / countG);
  Serial.println(totalGyroZ / countG);
  Serial.println();

  Serial.println("Final readings were:");
  Serial.print("Accel: ");
  Serial.print("X: ");
  Serial.print(accelData.xData);
  Serial.print(" ");
  Serial.print("Y: ");
  Serial.print(accelData.yData);
  Serial.print(" ");
  Serial.print("Z: ");
  Serial.print(accelData.zData);
  Serial.println(" ");
  Serial.print("Gyro: ");
  Serial.print("X: ");
  Serial.print(gyroData.xData);
  Serial.print(" ");
  Serial.print("Y: ");
  Serial.print(gyroData.yData);
  Serial.print(" ");
  Serial.print("Z: ");
  Serial.println(gyroData.zData);
  Serial.println();

  delay(1000);
}
