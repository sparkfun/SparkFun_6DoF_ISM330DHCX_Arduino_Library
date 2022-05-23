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

sfe_raw_data_t QwDevISM330DHCX::getRawAccel()
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 ){

		sfeAccelData.xData = 0;
		sfeAccelData.yData = 0;
		sfeAccelData.zData = 0;
		return sfeAccelData;

	}

	sfeAccelData.xData = tempVal[0];
	sfeAccelData.yData = tempVal[1];
	sfeAccelData.zData = tempVal[2];

	return sfeAccelData;

}

sfe_raw_data_t QwDevISM330DHCX::getRawGyro()
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_angular_rate_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 ){

		sfeGyroData.xData = 0;
		sfeGyroData.yData = 0;
		sfeGyroData.zData = 0;
		return sfeGyroData;

	}

	sfeGyroData.xData = tempVal[0];
	sfeGyroData.yData = tempVal[1];
	sfeGyroData.zData = tempVal[2];

	return sfeGyroData;

}

void QwDevISM330DHCX::convert2gToMg(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs2g_to_mg(data[i]);
	}
}

void QwDevISM330DHCX::convert4gToMg(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs4g_to_mg(data[i]);
	}
}
void QwDevISM330DHCX::convert8gToMg(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs8g_to_mg(data[i]);
	}
}

void QwDevISM330DHCX::convert16gToMg(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs16g_to_mg(data[i]);
	}
}

void QwDevISM330DHCX::convert125dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs125dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convert250dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs250dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convert500dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs500dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convert1000dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs1000dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convert2000dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs2000dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convert4000dpsToMdps(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_fs4000dps_to_mdps(data[i]);
	}
}

void QwDevISM330DHCX::convertToCelsius(int16_t* data, uint8_t len)
{
	int i; 
	for( i = 0; i < len; i++ ) 
	{
		data[i] = ism330dhcx_from_lsb_to_celsius(data[i]);
	}
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


bool QwDevISM330DHCX::checkStatus()
{
	ism330dhcx_status_reg_t tempVal;
	int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

	if( retVal != 0)
		return false;

	if( (tempVal.xlda == 1) && (tempVal.gda == 1) )
		return true; 

	return false; 

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
