#pragma once
#include "sfe_ism330dhcx.h"
#include "sfe_bus.h"
#include <Wire.h>
#include <SPI.h>

class SparkFun_ISM330DHCX : public QwDevISM330DHCX
{
public:

	SparkFun_ISM330DHCX() {};
	/**
	 * @brief      This method is called to initialize the ISM330DHCX library
	 *             and connect to the ISM330DHCX device. This method must be
	 *             called before calling any other method that interacts with
	 *             the device.
	 *
	 *             Method 1: User skips passing in an I2C object which then
	 *             defaults to Wire.
	 *
	 *             This method follows the standard startup pattern in SparkFun
	 *             Arduino libraries.
	 *
	 * @param[in]  deviceAddress  I2C Address. If not provided, the default
	 *                            address is used.
	 *
	 * @return     true on success, false on startup failure
	 */
	bool begin(uint8_t deviceAddress = ISM330DHCX_ADDRESS_HIGH)
	{
			// Setup  I2C object and pass into the superclass
			setCommunicationBus(_i2cBus, deviceAddress);

			// Initialize the I2C buss class i.e. setup default Wire port
			_i2cBus.init();

			// Initialize the system - return results
			return this->QwDevISM330DHCX::init();
	}

	/**
	 * @brief      This method is called to initialize the ISM330DHCX library
	 *             and connect to the ISM330DHCX device. This method must be
	 *             called before calling any other method that interacts with
	 *             the device.
	 *
	 *             Method 2:  User passes in an I2C object and an address
	 *             (optional).
	 *
	 *             This method follows the standard startup pattern in SparkFun
	 *             Arduino libraries.
	 *
	 * @param      wirePort       The wire port
	 * @param[in]  deviceAddress  I2C Address. If not provided, the default
	 *                            address is used.
	 *
	 * @return     true on success, false on startup failure
	 */
	bool begin(TwoWire &wirePort, uint8_t deviceAddress = ISM330DHCX_ADDRESS_HIGH)
	{
			// Setup  I2C object and pass into the superclass
			setCommunicationBus(_i2cBus, deviceAddress);

			// Give the I2C port provided by the user to the I2C bus class.
			_i2cBus.init(wirePort, true);

			// Initialize the system - return results
			return this->QwDevISM330DHCX::init();
	}

private:

	//I2C bus class
	sfe_ISM330DHCX::QwI2C _i2cBus;

};

class SparkFun_ISM330DHCX_SPI : public QwDevISM330DHCX
{
public:

	SparkFun_ISM330DHCX_SPI() {};


	/**
	 * @brief      This method is called to initialize the ISM330DHCX library
	 *             and connect to the ISM330DHCX device. This method must be
	 *             called before calling any other method that interacts with
	 *             the device.
	 *
	 * @param[in]  cs    Pin number connected to the cs pin
	 *
	 * @return    true on success, false on startup failure
	 */
	bool begin(uint8_t cs)
	{
			// Setup a SPI object and pass into the superclass
			setCommunicationBus(_spiBus);

			// Initialize the SPI bus class with the chip select pin, SPI port
			// defaults to SPI, and SPI settings are set to class defaults.
			_spiBus.init(cs, true);

			// Initialize the system - return results
			return this->QwDevISM330DHCX::init();
	}


	/**
	 * @brief      This method is called to initialize the ISM330DHCX library
	 *             and connect to the ISM330DHCX device. This method must be
	 *             called before calling any other method that interacts with
	 *             the device.
	 *
	 * @param      spiPort      The SPI port.
	 * @param[in]  ismSettings  SPI "transaction" settings are need for every
	 *                          data transfer.
	 * @param[in]  cs           Pin number connected to the cs pin
	 *
	 * @return     true on success, false on startup failure
	 */
	bool begin(SPIClass &spiPort, const SPISettings& ismSettings, uint8_t cs)
	{
			// Setup a SPI object and pass into the superclass
			setCommunicationBus(_spiBus);

			// Initialize the SPI bus class with provided SPI port, SPI settings,
			// and chip select pin.
			_spiBus.init(spiPort, ismSettings, cs, true);

			// Initialize the system - return results
			return this->QwDevISM330DHCX::init();
	}

private:

	// SPI bus class
	sfe_ISM330DHCX::SfeSPI _spiBus;

};
