#include "llc_bus_manager.h"

#ifndef LLC_I2C_H
#define LLC_I2C_H

namespace llc
{
#pragma pack(push, 1)
	stct SI2CDevice {
		u0_t				Address			= (u0_t)-1;
	};										  
	stct SI2CPinout {						  
		u0_t				Clock			= (u0_t)-1;
		u0_t				Datum			= (u0_t)-1;
	};
	stct SI2CHost {
#ifdef LLC_CMSIS
		I2C_HandleTypeDef	PlatformHandle	= 0;
#elif defined(LLC_ARDUINO)
		WireClass			PlatformHandle	= 0;
#endif // LLC_CMSIS
		apobj<SI2CDevice>	Slaves			= {};
		SI2CPinout			Pinout			= {};
		u2_t				Frequency		= (u2_t)-1;
	};
#pragma pack(pop)
	err_t	i2cInit	(SI2CHost & host);	// Set clock frequency, pin modes and pin states
	// Read
	err_t	i2cLoad	(SI2CHost & host, u0_t address, au0_t  & data, u1_t byteCountToLoad, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
	err_t	i2cLoad	(SI2CHost & host, u0_t address, vu0_t  & data, u1_t byteCountToLoad, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
	// Send
	err_t	i2cSave	(SI2CHost & host, u0_t address, vcu0_c & data, u2_t timeout, BUS_MODE mode = BUS_MODE_DMA);
} // namespace

#endif // LLC_I2C_H
