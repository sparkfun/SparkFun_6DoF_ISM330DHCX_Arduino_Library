#include "sfe_ism_shim.h"
#include "sfe_ism330dhcx.h"
#include "st_src\ism330dhcx_reg.h"


stmdev_ctx_t sfe_dev = {.write_reg = sfe_ism_write, 
												.read_reg = sfe_ism_read };

int32_t sfeISMWrite(void* fTarget, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	return (((QwDevISM330DHCX*)fTarget)->writeRegisterRegion(reg, (uint8_t*)bufp, len));
}

int32_t sfeISMRead(void* fTarget, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    return (((QwDevISM330DHCX*)fTarget)->readRegisterRegion(reg, bufp, len));
}

int32_t setAccelFullScale(uint8_t val){

	return (ism330dhcx_xl_full_scale_set(&sfe_dev, ism330dhcx_fs_xl_t(val)));
}

int32_t getUniqueId(uint8_t* buff){
	
	return(ism330dhcx_device_id_get(&sfe_dev, buff));
}



//int32_t set_xl_full_scale(void* handle, uint8_t accelerationVal)
//{
//
//	return( ism330dhcx_xl_full_scale_set(((stmdev_ctx_t*)handle, accelerationVal) );
//
//}
