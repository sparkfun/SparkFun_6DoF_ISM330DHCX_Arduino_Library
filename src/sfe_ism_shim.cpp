#include "sfe_ism_shim.h"
#include "sfe_ism330dhcx.h"
#include "st_src\ism330dhcx_reg.h"


stmdev_ctx_t sfe_dev = {.write_reg = sfeISMWrite, 
												.read_reg = sfeISMRead };

int32_t sfeISMWrite(void* fTarget, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	return (((QwDevISM330DHCX*)fTarget)->writeRegisterRegion(reg, (uint8_t*)bufp, len));
}

int32_t sfeISMRead(void* fTarget, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    return (((QwDevISM330DHCX*)fTarget)->readRegisterRegion(reg, bufp, len));
}



