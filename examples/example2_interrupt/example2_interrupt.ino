/*
  example2-interrupt

  This example shows the basic settings and functions for retrieving accelerometer
	data. In addition we're setting the data ready signal to interrupt pin one in an
	active high configuration and show additional ways in which the interrupts
	can be configured. 

	Please refer to the header file for more possible settings, found here:
	..\SparkFun_6DoF_ISM330DHCX_Arduino_Library\src\sfe_ism330dhcx_defs.h

  Written by Elias Santistevan @ SparkFun Electronics, August 2022

	Product:

		6DoF: https://www.sparkfun.com/products/19764
		9DoF: https://www.sparkfun.com/products/19895

  Repository:

		https://github.com/sparkfun/SparkFun_6DoF_ISM330DHCX_Arduino_Library

  SparkFun code, firmware, and software is released under the MIT 
	License	(http://opensource.org/licenses/MIT).
*/

#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

// Structs for X,Y,Z data
SparkFun_ISM330DHCX myISM; 
sfe_ism_data_t accelData; 


// Interrupt pin
byte interrupt_pin = D1; 

void setup(){

	// Set the interrupt to INPUT
	pinMode(interrupt_pin, INPUT);
	Serial.begin(115200);

	Wire.begin();

	if( !myISM.begin() ){
		Serial.println("Did not begin.");
		while(1);
	}

	// Reset the device to default settings. This is helpful if you're doing multiple
	// uploads testing different settings. 
	myISM.deviceReset();

	// Wait for it to finish reseting
	while( !myISM.getDeviceReset() ){ 
		delay(1);
	} 

	Serial.println("Reset.");
	Serial.println("Applying settings.");
	delay(100);
	
	myISM.setDeviceConfig();
	myISM.setBlockDataUpdate();
	
	// Set the output data rate and precision of the accelerometer
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_4g); 

	// Turn on the accelerometer's filter and apply settings. 
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Set the accelerometer's status i.e. the data ready to interrupt one. 
	// Commented out just below is the function to send the data ready
	// to interrupt two. 

	myISM.setAccelStatustoInt1();
	//myISM.setAccelStatustoInt2();


	// We can just as easily set the gyroscope's data read signal to either interrupt

	//myISM.setGyroStatustoInt1();
	//myISM.setGyroStatustoInt2();


	// Uncommenting the function call below will change the interrupt to 
	// active LOW instead of HIGH.

	//myISM.setPinMode();

	// This function call will modify which "events" trigger an interrupt. No 
	// argument has been given, please refer to the datasheet for more 
	// information.

	// myISM.setIntNotification(uint8_t val);
}

void loop(){

	if( digitalRead(interrupt_pin) == HIGH ){
		myISM.getAccel(&accelData);
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
	}

	delay(100);
}

