#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../system/kapi.h"

int core_init(struct kernel_api *kapi);

void core_deinit(void);

#ifdef __cplusplus
}
#endif
