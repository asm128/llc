#include "llc_sync.h"

#ifndef LLC_FREERTOS
#   include <mutex>
#else // LLC_FREERTOS
#   include "freertos/FreeRTOS.h"
#   include "freertos/task.h"
#   include "freertos/semphr.h"
#endif // LLC_FREERTOS

#ifndef LLC_MUTEX_H
#define LLC_MUTEX_H

namespace llc
{
    //usng ::std::mutex;
#ifndef LLC_FREERTOS
    struct mutex : ::std::mutex {
        inln err_t          lock        ()      { ::std::mutex::lock(); return 0; }
        inln err_t          try_lock    ()      { return ::std::mutex::try_lock() ? 0 : -1; }
        inln err_t          unlock      ()      { ::std::mutex::unlock(); return 0; }
#else // LLC_FREERTOS
    struct mutex {
        SemaphoreHandle_t   Lock        = {};

                            ~mutex      ()      { if(Lock) vSemaphoreDelete(Lock); }
                            mutex       ()      { if_null_te(Lock = xSemaphoreCreateMutex()); }

        err_t               lock        ()      { if_true_fe(xSemaphoreTake(Lock, portMAX_DELAY) != pdTRUE); return 0; }
        err_t               try_lock    ()      { if_true_fi(xSemaphoreTake(Lock, 1) != pdTRUE); return 0; }
        err_t               unlock      ()      { if_true_fe(xSemaphoreGive(Lock) != pdTRUE); return 0; }
#endif // LLC_FREERTOS
    };
} // namespace 

#ifndef LLC_MUTEX_DECLARE
#   ifdef LLC_MTSUPPORT
#	    define LLC_MUTEX_DECLARE(Name) ::llc::mutex Name
#	    define LLC_MUTEX_ENTER(Name)   (Name).lock()
#	    define LLC_MUTEX_LEAVE(Name)   (Name).unlock()
#   else // !LLC_MTSUPPORT
#	    define LLC_MUTEX_DECLARE(...)
#	    define LLC_MUTEX_ENTER(...)		do {} while(0)
#	    define LLC_MUTEX_LEAVE(...)		do {} while(0)
#   endif // LLC_MTSUPPORT
#endif // LLC_MUTEX_DECLARE

#endif // LLC_MUTEX_H