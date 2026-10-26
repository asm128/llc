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
		mutex			Lock	= {};

		inline	err_t 	Size	()						{ lock_guard locked(Lock); return Queue.size(); }
		inline	err_t	Clear	()						{ lock_guard locked(Lock); return Queue.clear(); }
		inline	err_t	Post	(const T & item) 		{ lock_guard locked(Lock); return Queue.push_back(item); }
		// Appends to the `output` container all the messages from the queue
		inline	err_t	Read	(TContainer & output, bool clearQueue = true) {
			lock_guard			locked(Lock);
			if_fail_fe(output.append(Queue));
			if(clearQueue) {
				Queue.clear();
			}
			return 0;
		}
		// Returns 1 + the amount of messages remaining in the queue
		err_t			Next	(T & output) {
			err_t	left;
			{
				lock_guard	locked(Lock);
				if_zerp_vi(0, Queue.size());
				output	= Queue[0];
				if_fail_fe(left = Queue.remove(0));
			}
			return 1 + left;
		}
	};

	tplt<tpnm _tObj> using qobj			= queue_async<_tObj, aobj<_tObj>>;
	tplt<tpnm _tPOD> using qpod			= queue_async<_tPOD, apod<_tPOD>>;
	tplt<tpnm _tObj> using qpobj		= qobj<pobj<_tObj>>;
	tplt<tpnm _tPOD> using qppod		= qobj<ppod<_tPOD>>;

	tplt<tpnm _tObj> using queue_obj	= qobj <_tObj>;
	tplt<tpnm _tPOD> using queue_pod	= qpod <_tPOD>;
	tplt<tpnm _tObj> using queue_pobj	= qpobj<_tObj>;
	tplt<tpnm _tPOD> using queue_ppod	= qppod<_tPOD>;

} // namespace

#endif // LLC_QUEUE_ASYNC_H
