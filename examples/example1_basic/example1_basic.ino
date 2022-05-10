#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM; 

void setup(){

	myISM.begin();

	Serial.begin(115200);
	Wire.begin();
	Serial.println("Hey.");


}

void loop(){
}

