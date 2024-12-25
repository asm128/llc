#include "llc_bus_manager.h"

#ifndef LLC_SPI_H
#define LLC_SPI_H

namespace llc
{
#pragma pack(push, 1)
	stct SSPIDevice {
		u0_t				Pin					= (u0_t)-1;
	};
	stct SSPIPinout {
		u0_t				Clock				= (u0_t)-1;
		u0_t				MasterOut			= (u0_t)-1;
		u0_t				RemoteOut			= (u0_t)-1;
	};
	stct SSPIHost {
#ifdef LLC_CMSIS
		SPI_HandleTypeDef	PlatformHandle		= 0;
#elif defined(LLC_ARDUINO)
		SPIClass			PlatformHandle		= 0;
#else
		void				* PlatformHandle	= 0;
#endif // LLC_CMSIS
		apobj<SSPIDevice>	Slaves				= {};
		SSPIPinout			Pinout				= {};
		u2_t				Frequency			= (u2_t)-1;
	};
#pragma pack(pop)

	err_t	spiInit	(SSPIHost & device);
	err_t	spiLoad	(SSPIHost & device, u0_t pin, au0_t  & data, u2_t registerIndex, u2_t registerCount, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
	err_t	spiLoad	(SSPIHost & device, u0_t pin, vu0_t  & data, u2_t registerIndex, u2_t registerCount, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
	err_t	spiSave	(SSPIHost & device, u0_t pin, vcu0_c & data, u2_t registerIndex, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
} // namespace

#endif // LLC_SPI_H
