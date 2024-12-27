#include "llc_sync.h"

#ifndef LLC_FREERTOS
#else // LLC_FREERTOS
#	include "freertos/FreeRTOS.h"
#	include "freertos/task.h"
#	include "freertos/semphr.h"
#endif // LLC_FREERTOS

#include <mutex>

#ifndef LLC_MUTEX_H
#define LLC_MUTEX_H

namespace llc
{
#ifndef LLC_FREERTOS
	struct mutex : ::std::mutex {
		inln	err_t	lock		()	{ ::std::mutex::lock(); rtrn 0; }
		inln	err_t	try_lock	()	{ rtrn ::std::mutex::try_lock() ? 0 : -1; }
		inln	err_t	unlock		()	{ ::std::mutex::unlock(); rtrn 0; }
#else // LLC_FREERTOS
	struct hal_mutex {
#	if CONFIG_DISABLE_HAL_LOCKS
		sinx	err_t	lock		()	{ rtrn 0; }
		sinx	err_t	try_lock	()	{ rtrn 0; }
		sinx	err_t	unlock		()	{ rtrn 0; }
#	else // !CONFIG_DISABLE_HAL_LOCKS
		QueueHandle_t	Lock		= {};
		inln			~mutex		()	{ if(Lock) vQueueDelete((QueueHandle_t)Lock); }
						mutex		()	{ if_null_te(Lock = xQueueCreateMutex(queueQUEUE_TYPE_MUTEX)); }
		err_t			lock		()	{ if_zero_fe(xQueueSemaphoreTake(Lock, portMAX_DELAY)); rtrn 0; }
		err_t			try_lock	()	{ if_zero_fi(xQueueSemaphoreTake(Lock, 1)); rtrn 0; }
		err_t			unlock		()	{ if_zero_fe(xQueueGenericSend((QueueHandle_t)Lock, 0, semGIVE_BLOCK_TIME, queueSEND_TO_BACK)); rtrn 0; }
#	endif // CONFIG_DISABLE_HAL_LOCKS
	};
	struct mutex {
		QueueHandle_t	Lock		= {};
		inln			~mutex		()	{ if(Lock) vQueueDelete((QueueHandle_t)Lock); }
						mutex		()	{ if_null_te(Lock = xQueueCreateMutex(queueQUEUE_TYPE_MUTEX)); }
		err_t			lock		()	{ if_zero_fe(xQueueSemaphoreTake(Lock, portMAX_DELAY)); rtrn 0; }
		err_t			try_lock	()	{ if_zero_fi(xQueueSemaphoreTake(Lock, 1)); rtrn 0; }
		err_t			unlock		()	{ if_zero_fe(xQueueGenericSend((QueueHandle_t)Lock, 0, semGIVE_BLOCK_TIME, queueSEND_TO_BACK)); rtrn 0; }
#endif // LLC_FREERTOS
	};
} // namespace

#endif // LLC_MUTEX_H