#include "ropp_actor.h"
#include <stdio.h>

using namespace ropp;

//#define RGBATOARGB( color ) \
//	( ((color & 0xFF) << 24L) | ((color & 0xFFFFFF00L) >> 8) )

s2_t ropp::BuildTexelsFromRGBAImage( const RGBA_IMAGE_DATA* pRGBAData, u2_t* out_pColors, u2_t* out_nTexelCount )
{
	if( 0 == pRGBAData )
		return -1;

	bool bCountOnly = false;
	if( 0 == out_pColors )
		bCountOnly = true;

	u2_t i, nPixelCount = pRGBAData->nWidth * pRGBAData->nHeight;
	for( i=0; i<nPixelCount; i++ )
	{
		if( false == bCountOnly )
			out_pColors[i] = pRGBAData->pImageData[i];
	}
	*out_nTexelCount = nPixelCount;
	return 0;
};


s2_t ropp::BuildTexelsFromPaletteImage( const PALETTE_IMAGE_RLE_DATA* pRLEData, const u2_t* pPalette, u2_t* out_nTexelCount, u2_t* out_pColors )
{
	if( 0 == pRLEData || 0 == pPalette || 0 == out_nTexelCount )
		return -1;

	bool bCountOnly = false;
	if( 0 == out_pColors )
		bCountOnly = true;

	if( bCountOnly )
	{
		*out_nTexelCount = pRLEData->nWidth * pRLEData->nHeight;
		return 0;
	}
	
	uint8_t * pImageData = pRLEData->pImageData;
	if( 0 == pImageData )
		return -1;
	
	u2_t i, nRepeat, nPixelIndex, nCompressedSize = pRLEData->nCompressedSize, // get some values in stack 
		nPixelCount = pRLEData->nWidth * pRLEData->nHeight;

	if( 0 == nCompressedSize )
	{
		nPixelIndex = 0;
		for( i=0; i<nPixelCount; i++ )
		{
			if( false == bCountOnly )
			{
				if( 0x00FF00FF == (pPalette[pImageData[i]] & 0x00FFFFFF) )
					out_pColors[i] = 0;
				else
					out_pColors[i] = 0xFF000000L | (pPalette[pImageData[i]]);
			}
			nPixelIndex++;
		}
		*out_nTexelCount = nPixelIndex;
		return 0;
	}

	nPixelIndex = 0;
	for( i=0; i<nCompressedSize; i++ )
	{
		if( (0 == pImageData[i]) && (i < (nCompressedSize-1)) )
		{
			i++; // increase to get the repeat count from next value
			nRepeat = pImageData[i];
			if( false == bCountOnly )
				memset( &out_pColors[nPixelIndex], 0, sizeof( u2_t )*nRepeat );
			nPixelIndex += nRepeat;
			continue;
		}
		else if(0 == pImageData[i])
		{
			if( false == bCountOnly )
				out_pColors[nPixelIndex] = 0;
			nPixelIndex++;
			continue;
		}
		
		if( false == bCountOnly )
		{
			if( 0 == pImageData[i] && (0xFFFF00FF == pPalette[pImageData[i]]) )
				out_pColors[nPixelIndex] = 0;
			else
				out_pColors[nPixelIndex] = 0xFF000000L 
				| ((pPalette[pImageData[i]] & 0xFF)<<16) 
				| ((pPalette[pImageData[i]] & 0xFF0000)>>16) 
				| (pPalette[pImageData[i]] & 0x00FF00) ;
		}
		nPixelIndex++;
	}
	
	*out_nTexelCount = nPixelIndex;
	return 0;
}


