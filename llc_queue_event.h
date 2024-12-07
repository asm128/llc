#include "llc_queue_async.h"
#include "llc_event.h"

#ifndef LLC_QUEUE_EVENT_H
#define LLC_QUEUE_EVENT_H

namespace llc
{
	tplt<tpnm _tEventType> using qevt			= ::llc::qobj <::llc::SEvent<_tEventType>>;
    tplt<tpnm _tEventType> using qpevt			= ::llc::qpobj<::llc::SEvent<_tEventType>>;

	tplt<tpnm _tEventType> using queue_event	= ::llc::qevt<_tEventType>;
    tplt<tpnm _tEventType> using queue_pevent	= ::llc::qpevt<_tEventType>;
}

#endif // LLC_QUEUE_EVENT_H
