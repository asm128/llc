#include "llc_error.h"

#ifndef LLC_PLATFORM_ERROR_H_9237849823
#define LLC_PLATFORM_ERROR_H_9237849823

namespace llc
{
#if defined(LLC_ST)
	::llc::err_t error_t_from_st_hal_status(HAL_StatusTypeDef halStatus);
#else 
	::llc::err_t error_t_from_st_hal_status(s2_t status);
#endif
} // namespace

#endif // LLC_ERROR_HELPER_H_9237849823
