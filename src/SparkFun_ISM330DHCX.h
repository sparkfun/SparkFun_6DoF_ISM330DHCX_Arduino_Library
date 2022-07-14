#pragma once
#include "sfe_ism330dhcx.h"
#include "sfe_bus.h"
#include <Wire.h>
#include <SPI.h>

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
		bool begin(uint8_t deviceAddress = ISM330DHCX_ADDRESS_HIGH)
		{
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_i2cBus, deviceAddress);
        _i2cBus.init();

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
		}

    bool begin(TwoWire &wirePort, uint8_t deviceAddress = ISM330DHCX_ADDRESS_HIGH)
    {
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_i2cBus, deviceAddress);
        _i2cBus.init(wirePort, true);

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
    }

	private: 

		QwI2C _i2cBus; 

};
	
class SparkFun_ISM330DHCX_SPI : public QwDevISM330DHCX
{
		public:

		SparkFun_ISM330DHCX_SPI() {};

    bool begin(uint8_t cs)
    {
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_spiBus);
        _spiBus.init(cs, true);

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
    }

    bool begin(SPIClass &spiPort, SPISettings ismSettings, uint8_t cs)
    {
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_spiBus);
        _spiBus.init(spiPort, ismSettings, cs, true);

        // Initialize the system - return results
        return this->QwDevISM330DHCX::init();
    }

		private:

		SfeSPI _spiBus;


};
