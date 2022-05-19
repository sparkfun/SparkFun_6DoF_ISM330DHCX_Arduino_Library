#include "sfe_ism330dhcx.h"

//////////////////////////////////////////////////////////////////////////////
// init()
//
// init the system
//
// Return Value: false on error, true on success
//

bool QwDevISM330DHCX::init(void)
{
    //  do we have a bus yet? is the device connected?
    if (!_i2cBus || !_i2cAddress || !_i2cBus->ping(_i2cAddress))
        return false;
		
		// Setup the struct needed by the C source files for reading and writing.
		initCtx((void*)this, &sfe_dev); 			

		// I2C ready, now check that we're using the correct sensor before moving on. 
		if (getUniqueId() != ISM330DHCX_ID)
			return false; 
		

    return true;
}



///////////////////////////////////////////////////////////////////////
// isConnected()
//
// Called to determine if a ISM330DHCX device, at the provided i2c address
// is connected.
//
//  Parameter   Description
//  ---------   -----------------------------
//  retVal      true if device is connected, false if not connected

bool QwDevISM330DHCX::isConnected()
{
    return (_i2cBus && _i2cAddress ? _i2cBus->ping(_i2cAddress) : false);
}


////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Method to set the bus object that is used to communicate with the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  idBus        The id/address of the device on the bus

void QwDevISM330DHCX::setCommunicationBus(QwI2C &theBus, uint8_t idBus)
{
    _i2cBus = &theBus;
    _i2cAddress = idBus;
}

//////////////////////////////////////////////////////////////////////////////
//
//
int32_t QwDevISM330DHCX::writeRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _i2cBus->writeRegisterRegion(_i2cAddress, offset, data, length);
}

int32_t QwDevISM330DHCX::readRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _i2cBus->readRegisterRegion(_i2cAddress, offset, data, length);
}

bool QwDevISM330DHCX::setAccelFullScale(uint8_t val)
{
	if( val < 0 | val > 3 )
		return false;
	
	int32_t retVal = (ism330dhcx_xl_full_scale_set(&sfe_dev, 
																								(ism330dhcx_fs_xl_t)val));
	
	if( retVal != 0 )
		return false; 

	return true; 
}

uint8_t QwDevISM330DHCX::getAccelFullScale()
{

	ism330dhcx_fs_xl_t val; 
	int32_t retVal = ism330dhcx_xl_full_scale_get(&sfe_dev, &val);

	if( retVal != 0 )
		return -1; 
	
	return (uint8_t)val; 
}

uint8_t QwDevISM330DHCX::getUniqueId()
{

	uint8_t buff = 0;
	int32_t retVal = (ism330dhcx_device_id_get(&sfe_dev, &buff));

	if(retVal != 0)
		return 0; 
	
	return buff;
}


int16_t QwDevISM330DHCX::getTemp()
{
	
	int16_t tempVal;	
	int32_t retVal = ism330dhcx_temperature_raw_get(&sfe_dev, &tempVal);

	if( retVal != 0 )
		return -1;

	return tempVal;

}

sfe_accel_data_t QwDevISM330DHCX::getAccel()
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 ){

		sfe_ism330dhcx_accelData.xAccelData = 0;
		sfe_ism330dhcx_accelData.yAccelData = 0;
		sfe_ism330dhcx_accelData.zAccelData = 0;
		return sfe_ism330dhcx_accelData;

	}

	sfe_ism330dhcx_accelData.xAccelData = tempVal[0];
	sfe_ism330dhcx_accelData.yAccelData = tempVal[1];
	sfe_ism330dhcx_accelData.zAccelData = tempVal[2];

	return sfe_ism330dhcx_accelData;

}



bool QwDevISM330DHCX::setAccelDataRate(uint8_t rate)
{
	if( rate < 0 || rate > 11 )
		return false; 

	int32_t retVal = ism330dhcx_xl_data_rate_set(&sfe_dev, (ism330dhcx_odr_xl_t)rate);

	if( retVal != 0)
		return false;

	return true; 
}

bool QwDevISM330DHCX::setGyroDataRate(uint8_t rate)
{
	if( rate < 0 || rate > 10 )
		return false;

	int32_t retVal = ism330dhcx_gy_data_rate_set(&sfe_dev,(ism330dhcx_odr_g_t)rate);

	if( retVal != 0 )
		return false;

	return true; 
}


bool QwDevISM330DHCX::setAccelStatustoInt()
{

	ism330dhcx_pin_int1_route_t intToPin = {1}; 
	
	int32_t retVal = ism330dhcx_pin_int1_route_set(&sfe_dev, &intToPin);

	if( retVal != 0 )
		return false;

	return true; 
}


bool QwDevISM330DHCX::checkAccelStatus()
{
	ism330dhcx_status_reg_t tempVal;
	int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

	if( retVal != 0)
		return false;

	if( tempVal.xlda == 1 )
		return true; 

	return false; 

}

bool QwDevISM330DHCX::checkGyroStatus()
{
	ism330dhcx_status_reg_t tempVal;
	int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

	if( retVal != 0)
		return false;

	if( tempVal.gda == 1 )
		return true; 

	return false; 

}

bool QwDevISM330DHCX::checkTempStatus()
{
	ism330dhcx_status_reg_t tempVal;
	int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

	if( retVal != 0)
		return false;

	if( tempVal.tda == 1 )
		return true; 

	return false; 

}
