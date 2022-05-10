#pragma once
#include "st_src\ism330dhcx_reg.h"

#ifdef __cpluplus
extern "C"{
#endif


int32_t sfeISMWrite(void* fTarget, uint8_t reg, const uint8_t *bufp, uint16_t len);
int32_t sfeISMRead(void* fTarget, uint8_t reg, uint8_t *bufp, uint16_t len);


extern stmdev_ctx_t sfe_dev; 


#ifdef __cpluplus
}
#endif
