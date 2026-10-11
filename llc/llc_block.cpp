#include "llc_block.h"

::llc::err_t llc::blockRecordIndices(::llc::u3_c idRecord, ::llc::u2_c blockSize, ::llc::SRecordMap & indices) nxpt {
	if_zero(blockSize)
		rtrn ::llc::OS_INVALID_PARAMETER;
	cnst ::llc::u3_t idBlock = idRecord / blockSize;
	if(idBlock > ::llc::u2_t(-1))
		rtrn ::llc::OS_OVERRUN;
	indices = {::llc::u2_t(idBlock), ::llc::u2_t(idRecord % blockSize)};
	rtrn 0;
}

::llc::err_t llc::blockRecordId(cnst ::llc::SRecordMap & indices, ::llc::u2_c blockSize, ::llc::u3_t & idRecord) nxpt {
	if_zero(blockSize)
		rtrn ::llc::OS_INVALID_PARAMETER;
	if(indices.IndexRecord >= blockSize)
		rtrn ::llc::OS_INVALID_PARAMETER;
	idRecord = ::llc::u3_t(indices.IdBlock) * blockSize + indices.IndexRecord;
	rtrn 0;
}
