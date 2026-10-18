#include "llc_engine_renderbuffer.h"
#include "llc_geometry.h"

#ifndef LLC_ENGINE_GEOMETRY_H
#define LLC_ENGINE_GEOMETRY_H

namespace llc 
{
	::llc::error_t			createBuffers
		( const ::llc::SGeometryBuffers	& geometry
		, ::llc::pobj<SRenderBuffer>	& pIndicesVertex
		, ::llc::pobj<SRenderBuffer>	& pVertices
		, ::llc::pobj<SRenderBuffer>	& pNormals
		, ::llc::pobj<SRenderBuffer>	& pUV
		);

	::llc::error_t			createBuffers
		( SRenderBufferManager			& bufferManager 
		, const ::llc::SGeometryBuffers	& geometry
		, uint32_t						& iIndicesVertex
		, uint32_t						& iVertices
		, uint32_t						& iNormals
		, uint32_t						& iUV
		);

} // namespace

#endif // LLC_ENGINE_GEOMETRY_H
