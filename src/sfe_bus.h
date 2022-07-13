// sfe_bus.cpp
//
// This is a library written for SparkFun Qwiic ISM330DHCX boards
//
// SparkFun sells these bpards at its website: www.sparkfun.com
//
// Do you like this library? Help support SparkFun. Buy a board!
//
//SparkFun Qwiic 6DoF - ISM330DHCX        https://www.sparkfun.com/products/19764
//
// Written by Kirk Benell @ SparkFun Electronics 
// Modified by Elias Santistevan @ SparkFun Electronics, April 2022
//
// Repository:
//     https://github.com/sparkfun/SparkFun_6DoF_ISM330DHCX_Arduino_Library
//
//
// SparkFun code, firmware, and software is released under the MIT
// License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2022 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Class provide an abstract interface to the I2C device



#pragma once

// Simple object to encapsulate basic I2C operations.
//
// This is following a pattern for future implementations
//
// This class is focused on Aurduino..

#include <Wire.h>
#include <SPI.h>


class SfeBus 
{
	public: 

		//virtual bool init(SPIClass& spiPort, SPISettings ismSPISettings, uint8_t cs, bool bInit=false) = 0;

		virtual bool init(TwoWire& wirePort = Wire, bool bInit=false) = 0;

		virtual bool ping(uint8_t address) = 0;

		virtual	bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data) = 0;

		virtual int writeRegisterRegion(uint8_t address, uint8_t offset, const uint8_t* data, uint16_t length) = 0;

		virtual int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes) = 0;

};

class QwI2C : public SfeBus
{
	public: 

		QwI2C(void);

		bool init(TwoWire& wirePort = Wire, bool bInit=false);

		bool ping(uint8_t address);

		bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data);

		int writeRegisterRegion(uint8_t address, uint8_t offset, const uint8_t* data, uint16_t length);

		int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes);

	private: 

    TwoWire* _i2cPort;
};

class SfeSPI : public SfeBus
{
	public:

		SfeSPI(void);

		bool init(SPIClass& spiPort, SPISettings ismSPISettings, uint8_t cs, bool bInit=false);

		bool ping(uint8_t address);

		bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data);

		int writeRegisterRegion(uint8_t address, uint8_t offset, const uint8_t* data, uint16_t length);

		int readRegisterRegion(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t numBytes);

	private:

		SPIClass* _spiPort; 
		SPISettings _sfeSPISettings; 
		uint8_t _cs; 
};

