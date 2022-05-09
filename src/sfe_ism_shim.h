#pragma once
#include "st_src\ism330dhcx_reg.h"

#ifdef __cpluplus
extern "C"{
#endif


int32_t sfe_ism_write(void* fTarget, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t sfe_ism_read(void* fTarget, uint8_t Reg, const uint8_t *Bufp, uint16_t len);


stmdev_ctx_t sfe_dev; 

sfe_dev.write_reg = sfe_ism_write;
sfe_dev.read_reg = sfe_ism_read;

//int32_t set_xl_full_scale(void* handle, uint8_t accelerationVal);

#ifdef __cpluplus
}
#endif
