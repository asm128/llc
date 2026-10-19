#include "llc_engine_d3d.h"

#if defined(LLC_WINDOWS)

#include "llc_file.h"

::llc::error_t			llc::d3dCreateBuffersFromEngineMeshes		(ID3D11Device * pDevice, const ::llc::SMeshManager & engineMeshes, const ::llc::SRenderBufferManager & engineBuffers, ::llc::array_com<ID3D11Buffer> & indexBuffers, ::llc::array_com<ID3D11Buffer> & vertexBuffers)	{
	for(uint32_t iMesh = 0; iMesh < engineMeshes.size(); ++iMesh) {
		const ::llc::pobj<::llc::SGeometryMesh>		& mesh									= engineMeshes[iMesh];
		if(!mesh)
			continue;

		::llc::pobj<::llc::SRenderBuffer>			engineBufferIndices;
		::llc::apobj<::llc::SRenderBuffer>			engineBufferVertices;
		::llc::au2_t						layoutOffsets;

		uint32_t										vertexSize					= 0;
		for(uint32_t iBuffer = 0; iBuffer < mesh->GeometryBuffers.size(); ++iBuffer) {
			const ::llc::pobj<::llc::SRenderBuffer>		& buffer								= engineBuffers[mesh->GeometryBuffers[iBuffer]];
			if(buffer->Desc.Usage == ::llc::BUFFER_USAGE_Index) {
				engineBufferIndices							= buffer;
				continue;
			}
			llc_necs(engineBufferVertices.push_back(buffer));
			llc_necs(layoutOffsets.push_back(vertexSize));
			vertexSize									+= buffer->Desc.Format.TotalBytes();

		}
		::llc::apod<uint8_t>						packed;
		for(uint32_t iValue = 0, valueCount = engineBufferVertices[0]->Data.size() / engineBufferVertices[0]->Desc.Format.TotalBytes(); iValue < valueCount; ++iValue) {
			for(uint32_t iBuffer = 0; iBuffer < engineBufferVertices.size(); ++iBuffer) {
				const ::llc::SRenderBuffer						& buffer					= *engineBufferVertices[iBuffer];
				::llc::vcu0_t							value						= {&buffer.Data[iValue * buffer.Desc.Format.TotalBytes()], buffer.Desc.Format.TotalBytes()};
				packed.append(value);
			}
		}
		{
			D3D11_SUBRESOURCE_DATA							vertexBufferData			= {packed.begin()};
			D3D11_BUFFER_DESC								vertexBufferDesc			= {packed.size()};
			vertexBufferDesc.BindFlags					= D3D11_BIND_VERTEX_BUFFER;
			::llc::ptr_com<ID3D11Buffer>					d3dBuffer;
			llc_hrcall(pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d3dBuffer));
			vertexBuffers.push_back(d3dBuffer);
		}
		{
			D3D11_SUBRESOURCE_DATA			indexBufferData			= {engineBufferIndices->Data.begin()};
			D3D11_BUFFER_DESC				indexBufferDesc			= {engineBufferIndices->Data.size()};
			indexBufferDesc.BindFlags	= D3D11_BIND_INDEX_BUFFER;
			::llc::au1_t						indices;
			const SDataTypeID				indexFormat				= engineBufferIndices->Desc.Format;
			const uint32_t					indexSize				= engineBufferIndices->Desc.Format.TotalBytes();
			//if(engineBufferIndices->Desc.Format.TotalBytes() == 4) 
			//else 
			if(indexSize == 1) {
				if_fail_fe(indices.resize(engineBufferIndices->Data.size()));
				indices.enumerate([engineBufferIndices](uint32_t & i, uint16_t & value){ value = engineBufferIndices->Data[i]; return 0; }, 0);
				indexBufferData.pSysMem	= indices.begin();
				indexBufferDesc.ByteWidth = indices.byte_count();
			}

			::llc::ptr_com<ID3D11Buffer>					d3dBuffer;
			llc_hrcall(pDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &d3dBuffer));
			indexBuffers.push_back(d3dBuffer);
		}
	}

	return 0;
}

::llc::error_t			llc::d3dCreateTexturesFromEngineSurfaces	(ID3D11Device* pDevice, const ::llc::SSurfaceManager & engineSurfaces, ::llc::array_com<ID3D11ShaderResourceView> & shaderResourceViews) {
	for(uint32_t iSurface = 0; iSurface < engineSurfaces.size(); ++iSurface) {
		const ::llc::SSurface						& surface			= *engineSurfaces[iSurface];
		D3D11_TEXTURE2D_DESC						desc				= {surface.Desc.Dimensions.x, surface.Desc.Dimensions.y, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, {1, 0}, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE};
		D3D11_SUBRESOURCE_DATA						initData			= {};
		initData.pSysMem						= surface.Data.begin();
		initData.SysMemPitch					= surface.Desc.Dimensions.x * 4;

		::llc::ptr_com<ID3D11Texture2D>				pTex2D				= {};
		llc_hrcall(pDevice->CreateTexture2D(&desc, &initData, &pTex2D));
		if(!pTex2D) 
			continue;

		D3D11_SHADER_RESOURCE_VIEW_DESC				srvDesc				= {};
		srvDesc.Format							= desc.Format;
		srvDesc.ViewDimension					= D3D_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels				= desc.MipLevels;

		::llc::ptr_com<ID3D11ShaderResourceView>	pSRV;
		llc_hrcall(pDevice->CreateShaderResourceView(pTex2D, &srvDesc, &pSRV));
		shaderResourceViews.push_back(pSRV);
	}
	return 0;
}

//
::llc::error_t			llc::d3dCreatePixelShadersFromEngineShaders	(ID3D11Device* pDevice, const ::llc::SShaderManager & engineShaders, ::llc::array_com<ID3D11PixelShader> & pixelShaders) {
	for(uint32_t iShader = 0; iShader < engineShaders.size(); ++iShader) {
		const ::llc::vcsc_t								shaderName				= engineShaders.Names[iShader];

		// After the pixel shader file is loaded, create the shader.
		sc_t											shaderFileName	[1024]	= {};
		sprintf_s(shaderFileName, "%s.cso", shaderName.begin());
		::llc::apod<int8_t>						filePS;
		llc_necs(llc::fileToMemory(::llc::vcs{shaderFileName}, filePS));

		::llc::ptr_com<ID3D11PixelShader>				pixelShader;
		llc_hrcall(pDevice->CreatePixelShader(&filePS[0], filePS.size(), nullptr, &pixelShader));
		pixelShaders.push_back(pixelShader);
	}
	return 0;
}

#endif // LLC_WINDOWS
