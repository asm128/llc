#include "llc_error.h"

#ifndef LLC_BLOCK_H_23627
#define LLC_BLOCK_H_23627

namespace llc
{
	stct SRecordMap {
		::llc::u2_t	IdBlock		= {};
		::llc::u2_t	IndexRecord	= {};
	};

	::llc::err_t	blockRecordIndices	(::llc::u3_c idRecord, ::llc::u2_c blockSize, ::llc::SRecordMap & indices)	nxpt;
	::llc::err_t	blockRecordId		(cnst ::llc::SRecordMap & indices, ::llc::u2_c blockSize, ::llc::u3_t & idRecord)	nxpt;
} // namespace

#endif // LLC_BLOCK_H_23627
