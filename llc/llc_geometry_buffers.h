#include "llc_apod_n3.h"
#include "llc_apod_n2.h"

#ifndef LLC_GEOMETRYBUFFERS_H_23627
#define LLC_GEOMETRYBUFFERS_H_23627

namespace llc
{
	struct SGeometryBuffers {
		::llc::an3f2_t	Positions;
		::llc::an3f2_t	Normals;
		::llc::an2f2_t	TextureCoords;
		::llc::au2_t	Colors;
		::llc::au2_t	PositionIndices;
	};
} // llc

#endif // LLC_GEOMETRYBUFFERS_H_23627