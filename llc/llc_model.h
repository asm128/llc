#include "llc_matrix3.h"

#ifndef LLC_MODEL_H_23627
#define LLC_MODEL_H_23627

namespace llc
{
#pragma pack(push, 1)
	struct SModelMatrices {
		::llc::m4f2_t	Scale		;
		::llc::m4f2_t	Rotation	;
		::llc::m4f2_t	Position	;
	};

	struct SModel3 {
		::llc::n3f2_t	Scale		;
		::llc::n3f2_t	Rotation	;
		::llc::n3f2_t	Position	;
	};
#pragma pack(pop)
} // namespace

#endif // LLC_MODEL_H_23627
