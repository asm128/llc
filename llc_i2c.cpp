#include "llc_i2c.h"
#include "llc_platform_error.h"

::llc::error_t llc::i2cInit(SI2CHost & device) {
	(void)device;
	return 0;
}
::llc::err_t	llc::i2cLoad	(SI2CHost & host, u0_t address, au0_t & data, u1_t howManyBytes, u2_t timeout, BUS_MODE mode) {
	if_fail_fe(data.resize(howManyBytes));
	if_fail_fe(i2cLoad(host, address, (vu0_t&)data, howManyBytes, timeout, mode));
	return 0;
}
::llc::err_t	llc::i2cLoad	(SI2CHost & host, u0_t address, vu0_t & data, u1_t howManyBytes, u2_t timeout, BUS_MODE mode) {
#ifdef LLC_ST
	HAL_StatusTypeDef result = HAL_ERROR;
	switch(mode) {
	case I2C_MODE_TASK: result = HAL_I2C_Receive    (host.PlatformHandle, address, memAddSize, data.begin(), data.size(), timeout);
	case I2C_MODE_IT  : result = HAL_I2C_Receive_IT (host.PlatformHandle, address, memAddSize, data.begin(), data.size(), timeout);
	case I2C_MODE_DMA : result = HAL_I2C_Receive_DMA(host.PlatformHandle, address, memAddSize, data.begin(), data.size(), timeout);
	default:
		error_printf("Unsupported operation mode: %i", (int32_t)mode);
	}
	return ::llc::error_t_from_hal_status(result);
#else
	(void)host, (void)address, (void)howManyBytes, (void)data, (void)mode, (void)timeout;
	return -1;
#endif
}
::llc::error_t llc::i2cSave		(SI2CHost & host, u0_t address, vcu0_c & data, u2_t timeout, BUS_MODE mode) {
#ifdef LLC_ST
	HAL_StatusTypeDef result = HAL_ERROR;
	switch(mode) {
	case I2C_MODE_TASK: result = HAL_I2C_Transmit    (host.Handle, address, memAddSize, data.begin(), data.size(), timeout);
	case I2C_MODE_IT  : result = HAL_I2C_Transmit_IT (host.Handle, address, memAddSize, data.begin(), data.size(), timeout);
	case I2C_MODE_DMA : result = HAL_I2C_Transmit_DMA(host.Handle, address, memAddSize, data.begin(), data.size(), timeout);
	default:
	  error_printf("Unsupported operation mode: %i", (int32_t)mode);
	}
	return ::llc::error_t_from_hal_status(result);
#else
	(void)host, (void)address, (void)data, (void)mode, (void)timeout;
	return -1;
#endif
}
