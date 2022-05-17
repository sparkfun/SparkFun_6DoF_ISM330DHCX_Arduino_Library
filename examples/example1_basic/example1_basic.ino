#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 

void setup(){

	Wire.begin();

	Serial.begin(115200);
	Serial.println("Hey.");

	Serial.print("Did we begin: "); 
	Serial.println(myISM.begin());
}

void loop(){
}

