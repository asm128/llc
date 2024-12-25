#include "llc_spi.h"
#include "llc_platform_error.h"

::llc::error_t llc::spiInit(SSPIDevice & device) {
	(void)device;
	return 0;
}
::llc::error_t llc::spiRead(SSPIDevice & device, uint16_t address, uint16_t count, ::llc::au0_t & data, ::llc::SPI_MODE mode, uint32_t timeout) {
#if defined(LLC_ST)
	HAL_StatusTypeDef result = HAL_ERROR;
	switch(mode) {
	case SPI_MODE_TASK: result = HAL_SPI_Receive    (device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	case SPI_MODE_IT  : result = HAL_SPI_Receive_IT (device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	case SPI_MODE_DMA : result = HAL_SPI_Receive_DMA(device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	default:
		error_printf("Unsupported operation mode: %i", (int32_t)mode);
	}
	return ::llc::error_t_from_hal_status(result);
#else
	(void)device, (void)address, (void)count, (void)data, (void)mode, (void)timeout;
	return -1;
#endif
}
::llc::error_t llc::spiWrite(SSPIDevice & device, uint16_t address, uint16_t count, ::llc::vcu0_c & data, ::llc::SPI_MODE mode, uint32_t timeout) {
#if defined(LLC_ST)
	HAL_StatusTypeDef result = HAL_ERROR;
	switch(mode) {
	case SPI_MODE_TASK: result = HAL_SPI_Transmit    (device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	case SPI_MODE_IT  : result = HAL_SPI_Transmit_IT (device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	case SPI_MODE_DMA : result = HAL_SPI_Transmit_DMA(device.Handle, device.Address, memAddress, memAddSize, data.begin(), data.size(), timeout);
	default:
	  error_printf("Unsupported operation mode: %i", (int32_t)mode);
	}
	return ::llc::error_t_from_hal_status(result);
#else
	(void)device, (void)address, (void)count, (void)data, (void)mode, (void)timeout;
	return -1;
#endif
}
