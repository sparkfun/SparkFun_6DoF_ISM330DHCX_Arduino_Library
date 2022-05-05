#include "sfe_ism_shim.h"

int32_t platform_write(void *handle uint8_t Reg, const uint8_t *Bufp, uint16_t len)
{
	return ((QwDevISM330DHCX*)handle->writeRegisterRegion(Reg,*Bufp, len));
}

int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp, uint16_t len)
{
    return ((QwDevISM330DHCX*)handle->readRegisterRegion(Reg, *Bufp, len));
}

