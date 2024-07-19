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
    if (!_sfeBus->ping(_i2cAddress))
        return false;

    initCtx((void *)this, &sfe_dev);

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
    if (getUniqueId() != ISM330DHCX_ID)
        return false;
    else
        return true;
}

////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Method to set the bus object that is used to communicate with the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  i2cAddress   I2C address for the 6DoF

void QwDevISM330DHCX::setCommunicationBus(sfe_ISM330DHCX::QwIDeviceBus &theBus, uint8_t i2cAddress)
{
    _sfeBus = &theBus;
    _i2cAddress = i2cAddress;
}

////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Overloaded option for setting the data bus (theBus) object to a SPI bus object.
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//

void QwDevISM330DHCX::setCommunicationBus(sfe_ISM330DHCX::QwIDeviceBus &theBus)
{
    _sfeBus = &theBus;
}

//////////////////////////////////////////////////////////////////////////////
// writeRegisterRegion()
//
// Writes data to the given register
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be written to
//  data         Data to be written
//  length       Number of bytes to be written

int32_t QwDevISM330DHCX::writeRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->writeRegisterRegion(_i2cAddress, offset, data, length);
}

//////////////////////////////////////////////////////////////////////////////
// readRegisterRegion()
//
// Reads data from the given register, auto-incrementing fore each successive read
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be read from
//  data         Pointer to where data will be saved
//  length       Number of bytes to be read

int32_t QwDevISM330DHCX::readRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->readRegisterRegion(_i2cAddress, offset, data, length);
}

//////////////////////////////////////////////////////////////////////////////
// setAccelFullScale()
//
// Sets the scale of the acceleromter's readings 2g - 16g
//
//  Parameter    Description
//  ---------    -----------------------------
//  val          The scale to be applied to the accelerometer (0 - 3)
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//

bool QwDevISM330DHCX::setAccelFullScale(uint8_t val)
{
    // 0 = 2g, 1 = 16g, 2 = 4g, 3 = 8g
    if (val > 3)
        return false;

    int32_t retVal = (ism330dhcx_xl_full_scale_set(&sfe_dev, (ism330dhcx_fs_xl_t)val));

    fullScaleAccel = val;

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// setGyroFullScale()
//
// Sets the scale of the gyroscopes's readings 125, 250, 500, 1000, 2000, 4000 degrees per second
//
//  Parameter    Description
//  ---------    -----------------------------
//  val          The scale to be applied to the gyroscope (0,1,2,4,6,12)
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//
bool QwDevISM330DHCX::setGyroFullScale(uint8_t val)
{
    // 0,1,2,4,8,12
    if (val > 12)
        return false;

    int32_t retVal = ism330dhcx_gy_full_scale_set(&sfe_dev, (ism330dhcx_fs_g_t)val);

    fullScaleGyro = val;

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// getAccelFullScale()
//
// Retrieves the scale of the accelerometer's readings
//

uint8_t QwDevISM330DHCX::getAccelFullScale()
{

    ism330dhcx_fs_xl_t val;
    int32_t retVal = ism330dhcx_xl_full_scale_get(&sfe_dev, &val);

    if (retVal != 0)
        return -1;

    return (uint8_t)val;
}

//////////////////////////////////////////////////////////////////////////////
// getGyroFullScale()
//
// Retrieves the scale of the gyroscope's readings
//

uint8_t QwDevISM330DHCX::getGyroFullScale()
{
    ism330dhcx_fs_g_t val;
    int32_t retVal = ism330dhcx_gy_full_scale_get(&sfe_dev, &val);

    if (retVal != 0)
        return -1;

    return (uint8_t)val;
}

//////////////////////////////////////////////////////////////////////////////
// getUniqueId()
//
// Retrieves the the device's ID: 0x6B for the ISM330DHCX
//

uint8_t QwDevISM330DHCX::getUniqueId()
{

    uint8_t buff = 0;
    int32_t retVal = (ism330dhcx_device_id_get(&sfe_dev, &buff));

    if (retVal != 0)
        return 0;

    return buff;
}

//////////////////////////////////////////////////////////////////////////////
// getTemp()
//
// Gets the temperature
//

int16_t QwDevISM330DHCX::getTemp()
{

    int16_t tempVal;
    int32_t retVal = ism330dhcx_temperature_raw_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return -1;

    return tempVal;
}

//////////////////////////////////////////////////////////////////////////////
// getRawAccel()
//
// Retrieves raw register values for accelerometer data
//
//  Parameter    Description
//  ---------    -----------------------------
//  accelData    Accel data type pointer at which data will be stored.

bool QwDevISM330DHCX::getRawAccel(sfe_ism_raw_data_t *accelData)
{
    int16_t tempVal[3] = {0};
    int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

    if (retVal != 0)
        return false;

    accelData->xData = tempVal[0];
    accelData->yData = tempVal[1];
    accelData->zData = tempVal[2];

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// getRawGyro()
//
// Retrieves raw register values for gyroscope data
//
//  Parameter    Description
//  ---------   -----------------------------
//  gyroData    Gyro data type pointer at which data will be stored.
//

bool QwDevISM330DHCX::getRawGyro(sfe_ism_raw_data_t *gyroData)
{

    int16_t tempVal[3] = {0};
    int32_t retVal = ism330dhcx_angular_rate_raw_get(&sfe_dev, tempVal);

    if (retVal != 0)
        return false;

    gyroData->xData = tempVal[0];
    gyroData->yData = tempVal[1];
    gyroData->zData = tempVal[2];

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// getAccel()
//
// Retrieves raw register values and converts them according to the full scale settings
//
//  Parameter    Description
//  ---------   -----------------------------
//  accelData    Accel data type pointer at which data will be stored.
//

bool QwDevISM330DHCX::getAccel(sfe_ism_data_t *accelData)
{

    int16_t tempVal[3] = {0};
    int32_t retVal = ism330dhcx_acceleration_raw_get(&sfe_dev, tempVal);

    if (retVal != 0)
        return false;

    // "fullScaleAccel" is a private variable that keeps track of the users settings
    // so that the register values can be converted accordingly
    switch (fullScaleAccel)
    {
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
        return false; // Something has gone wrong
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// getGyro()
//
// Retrieves raw register values and converts them according to the full scale settings
//
//  Parameter    Description
//  ---------   -----------------------------
//  gyroData    Gyroscope data type pointer at which data will be stored.
//

bool QwDevISM330DHCX::getGyro(sfe_ism_data_t *gyroData)
{

    int16_t tempVal[3] = {0};
    int32_t retVal = ism330dhcx_angular_rate_raw_get(&sfe_dev, tempVal);

    if (retVal != 0)
        return false;

    // "fullScaleGyro" is a private variable that keeps track of the users settings
    // so that the register values can be converted accordingly
    switch (fullScaleGyro)
    {
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
        return false; // Something has gone wrong
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Conversions Methods
//
//

float QwDevISM330DHCX::convert2gToMg(int16_t data)
{
    return (ism330dhcx_from_fs2g_to_mg(data));
}

float QwDevISM330DHCX::convert4gToMg(int16_t data)
{
    return (ism330dhcx_from_fs4g_to_mg(data));
}

float QwDevISM330DHCX::convert8gToMg(int16_t data)
{
    return (ism330dhcx_from_fs8g_to_mg(data));
}

float QwDevISM330DHCX::convert16gToMg(int16_t data)
{
    return (ism330dhcx_from_fs16g_to_mg(data));
}

float QwDevISM330DHCX::convert125dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs125dps_to_mdps(data));
}

float QwDevISM330DHCX::convert250dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs250dps_to_mdps(data));
}

float QwDevISM330DHCX::convert500dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs500dps_to_mdps(data));
}

float QwDevISM330DHCX::convert1000dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs1000dps_to_mdps(data));
}

float QwDevISM330DHCX::convert2000dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs2000dps_to_mdps(data));
}

float QwDevISM330DHCX::convert4000dpsToMdps(int16_t data)
{
    return (ism330dhcx_from_fs4000dps_to_mdps(data));
}

float QwDevISM330DHCX::convertToCelsius(int16_t data)
{
    return (ism330dhcx_from_lsb_to_celsius(data));
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

//////////////////////////////////////////////////////////////////////////////////
// setDeviceConfig()
//
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable      Enable the general device configuration - this is a configuration
//							register in the ISM330DHCX, not a library implementation.

bool QwDevISM330DHCX::setDeviceConfig(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_device_conf_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// deviceReset()
//
// Resets the deivice to default settings
//

bool QwDevISM330DHCX::deviceReset()
{
    int32_t retVal;

    retVal = ism330dhcx_reset_set(&sfe_dev, 1);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// getDeviceReset()
//
// Checks to see that the reset bit has been set
//

bool QwDevISM330DHCX::getDeviceReset()
{

    int32_t retVal;
    uint8_t tempVal;
    retVal = ism330dhcx_reset_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if ((tempVal & 0x01) == 0x00)
    {
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelSlopeFilter()
//
// Sets the accelerometer's slope filter
//
//  Parameter   Description
//  ---------   -----------------------------
//  val         This parameter determines the intensity of the filter - (0x00 - 0x37)
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setAccelSlopeFilter(uint8_t val)
{
    int32_t retVal;
    if (val > 0x37)
        return false;

    retVal = ism330dhcx_xl_hp_path_on_out_set(&sfe_dev, (ism330dhcx_hp_slope_xl_en_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelFilterLP2()
//
// Enables the accelerometer's high resolution slope filter
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable      Enables/Disables the high resolution slope filter
//

bool QwDevISM330DHCX::setAccelFilterLP2(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_xl_filter_lp2_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroFilterLP1()
//
// Enables the gyroscope's slope filter
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable      Enables/Disables the high resolution slope filter
//

bool QwDevISM330DHCX::setGyroFilterLP1(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_gy_filter_lp1_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroLP1Bandwidth()
//
// Sets the low pass filter's bandwidth for the gyroscope
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Sets the bandwidth's value
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//

bool QwDevISM330DHCX::setGyroLP1Bandwidth(uint8_t val)
{
    int32_t retVal;
    if (val > 7)
        return false;

    retVal = ism330dhcx_gy_lp1_bandwidth_set(&sfe_dev, (ism330dhcx_ftype_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setBlockDataUpdate()
//
// Data is not updated until both MSB and LSB have been read from output registers
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable			Enable/disables block data update.
//

bool QwDevISM330DHCX::setBlockDataUpdate(bool enable)
{

    int32_t retVal = ism330dhcx_block_data_update_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// getBlockDataUpdate()
//
// Retrieves the bit indicating whether block data update is enabled.
//
//

uint8_t QwDevISM330DHCX::getBlockDataUpdate()
{

    uint8_t tempVal;
    int32_t retVal = ism330dhcx_block_data_update_get(&sfe_dev, &tempVal);

    (void)retVal;
    return tempVal;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelDataRate()
//
// This sets the data output rate for the accelerometer.
//
//  Parameter   Description
//  ---------   -----------------------------
//  rate        Sets the output data rate, expects some value < 11.
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setAccelDataRate(uint8_t rate)
{
    if (rate > 11)
        return false;

    int32_t retVal = ism330dhcx_xl_data_rate_set(&sfe_dev, (ism330dhcx_odr_xl_t)rate);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroDataRate()
//
// This sets the data output rate for the gyroscope.
//
//  Parameter   Description
//  ---------   -----------------------------
//  rate        Sets the output data rate, expects some value < 10.
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//

bool QwDevISM330DHCX::setGyroDataRate(uint8_t rate)
{
    if (rate > 10)
        return false;

    int32_t retVal = ism330dhcx_gy_data_rate_set(&sfe_dev, (ism330dhcx_odr_g_t)rate);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// enableTimestamp()
//
// Enables time stamp counter
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable      Enables/disables time stamp counter
//

bool QwDevISM330DHCX::enableTimestamp(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_timestamp_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// resetTimestamp()
//
// Resets time stamp counter
//
//

bool QwDevISM330DHCX::resetTimestamp()
{
    int32_t retVal;

    retVal = ism330dhcx_timestamp_rst(&sfe_dev);

    if (retVal != 0)
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

//////////////////////////////////////////////////////////////////////////////////
// setFifoWatermark()
//
// Sets the FIFO's watermark
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Sets the watermark, must be some value less than 511 (9 bit value)
//

bool QwDevISM330DHCX::setFifoWatermark(uint16_t val)
{
    int32_t retVal;

    if (val > 511)
        return false;

    retVal = ism330dhcx_fifo_watermark_set(&sfe_dev, val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setFifoMode()
//
// Sets the FIFO's mode.
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Selects the FIFO's mode
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//

bool QwDevISM330DHCX::setFifoMode(uint8_t val)
{
    int32_t retVal;

    if (val > 7)
        return false;

    retVal = ism330dhcx_fifo_mode_set(&sfe_dev, (ism330dhcx_fifo_mode_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelFifoBatchSet()
//
// Sets the batch data rate for the accelerometer
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Sets the batch data rate.
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments
//

bool QwDevISM330DHCX::setAccelFifoBatchSet(uint8_t val)
{
    int32_t retVal;
    if (val > 11)
        return false;

    retVal = ism330dhcx_fifo_xl_batch_set(&sfe_dev, (ism330dhcx_bdr_xl_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroFifoBatchSet()
//
// Sets the batch data rate for the gyroscope
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Sets the batch data rate.
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setGyroFifoBatchSet(uint8_t val)
{
    int32_t retVal;
    if (val > 11)
        return false;

    retVal = ism330dhcx_fifo_gy_batch_set(&sfe_dev, (ism330dhcx_bdr_gy_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setFifoTimestampDec()
//
// Sets the FIFO time stamp decimation rate.
//
//  Parameter   Description
//  ---------   -----------------------------
//  val					Sets the time stamp decimation rate,
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setFifoTimestampDec(uint8_t val)
{
    int32_t retVal;
    if (val > 3)
        return false;

    retVal = ism330dhcx_fifo_timestamp_decimation_set(&sfe_dev, (ism330dhcx_odr_ts_batch_t)val);

    if (retVal != 0)
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

//////////////////////////////////////////////////////////////////////////////////
// setPinMode
//
// Sets the active state of the interrupt pin - high or low.
//
//  Parameter   Description
//  ---------   -----------------------------
//  openDrain   Set true for active low and false for active high
//

bool QwDevISM330DHCX::setPinMode(bool activeLow)
{
    int32_t retVal;

    // 0 = active high :  active low, 1
    retVal = ism330dhcx_pin_polarity_set(&sfe_dev, (ism330dhcx_h_lactive_t)activeLow);

    if (retVal != 0)
        return false;

    if (activeLow)
        // pinmode must be set to push-pull when active low is set.
        // See section 9.14 on pg 51 of datasheet for more information
        retVal = ism330dhcx_pin_mode_set(&sfe_dev, (ism330dhcx_pp_od_t)0);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setIntNotification
//
// Sets what the triggers an interrupt
//
//  Parameter   Description
//  ---------   -----------------------------
//  val         Sets which event triggers an interrupt.
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setIntNotification(uint8_t val)
{
    int32_t retVal;
    if (val > 4)
        return false;

    retVal = ism330dhcx_int_notification_set(&sfe_dev, (ism330dhcx_lir_t)val);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelStatustoInt1
//
// Sends the accelerometer's data ready signal to interrupt one.
//

bool QwDevISM330DHCX::setAccelStatustoInt1(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int1_route_t int1_route;

    retVal = ism330dhcx_pin_int1_route_get(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    int1_route.int1_ctrl.int1_drdy_xl = (uint8_t)enable;

    retVal = ism330dhcx_pin_int1_route_set(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    return true;
}


//////////////////////////////////////////////////////////////////////////////////
// setFIFOThresholdInt1
//
// Sends the accelerometer's data ready signal to interrupt one.
//
bool QwDevISM330DHCX::setFIFOThresholdInt1(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int1_route_t int1_route;

    retVal = ism330dhcx_pin_int1_route_get(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    int1_route.int1_ctrl.int1_fifo_th = (uint8_t)enable;

    retVal = ism330dhcx_pin_int1_route_set(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setBatchCounterInt1
//
// Sends the accelerometer's data ready signal to interrupt one.
//
bool QwDevISM330DHCX::setBatchCounterInt1(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int1_route_t int1_route;

    retVal = ism330dhcx_pin_int1_route_get(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    int1_route.int1_ctrl.int1_cnt_bdr = (uint8_t)enable;

    retVal = ism330dhcx_pin_int1_route_set(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setAccelStatustoInt2
//
// Sends the accelerometer's data ready signal to interrupt two.
//

bool QwDevISM330DHCX::setAccelStatustoInt2(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int2_route_t int2_route;

    retVal = ism330dhcx_pin_int2_route_get(&sfe_dev, &int2_route);

    if (retVal != 0)
        return false;

    int2_route.int2_ctrl.int2_drdy_xl = (uint8_t)enable;

    retVal = ism330dhcx_pin_int2_route_set(&sfe_dev, &int2_route);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroStatustoInt1
//
// Sends the gyroscopes's data ready signal to interrupt one.
//
bool QwDevISM330DHCX::setGyroStatustoInt1(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int1_route_t int1_route;

    retVal = ism330dhcx_pin_int1_route_get(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    int1_route.int1_ctrl.int1_drdy_g = (uint8_t)enable;

    retVal = ism330dhcx_pin_int1_route_set(&sfe_dev, &int1_route);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setGyroStatustoInt2
//
// Sends the gyroscopes's data ready signal to interrupt two.
//
bool QwDevISM330DHCX::setGyroStatustoInt2(bool enable)
{

    int32_t retVal;

    ism330dhcx_pin_int2_route_t int2_route;

    retVal = ism330dhcx_pin_int2_route_get(&sfe_dev, &int2_route);

    if (retVal != 0)
        return false;

    int2_route.int2_ctrl.int2_drdy_g = (uint8_t)enable;

    retVal = ism330dhcx_pin_int2_route_set(&sfe_dev, &int2_route);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setDataReadyMode
//
// Sets how the data ready signal is latched i.e. only return zero after interface reading
// OR is pulsed.
//
bool QwDevISM330DHCX::setDataReadyMode(uint8_t val)
{
    int32_t retVal;

    // 0 = Latched, 1 = Pulsed
    if (val > 1)
        return false;

    retVal = ism330dhcx_data_ready_mode_set(&sfe_dev, (ism330dhcx_dataready_pulsed_t)val);

    if (retVal != 0)
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

//////////////////////////////////////////////////////////////////////////////////
// setHubODR
//
// Sets the output data rate for the Sensor Hub.
//
//  Parameter   Description
//  ---------   -----------------------------
//  rate				Selects the rate
//
// See sfe_ism330dhcx_defs.h for a list of valid arguments

bool QwDevISM330DHCX::setHubODR(uint8_t rate)
{
    // 0 = 104Hz, 1 = 52Hz, 2 = 26Hz, 3 = 13Hz
    if (rate > 3)
        return false;

    int32_t retVal = ism330dhcx_sh_data_rate_set(&sfe_dev, (ism330dhcx_shub_odr_t)rate);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setHubSensorRead
//
// Sets the general sensor hub settings, which sensor and their I2C address and register
// to read.
//
//  Parameter   Description
//  ---------   -----------------------------
//  sensor      Selects the external sensor (1-4)
//  settings		The I2C address, register address, and number of reads for the external sensor
//							that is connected in sensor hub mode.
//
//
bool QwDevISM330DHCX::setHubSensorRead(uint8_t sensor, sfe_hub_sensor_settings_t *settings)
{
    int32_t retVal;
    ism330dhcx_sh_cfg_read_t tempSett;

    if (sensor > 3)
        return false;

    tempSett.slv_add = settings->address;
    tempSett.slv_subadd = settings->subAddress;
    tempSett.slv_len = settings->lenData;

    switch (sensor)
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

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setHubSensorWrite
//
// Gives settings to the 6DoF with regards to the address, register, and data to write to an external
// sensor.
//
//  Parameter   Description
//  ---------   -----------------------------
//  settings    Struct containing the address, register, and data to write to the external sensor
//

bool QwDevISM330DHCX::setHubSensorWrite(sfe_hub_sensor_settings_t *settings)
{
    int32_t retVal;
    ism330dhcx_sh_cfg_write_t tempSett;

    tempSett.slv0_add = settings->address;
    tempSett.slv0_subadd = settings->subAddress;
    tempSett.slv0_data = settings->lenData;

    retVal = ism330dhcx_sh_cfg_write(&sfe_dev, &tempSett);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setNumberHubSensors
//
// Sets the number of external sensor connected to the ISM
//
//  Parameter   Description
//  ---------   -----------------------------
//  numSensor   The number of sensors 0 - 3 = 4
//

bool QwDevISM330DHCX::setNumberHubSensors(uint8_t numSensors)
{

    int32_t retVal;
    if (numSensors > 3)
        return false;

    retVal = ism330dhcx_sh_slave_connected_set(&sfe_dev, (ism330dhcx_aux_sens_on_t)numSensors);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// enableSensorI2C
//
// Enables the 6DoF as an I2C sensor controller
//

bool QwDevISM330DHCX::enableSensorI2C(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_sh_master_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// readPeripheralSensor
//
// Read external sensor hub data.
//
//  Parameter   Description
//  ---------   -----------------------------
//  shReg       Pointer to store the data to be read.
//  len					Number of reads to be executed.

bool QwDevISM330DHCX::readPeripheralSensor(uint8_t *shReg, uint8_t len)
{
    int32_t retVal;

    retVal = ism330dhcx_sh_read_data_raw_get(&sfe_dev, (ism330dhcx_emb_sh_read_t *)shReg, len);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// getHubStatus
//
// Checks whether communication with the external sensor has concluded.
//

bool QwDevISM330DHCX::getHubStatus()
{

    int32_t retVal;
    ism330dhcx_status_master_t tempVal;

    retVal = ism330dhcx_sh_status_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if (tempVal.sens_hub_endop == 1)
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////
// getExternalSensorNack
//
// Retrieves communication status when communicating with external sensor
//
//  Parameter   Description
//  ---------   -----------------------------
//  sensor			The sensor to check, (0-3)

bool QwDevISM330DHCX::getExternalSensorNack(uint8_t sensor)
{

    int32_t retVal;
    ism330dhcx_status_master_t tempVal;

    retVal = ism330dhcx_sh_status_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    switch (sensor)
    {
    case 0:
        if (tempVal.slave0_nack == 1)
            return true;
        break;
    case 1:
        if (tempVal.slave1_nack == 1)
            return true;
        break;
    case 2:
        if (tempVal.slave2_nack == 1)
            return true;
        break;
    case 3:
        if (tempVal.slave3_nack == 1)
            return true;
        break;
    default:
        return false;
    }

    return false;
}

bool QwDevISM330DHCX::readMMCMagnetometer(uint8_t *magData, uint8_t len)
{
    return (readPeripheralSensor(magData, len));
}

//////////////////////////////////////////////////////////////////////////////////
// setHubWriteMode
//
// Sets how often the 6DoF should write to the external sensor: once per cycle i.e. output
// data rate, or just once.
//
//  Parameter   Description
//  ---------   -----------------------------
//  config       How often to be written

bool QwDevISM330DHCX::setHubWriteMode(uint8_t config)
{
    int32_t retVal;

    if (config > 1)
        return false;

    // 0 = Write each cycle
    // 1 = Write once
    retVal = ism330dhcx_sh_write_mode_set(&sfe_dev, (ism330dhcx_write_once_t)config);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setHubPassThrough
//
//	Allows the primary I2C data lines to communicate through the auxiliary I2C lines.
//
//  Parameter   Description
//  ---------   -----------------------------
//  set					Enables/disables pass through
//

bool QwDevISM330DHCX::setHubPassThrough(bool enable)
{

    int32_t retVal = ism330dhcx_sh_pass_through_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setHubFifoBatching
//
// Sets sensor hub FIFO batching
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable			Enable/disables fifo batching

bool QwDevISM330DHCX::setHubFifoBatching(bool enable)
{
    int32_t retVal;

    retVal = ism330dhcx_sh_batch_slave_0_set(&sfe_dev, (uint8_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// setHubPullUps
//
// Enables/Disables internal pullups on SDX/SCX lines
//
//  Parameter   Description
//  ---------   -----------------------------
//  enable			Enable/disables internal pullups on SDX/SCX lines

bool QwDevISM330DHCX::setHubPullUps(bool enable)
{

    int32_t retVal;

    retVal = ism330dhcx_sh_pin_mode_set(&sfe_dev, (ism330dhcx_shub_pu_en_t)enable);

    if (retVal != 0)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////
// resetSensorHub
//
// Resets all settings in the "Master Config" register
//

bool QwDevISM330DHCX::resetSensorHub()
{

    int32_t retVal;

    retVal = ism330dhcx_sh_reset_set(&sfe_dev);

    if (retVal != 0)
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

    if (val > 2)
        return false;

    retVal = ism330dhcx_xl_self_test_set(&sfe_dev, (ism330dhcx_st_xl_t)val);

    if (retVal != 0)
        return false;

    return true;
}

bool QwDevISM330DHCX::setGyroSelfTest(uint8_t val)
{
    int32_t retVal;

    // 0, 1, or 3
    if (val > 3)
        return false;

    retVal = ism330dhcx_gy_self_test_set(&sfe_dev, (ism330dhcx_st_g_t)val);

    if (retVal != 0)
        return false;

    return true;
}
//
//
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// checkStatus
//
// Checks if data is ready for both the acclerometer and the gyroscope
//

bool QwDevISM330DHCX::checkStatus()
{
    ism330dhcx_status_reg_t tempVal;
    int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if ((tempVal.xlda == 1) && (tempVal.gda == 1))
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////
// checkAccelStatus
//
// Checks if data is ready for the acclerometer

bool QwDevISM330DHCX::checkAccelStatus()
{
    ism330dhcx_status_reg_t tempVal;
    int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if (tempVal.xlda == 1)
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////
// checkAccelStatus
//
// Checks if data is ready for the gyroscope

bool QwDevISM330DHCX::checkGyroStatus()
{
    ism330dhcx_status_reg_t tempVal;
    int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if (tempVal.gda == 1)
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////
// checkAccelStatus
//
// Checks if data is ready for the temperature sensor

bool QwDevISM330DHCX::checkTempStatus()
{
    ism330dhcx_status_reg_t tempVal;
    int32_t retVal = ism330dhcx_status_reg_get(&sfe_dev, &tempVal);

    if (retVal != 0)
        return false;

    if (tempVal.tda == 1)
        return true;

    return false;
}
