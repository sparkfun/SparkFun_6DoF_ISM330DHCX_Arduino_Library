#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 

void setup(){

	Wire.begin();

	Serial.begin(115200);
	Serial.println("Hey.");

	Serial.print("Did we begin: "); 
	Wire.beginTransmission(0x6B);
	Serial.println(Wire.endTransmission());
	Serial.println(myISM.begin());


	Serial.print("Unique ID: ");
	Serial.println(myISM.getUniqueId(), HEX);


}

void loop(){
}

