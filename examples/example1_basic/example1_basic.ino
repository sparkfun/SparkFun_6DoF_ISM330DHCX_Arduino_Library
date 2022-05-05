#include <Wire.h>
#include "sfe_ism330dhcx.h"


void setup(){

	Serial.begin(115200);
	Wire.begin();
	Serial.println("Hey.");
}

void loop(){
}

