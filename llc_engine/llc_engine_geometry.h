#include "llc_engine_renderbuffer.h"
#include "llc_geometry.h"

#ifndef LLC_ENGINE_GEOMETRY_H
#define LLC_ENGINE_GEOMETRY_H

namespace llc 
{
	::llc::err_t			createBuffers
		( const ::llc::SGeometryBuffers	& geometry
		, ::llc::pobj<SRenderBuffer>	& pIndicesVertex
		, ::llc::pobj<SRenderBuffer>	& pVertices
		, ::llc::pobj<SRenderBuffer>	& pNormals
		, ::llc::pobj<SRenderBuffer>	& pUV
		);

	::llc::err_t			createBuffers
		( SRenderBufferManager			& bufferManager 
		, const ::llc::SGeometryBuffers	& geometry
		, u2_t						& iIndicesVertex
		, u2_t						& iVertices
		, u2_t						& iNormals
		, u2_t						& iUV
		);

} // namespace

#endif // LLC_ENGINE_GEOMETRY_H
