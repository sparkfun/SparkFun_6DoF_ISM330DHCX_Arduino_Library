#pragma once
#include "sfe_ism330dhcx.h"
#include <Wire.h>

class SparkFun_ISM330DHCX : public QwDevISM330DHCX
{

	public: 

		SparkFun_ISM330DHCX() {};

    ///////////////////////////////////////////////////////////////////////
    // begin()
    //
    // This method is called to initialize the TMF882X library and connect to
    // the TMF882X device. This method must be called before calling any other method
    // that interacts with the device.
    //
    // This method follows the standard startup pattern in SparkFun Arduino
    // libraries.
    //
    //  Parameter   Description
    //  ---------   ----------------------------
    //  wirePort    optional. The Wire port. If not provided, the default port is used
    //  address     optional. I2C Address. If not provided, the default address is used.
    //  retval      true on success, false on startup failure
    //
    // This methond is overridden, implementing two versions.
    //
    // Version 1:
    // User passes in an aready setup wirePort object
    bool begin(TwoWire &wirePort, uint8_t deviceAddress = ISM330DHCX_ADDRESS_HIGH)
    {
        // Setup  I2C object and pass into the superclass
        _sfeBus.init(wirePort);
        setCommunicationBus(_sfeBus, deviceAddress);

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
    }

		bool begin(SPIClass& spiPort, SPISettings ismSPISettings, uint8_t cs);
    {
        // Setup  I2C object and pass into the superclass
        _sfeBus.init(spiPort, ismSPISettings, cs);
        setCommunicationBus(_sfeBus);

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
    }
    ///////////////////////////////////////////////////////////////////////
    // begin()
    //
    // Version 2:
    //
    // User doesn't provide a wireport object.
    bool begin(uint8_t deviceAddress =  ISM330DHCX_ADDRESS_HIGH)
    {
        _sfeBus.init(wirePort);
        setCommunicationBus(_i2cBus, deviceAddress);
        return this->QwDevISM330DHCX::init();
    }

	private: 
	
		SfeBus _sfeBus; 

};
