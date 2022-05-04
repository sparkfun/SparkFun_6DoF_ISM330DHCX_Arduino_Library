#include "sfe_ism_shim.h"


////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Method to set the bus object that is used to communicate with the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  idBus        The id/address of the device on the bus

void QwDevTMF882X::setCommunicationBus(QwI2C &theBus, uint8_t idBus)
{
    _i2cBus = &theBus;
    _i2cAddress = idBus;
}

//////////////////////////////////////////////////////////////////////////////
//
//
int32_t platform_write(void *handle uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{
	return ((QwDevISM330DHCX*)handle->writeRegisterRegion(Reg,*Bufp, len));
}

int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
    return ((QwDevISM330DHCX*)handle->readRegisterRegion(Reg, *Bufp, len));
}

