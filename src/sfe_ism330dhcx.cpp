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
	//0 = 2g, 1 = 16g, 2 = 4g, 3 = 8g
	if( val > 3 )
		return false;
	
	int32_t retVal = (ism330dhcx_xl_full_scale_set(&sfe_dev, 
																								(ism330dhcx_fs_xl_t)val));
	
	fullScaleAccel = val; 

	if( retVal != 0 )
		return false; 

	return true; 
}

bool QwDevISM330DHCX::setGyroFullScale(uint8_t val)
{
	//0,1,2,4,8,12
	if( val > 12 )
		return false;
	
	int32_t retVal = ism330dhcx_gy_full_scale_set(&sfe_dev,
																							  (ism330dhcx_fs_g_t)val);
	
	fullScaleGyro = val; 

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

bool QwDevISM330DHCX::getRawAccel(sfe_ism_raw_data_t* accelData)
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 )
		return false;

	accelData->xData = tempVal[0];
	accelData->yData = tempVal[1];
	accelData->zData = tempVal[2];

	return true;

}

bool QwDevISM330DHCX::getRawGyro(sfe_ism_raw_data_t* gyroData)
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_angular_rate_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 )
		return false;


	gyroData->xData = tempVal[0];
	gyroData->yData = tempVal[1];
	gyroData->zData = tempVal[2];

	return true;

}

bool QwDevISM330DHCX::getAccel(sfe_ism_data_t* accelData)
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 )
		return false;
	
	// Private variable that keeps track of the users settings
	// so that the register values can be converted accordingly
	switch( fullScaleAccel ){
		case 0:
			accelData->xData = convert2gToMg(tempVal[0]);
			accelData->yData = convert2gToMg(tempVal[1]);
			accelData->zData = convert2gToMg(tempVal[2]);
			break;
		case 1:
			accelData->xData = convert16gToMg(tempVal[0]);
			accelData->yData = convert16gToMg(tempVal[1]);
			accelData->zData = convert16gToMg(tempVal[2]);
			break;
		case 2:
			accelData->xData = convert4gToMg(tempVal[0]);
			accelData->yData = convert4gToMg(tempVal[1]);
			accelData->zData = convert4gToMg(tempVal[2]);
			break;
		case 3:
			accelData->xData = convert8gToMg(tempVal[0]);
			accelData->yData = convert8gToMg(tempVal[1]);
			accelData->zData = convert8gToMg(tempVal[2]);
			break;
		default:
			return false; //Something has gone wrong
	}

	return true;
}


bool QwDevISM330DHCX::getGyro(sfe_ism_data_t* gyroData)
{
	
	int16_t tempVal[3] = {0};	
	int32_t retVal = ism330dhcx_angular_rate_raw_get(&sfe_dev, tempVal);

	if( retVal != 0 )
		return false;

	// Private variable that keeps track of the users settings
	// so that the register values can be converted accordingly
	switch( fullScaleGyro ){
		case 0:
			gyroData->xData = convert250dpsToMdps(tempVal[0]);
			gyroData->yData = convert250dpsToMdps(tempVal[1]);
			gyroData->zData = convert250dpsToMdps(tempVal[2]);
			break;
		case 1:
			gyroData->xData = convert4000dpsToMdps(tempVal[0]);
			gyroData->yData = convert4000dpsToMdps(tempVal[1]);
			gyroData->zData = convert4000dpsToMdps(tempVal[2]);
			break;
		case 2:
			gyroData->xData = convert125dpsToMdps(tempVal[0]);
			gyroData->yData = convert125dpsToMdps(tempVal[1]);
			gyroData->zData = convert125dpsToMdps(tempVal[2]);
			break;
		case 4:
			gyroData->xData = convert500dpsToMdps(tempVal[0]);
			gyroData->yData = convert500dpsToMdps(tempVal[1]);
			gyroData->zData = convert500dpsToMdps(tempVal[2]);
			break;
		case 8:
			gyroData->xData = convert1000dpsToMdps(tempVal[0]);
			gyroData->yData = convert1000dpsToMdps(tempVal[1]);
			gyroData->zData = convert1000dpsToMdps(tempVal[2]);
			break;
		case 12:
			gyroData->xData = convert2000dpsToMdps(tempVal[0]);
			gyroData->yData = convert2000dpsToMdps(tempVal[1]);
			gyroData->zData = convert2000dpsToMdps(tempVal[2]);
			break;
		default:
			return false; //Something has gone wrong
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////
// Conversions
//
// 
//
//
//  
// 
// 
//

float QwDevISM330DHCX::convert2gToMg(int16_t data)
{
	return(ism330dhcx_from_fs2g_to_mg(data));
}

float QwDevISM330DHCX::convert4gToMg(int16_t data)
{
	return(ism330dhcx_from_fs4g_to_mg(data));
}

float QwDevISM330DHCX::convert8gToMg(int16_t data)
{
	return(ism330dhcx_from_fs8g_to_mg(data));
}

float QwDevISM330DHCX::convert16gToMg(int16_t data)
{
	return(ism330dhcx_from_fs16g_to_mg(data));
}

float QwDevISM330DHCX::convert125dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs125dps_to_mdps(data));
}

float QwDevISM330DHCX::convert250dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs250dps_to_mdps(data));
}

float QwDevISM330DHCX::convert500dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs500dps_to_mdps(data));
}

float QwDevISM330DHCX::convert1000dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs1000dps_to_mdps(data));
}

float QwDevISM330DHCX::convert2000dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs2000dps_to_mdps(data));
}

float QwDevISM330DHCX::convert4000dpsToMdps(int16_t data)
{
	return(ism330dhcx_from_fs4000dps_to_mdps(data));
}

float QwDevISM330DHCX::convertToCelsius(int16_t data)
{
	return(ism330dhcx_from_lsb_to_celsius(data));
}

//
//
//////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////
// General Settings
//
// 
//
//
//  
// 
// 
//

bool QwDevISM330DHCX::setDeviceConfig(bool enable)
{
	int32_t retVal;

	retVal = ism330dhcx_device_conf_set(&sfe_dev, (uint8_t)enable);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::deviceReset()
{
	int32_t retVal;

	retVal = ism330dhcx_reset_set(&sfe_dev, 1);

	if( retVal != 0 )
		return false;

	return true;
}

uint8_t QwDevISM330DHCX::getDeviceReset()
{

	int32_t retVal;
	uint8_t tempVal;
	retVal = ism330dhcx_reset_get(&sfe_dev, &tempVal);

	if( retVal != 0 )
		return 0;

	return tempVal;
}

bool QwDevISM330DHCX::setSlopeFilter(uint8_t val)
{
	int32_t retVal;
	if( val > 7 )
		return false;

	retVal = ism330dhcx_xl_hp_path_on_out_set(&sfe_dev,
                                         (ism330dhcx_hp_slope_xl_en_t)val);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setAccelFilterLP2(bool enable)
{
	int32_t retVal;

	retVal = ism330dhcx_xl_filter_lp2_set(&sfe_dev, (uint8_t)enable);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setBlockDataUpdate(bool set)
{

	int32_t retVal = ism330dhcx_block_data_update_set(&sfe_dev, (uint8_t)set);

	if( retVal != 0 )
		return false;

	return true; 
}

uint8_t QwDevISM330DHCX::getBlockDataUpdate()
{

	uint8_t tempVal;
	int32_t retVal = ism330dhcx_block_data_update_get(&sfe_dev, &tempVal);

	return tempVal; 
}

bool QwDevISM330DHCX::setAccelDataRate(uint8_t rate)
{
	if( rate > 11 )
		return false; 

	int32_t retVal = ism330dhcx_xl_data_rate_set(&sfe_dev, (ism330dhcx_odr_xl_t)rate);

	if( retVal != 0)
		return false;

	return true; 
}

bool QwDevISM330DHCX::setGyroDataRate(uint8_t rate)
{
	if( rate > 10 )
		return false;

	int32_t retVal = ism330dhcx_gy_data_rate_set(&sfe_dev,(ism330dhcx_odr_g_t)rate);

	if( retVal != 0 )
		return false;

	return true; 
}



bool QwDevISM330DHCX::enableTimestamp(bool enable)
{
	int32_t retVal;

	retVal = ism330dhcx_timestamp_set(&sfe_dev, (uint8_t)enable);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::resetTimestamp()
{
	int32_t retVal; 

	retVal = ism330dhcx_timestamp_rst(&sfe_dev);

	if( retVal != 0 )
		return false;

	return true;
}
//
//
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// FIFO Settings
//
// 
//
//
//  
// 

bool QwDevISM330DHCX::setFifoWatermark(uint16_t val)
{
	int32_t retVal;

	retVal = ism330dhcx_fifo_watermark_set(&sfe_dev, val);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setFifoMode(uint8_t val)
{
	int32_t retVal;

	if( val > 7 )
		return false;

	retVal = ism330dhcx_fifo_mode_set(&sfe_dev,
                                 (ism330dhcx_fifo_mode_t)val);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setAccelFifoBatchSet(uint8_t val)
{
	int32_t retVal;
	if( val > 11)
		return false;

	retVal = ism330dhcx_fifo_xl_batch_set(&sfe_dev,
                                     (ism330dhcx_bdr_xl_t)val);

	if( retVal != 0 )
		return false;

	return true;
}


bool QwDevISM330DHCX::setGyroFifoBatchSet(uint8_t val)
{
	int32_t retVal;
	if( val > 11)
		return false;

	retVal = ism330dhcx_fifo_gy_batch_set(&sfe_dev,
                                     (ism330dhcx_bdr_gy_t)val);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setFifoTimestampDec(uint8_t val)
{
	int32_t retVal;
	if( val > 3 )
		return false;

	retVal = ism330dhcx_fifo_timestamp_decimation_set(&sfe_dev,
                                                   (ism330dhcx_odr_ts_batch_t)val);

	if( retVal != 0 )
		return false;

	return true;
}

//
//
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Interrupt Settings
//
// 
//
//
//  
// 
bool QwDevISM330DHCX::setIntNotification(uint8_t val)
{
	int32_t retVal;
	if( val > 4)
		return false;

	retVal = ism330dhcx_int_notification_set(&sfe_dev,
                                        (ism330dhcx_lir_t)val);

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

//
//
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Sensor Hub Settings
//
// 
//
//
//  
// 
// 

bool QwDevISM330DHCX::setHubODR(uint8_t rate)
{
	//0 = 104Hz, 1 = 52Hz, 2 = 26Hz, 3 = 13Hz
	if( rate > 3 )
		return false;

	int32_t retVal = ism330dhcx_sh_data_rate_set(&sfe_dev, (ism330dhcx_shub_odr_t)rate);

	if( retVal != 0 )
		return false;

	return true; 
}

bool QwDevISM330DHCX::setHubSensor(uint8_t sensor, sfe_hub_sensor_settings_t* settings)
{
	int32_t retVal;
	ism330dhcx_sh_cfg_read_t tempSett;

	if( sensor > 3 )
		return false;

	tempSett.slv_add = settings->address; 
	tempSett.slv_subadd = settings->subAddress; 
	tempSett.slv_len = settings->length; 

	switch( sensor )
	{
		case 0:
			retVal = ism330dhcx_sh_slv0_cfg_read(&sfe_dev, &tempSett);
			break;
		case 1:
			retVal = ism330dhcx_sh_slv1_cfg_read(&sfe_dev, &tempSett);
			break;
		case 2:
			retVal = ism330dhcx_sh_slv2_cfg_read(&sfe_dev, &tempSett);
			break;
		case 3:
			retVal = ism330dhcx_sh_slv3_cfg_read(&sfe_dev, &tempSett);
			break;
		default:
			return false;
	}


	if( retVal != 0 )
		return false;

	return true; 
}

bool QwDevISM330DHCX::setNumberHubSensors(uint8_t numSensors)
{

	int32_t retVal;
	if( numSensors > 3 ) 
		return false; 

	retVal = ism330dhcx_sh_slave_connected_set(&sfe_dev, 
																					 (ism330dhcx_aux_sens_on_t)numSensors);

	if( retVal != 0 )
		return false;

	return true; 

}

bool QwDevISM330DHCX::enableSensorI2C(bool enable)
{
	int32_t retVal;

	retVal = ism330dhcx_sh_master_set(&sfe_dev, (uint8_t)enable);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::readPeripheralSensor(uint8_t* shReg, uint8_t len)
{
	int32_t retVal;

	retVal = ism330dhcx_sh_read_data_raw_get(&sfe_dev, (ism330dhcx_emb_sh_read_t*)shReg, len);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::readMMCMagnetometer(uint8_t* magData, uint8_t len)
{
	int32_t retVal;

	readPeripheralSensor(magData, len); 

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setHubWriteMode(uint8_t config)
{
	int32_t retVal;

	if( config > 1)
		return false;

	// 0 = Write each cycle 
	// 1 = Write once
	retVal = ism330dhcx_sh_write_mode_set(&sfe_dev,
																			 (ism330dhcx_write_once_t)config);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setHubPassThrough(bool set)
{

	int32_t retVal;
	ism330dhcx_sh_pass_through_set(&sfe_dev, (uint8_t)set);

	if( retVal != 0 );
		return false;

	return true; 
}

bool QwDevISM330DHCX::setHubFifoBatching(bool enable)
{
	int32_t retVal;

	retVal = ism330dhcx_sh_batch_slave_0_set(&sfe_dev, (uint8_t)enable);

	if( retVal != 0 )
		return false;

	return true;
}
//
//
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Self Test
//
// 
//
//
//  
// 
// 

bool QwDevISM330DHCX::setAccelSelfTest(uint8_t val)
{
	int32_t retVal;

	if( val > 2)
		return false;

	retVal = ism330dhcx_xl_self_test_set(&sfe_dev,
                                    (ism330dhcx_st_xl_t)val);

	if( retVal != 0 )
		return false;

	return true;
}

bool QwDevISM330DHCX::setGyroSelfTest(uint8_t val)
{
	int32_t retVal;

	// 0, 1, or 3
	if( val > 3)
		return false;

	retVal = ism330dhcx_gy_self_test_set(&sfe_dev,
                                    (ism330dhcx_st_g_t)val);

	if( retVal != 0 )
		return false;

	return true;
}
//
//
//////////////////////////////////////////////////////////////////////////////////
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
