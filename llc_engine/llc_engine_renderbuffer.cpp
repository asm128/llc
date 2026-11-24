#include "llc_engine_renderbuffer.h"

::llc::err_t			llc::createBuffers	
	( ::llc::u2_t							vertexCount
	, ::llc::pobj<::llc::SRenderBuffer>	& pIndicesVertex
	, ::llc::pobj<::llc::SRenderBuffer>	& pVertices
	, ::llc::pobj<::llc::SRenderBuffer>	& pNormals
	, ::llc::pobj<::llc::SRenderBuffer>	& pUV
	) {
	::llc::clear(pIndicesVertex, pVertices, pNormals, pUV);
	pVertices	->Desc.Format	= ::llc::DATA_TYPE_FLOAT32_3;
	pVertices	->Desc.Usage	= ::llc::BUFFER_USAGE_Position;

	pNormals	->Desc.Format	= ::llc::DATA_TYPE_FLOAT32_3;
	pNormals	->Desc.Usage	= ::llc::BUFFER_USAGE_Normal;

	pUV			->Desc.Format	= ::llc::DATA_TYPE_FLOAT32_2;
	pUV			->Desc.Usage	= ::llc::BUFFER_USAGE_UV;

	pIndicesVertex->Desc.Usage	= ::llc::BUFFER_USAGE_Index;
	pIndicesVertex->Desc.Format	
		= (vertexCount > 0xFFFF) ? ::llc::DATA_TYPE_UINT32
		: (vertexCount > 0x00FF) ? ::llc::DATA_TYPE_UINT16
		: ::llc::DATA_TYPE_UINT8
		;
	return 0;
}
