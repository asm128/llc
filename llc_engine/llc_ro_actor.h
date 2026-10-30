// Copyright (c) 2011 Pablo Ariel Zorrilla Cepeda
//
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// http://www.opensource.org/licenses/mit-license.php
//
#include "ropp_defines.h"

#ifndef _ROPP_ACTOR_H
#define _ROPP_ACTOR_H

namespace ropp
{
#pragma pack( push, 1 )
	//
	// Notes:
	// - magenta RGB(255,0,255) is usually displayed as an 'invisible' color
	// - images with square dimentions are recommended 
	//
	// On version 1.0 the system palette is used instead.
	// 1.1 - Added palette 
	// 2.0 - Added RGBA images
	// 2.1 - Added RLE-encoding of the background in palette images


	// Palette image (RLE-encoded background)
	// Each byte represents an index in the palette.
	// Palette index 0 is the 'invisible' background.
	// Only the background is RLE encoded.
	// When a 0x00 byte is parsed, the next byte indicates the number of 0x00 bytes it decompresses to. (0x00 0x00 decompresses to a single 0x00 byte) 
	typedef struct _PALETTE_IMAGE_RLE_DATA
	{
		uint16_t		nWidth;
		uint16_t		nHeight;
		uint16_t		nCompressedSize; //  size of the compressed data // set 0 for uncompressed images
		uint8_t*		pImageData; // image data, RLE encoded for the background. if compressedsize>0 then the size is nWidth*nHeight
	} PALETTE_IMAGE_RLE_DATA;

	// 32bpp
	typedef struct _RGBA_IMAGE_DATA
	{
		uint16_t	nWidth;
		uint16_t	nHeight;
		u2_t*	pImageData;
	} RGBA_IMAGE_DATA;

	typedef struct _SPR_2_1_FILE_DATA
	{
		uint16_t				nMagicNumber;	// 'PS'
		uint8_t					nVersionMinor; 	
		uint8_t					nVersionMajor;	// I know it should be the other way but it seems to use this order
		uint16_t				nPaletteImages;
		uint16_t				nRGBAImages;	// v >= 2.0
		PALETTE_IMAGE_RLE_DATA*	pPaletteImages; // v >= 2.1
		RGBA_IMAGE_DATA*		pRGBAImages;	// v >= 2.0
		u2_t				Palette[256];	// v >= 1.1
	} SPR_2_1_FILE_DATA, SPR_FILE_DATA;

	// this seems to add an offset to the sprite
	typedef struct _ACT_PIVOT_DATA
	{
	// TODO investigate. imf files are related to this data
		u2_t	Unknown;	//???			4			??? ignored
		s2_t		CenterX;	//			4			??? offset in the frame (center is 0,0)
		s2_t		CenterY;	//			4			??? offset in the frame (center is 0,0)
		u2_t	nAttribute;	//		4			???
	}ACT_PIVOT_DATA;

	// this structure contains info about how to map the frames from the images
	typedef struct _ACT_FRAME_DATA
	{
		s2_t		CenterX;		// offset in the frame (center is 0,0?) 
		s2_t		CenterY;		// 
		u2_t	nFrameIndex;	// frame index in the sprite texture
		u2_t	bMirror;		// If non zero the image is mirrored along the vertical axis
		u2_t	nColor;			// RGBA color (for blending?) (default 0xFFFFFFFF)
		f2_t		ScaleX;			
		f2_t		ScaleY;
		f2_t		fRotationAngle;			
		u2_t	nSpriteType;	// 0 = palette image, 1 = rgb image (default 0) 
		u2_t	nWidth;			// ??
		u2_t	nHeight;		// ??
	}ACT_FRAME_DATA;

	typedef struct _ACT_SPRITE_DATA
	{
		s2_t			Range1[4];	//RECT{left,top,right,bottom} ?
		s2_t			Range2[4];
		u2_t		nFrameCount;	// Number of sprite frame (max=1000) (int)
		ACT_FRAME_DATA*	lstFrameData;	// 
//		{ >= v2.0 (set to -1 on v2.0)
		u2_t		nEventIndex; //	(default -1)
//			{	>= v2.3
		u2_t		nPivotCount; // ??? number of anchors (default 0)
		ACT_PIVOT_DATA* lstPivotData;
//			}
//		}
	}ACT_SPRITE_DATA;

	typedef struct _ACT_ACTION_DATA
	{
		u2_t nSpriteCount;
		ACT_SPRITE_DATA* lstSpriteData;
	}ACT_ACTION_DATA;

	typedef char ACT_EVENT_NAME[40];// event ("atk") or pathname of sound file

	typedef struct _ACT_FILE_DATA
	{
		uint16_t			nMagicNumber;
		uint8_t				nVersionMinor; 	
		uint8_t				nVersionMajor; 	
		uint16_t			nActionCount;	// Number of actions 
		uint8_t				nUknown[10];	// Whadda u know about the ways of the UnderSide?
		ACT_ACTION_DATA*	lstActionData;	
//	{ >= v2.1
		u2_t			nEventCount; // number of events/sound files (default 0)
		ACT_EVENT_NAME* 	lstEventNames;
//		{ >= v2.2
		f2_t*				lstFrameDelay; //sizeof(f2_t)*nActionCount interval of action frames? (f2_t) (default 4.0)
//		}
//	}
	}ACT_FILE_DATA;
#pragma pack( pop )

	void CleanSPRData( SPR_FILE_DATA& pSPRData );

	s2_t LoadSPRDataFromFile( const wchar_t* pFilename, SPR_FILE_DATA* pSPRData );
	s2_t LoadSPRDataFromMemory( const char* pBuffer, SPR_FILE_DATA* pSPRData );
	
	s2_t	LoadACTDataFromFile( const wchar_t* pFilename, ACT_FILE_DATA* pACTData );
	s2_t	LoadACTDataFromFile( const char* pFilename, ACT_FILE_DATA* pACTData );
	void	CleanACTData( ACT_FILE_DATA& ActData );

	s2_t BuildTexelsFromPaletteImage( const PALETTE_IMAGE_RLE_DATA* pRLEData, 
		const u2_t* pPalette, u2_t* out_nTexelCount, u2_t* out_pColors );
	s2_t BuildTexelsFromRGBAImage( const RGBA_IMAGE_DATA* pRGBAData, 
		u2_t* out_pColors, u2_t* out_nTexelCount );
} // namespace

#endif