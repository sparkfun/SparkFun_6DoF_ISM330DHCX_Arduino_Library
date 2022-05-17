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

		// I2C ready, now check that we're using the correct sensor before moving on. 
		if (getUniqueId() != ISM330DHCX_ID)
			return false; 
		
		
		// Initialize the 
		initCtx((void*)this, &sfe_dev); 			
		

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
//  retval      true if device is connected, false if not connected

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

	if(retVal == 0)
		return buff; 
	
	return 0;
}

