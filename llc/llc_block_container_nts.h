#include "llc_block_container.h"

#ifndef LLC_BLOCK_CONTAINER_NTS_261011
#define LLC_BLOCK_CONTAINER_NTS_261011

namespace llc
{
	tplt<size_t _size>
	class block_container_nts : private block_container<sc_t, _size> {
		using TBase = block_container<sc_t, _size>;

	public:
		using TBase::clear;
		using TBase::Save;
		using TBase::Load;

		err_t	push_sequence	(sc_c * sequence, u2_t length, vcst_t & out_view) {
			if_true_fef(length >= _size, LLC_FMT_GE_U2, length, (u2_t)_size);
			if_true_fe(length && 0 == sequence);
			vs0_t	stored	= {};
			err_t	iBlock	= {};
			if_fail_fe(iBlock = TBase::reserve(length + 1, stored));
			if(length)
				memcpy(stored.begin(), sequence, length);
			stored[length]	= 0;
			out_view		= {stored.begin(), length};
			rtrn iBlock;
		}
	};
} // namespace

#endif // LLC_BLOCK_CONTAINER_NTS_261011
