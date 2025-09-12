#include "llc_array_pod.h"
#include "llc_array_obj.h"
#include "llc_ptr_pod.h"
#include "llc_ptr_obj.h"

#include "llc_mutex.h"

#ifndef LLC_QUEUE_ASYNC_H
#define LLC_QUEUE_ASYNC_H

namespace llc
{
	tplt<tpnm _t, tpnm _tContainer>
	struct queue_async {
		tydf	_t	 		T;
		tydf	_tContainer	TContainer;

		TContainer				Queue	= {};
		::llc::mutex			Lock	= {};

		inline	err_t 	Size	()						{ ::llc::lock_guard locked(Lock); return Queue.size(); }
		inline	err_t	Clear	()						{ ::llc::lock_guard	locked(Lock); return Queue.clear(); }
		inline	err_t	Post	(const T & item) 		{ ::llc::lock_guard locked(Lock); return Queue.push_back(item); }
		// Appends to the `output` container all the messages from the queue
		inline	err_t	Read	(TContainer & output, bool clearQueue = true) {
			::llc::lock_guard		locked(Lock);
			llc_necs(output.append(Queue));
			if(clearQueue) {
				Queue.clear();
			}
			return 0;
		}
		// Returns 1 + the amount of messages remaining in the queue
		err_t			Next	(T & output) {
			err_t	left;
			{
				::std::lock_guard	locked(Lock);
				rnis_if(0 == Queue.size());
				output	= Queue[0];
				llc_necs(left = Queue.remove(0));
			}
			return 1 + left;
		}
	};

	tplt<tpnm _tObj> using qobj			= ::llc::queue_async<_tObj, ::llc::aobj<_tObj>>;
	tplt<tpnm _tPOD> using qpod			= ::llc::queue_async<_tPOD, ::llc::apod<_tPOD>>;
	tplt<tpnm _tObj> using qpobj		= ::llc::qobj<::llc::pobj<_tObj>>;
	tplt<tpnm _tPOD> using qppod		= ::llc::qobj<::llc::ppod<_tPOD>>;

	tplt<tpnm _tObj> using queue_obj	= ::llc::qobj <_tObj>;
	tplt<tpnm _tPOD> using queue_pod	= ::llc::qpod <_tPOD>;
	tplt<tpnm _tObj> using queue_pobj	= ::llc::qpobj<_tObj>;
	tplt<tpnm _tPOD> using queue_ppod	= ::llc::qppod<_tPOD>;

} // namespace

#endif // LLC_QUEUE_ASYNC_H
