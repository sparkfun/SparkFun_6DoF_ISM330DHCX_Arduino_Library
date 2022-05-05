#include "st_src\ism330dhcx_reg.h"

#ifdef __cpluplus
extern "C"{
#endif

int32_t platform_write(void* handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
int32_t platform_read(void* handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);

int32_t set_xl_full_scale(void* handle, uint8_t accelerationVal);

#ifdef __cpluplus
}
#endif
