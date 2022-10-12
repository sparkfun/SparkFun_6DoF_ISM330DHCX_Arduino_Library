#pragma once
#include "st_src/ism330dhcx_reg.h"

#ifdef __cpluplus
extern "C"{
#endif

// These two functions satisfy STMicroelectronic's library requirements for providing
// write and read methods.  
int32_t sfeISMWrite(void* fTarget, uint8_t reg, const uint8_t *bufp, uint16_t len);
int32_t sfeISMRead(void* fTarget, uint8_t reg, uint8_t *bufp, uint16_t len);

// Initializes the interfacing struct for STMicroelectronic's library. 
void initCtx(void* handle, stmdev_ctx_t* dev);


#ifdef __cpluplus
}
#endif
