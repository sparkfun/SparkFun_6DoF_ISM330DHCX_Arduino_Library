#include "ism330dhcx_reg.h"

#ifdef __cpluplus
extern "C"{
#endif

void platform_write(void* handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);
void platform_read(void* handle, uint8_t Reg, const uint8_t *Bufp, uint16_t len);


#ifdef __cpluplus
}
#endif
