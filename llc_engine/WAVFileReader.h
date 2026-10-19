// Functions for loading WAV audio files. Copyright (c) Microsoft Corporation. Licensed under the MIT License.
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
#include "llc_array_ptr.h"
#include "llc_array.h"

#ifdef LLC_WINDOWS

#include <objbase.h>

#include <mmreg.h>

#ifndef WAVFILEREADER_H_23627
#define WAVFILEREADER_H_23627

namespace DirectX
{
	HRESULT					LoadWAVAudioInMemory	(::llc::vcu0_t wavData, _Outptr_ const WAVEFORMATEX* * wfx, ::llc::vcu0_t & audioView) noexcept;
	HRESULT					LoadWAVAudioFromFile	(::llc::vcsc_t szFileName, ::llc::au0_t & wavData, const WAVEFORMATEX* * wfx, ::llc::vcu0_t & audioView) noexcept;
							
	struct WAVData {		
		const WAVEFORMATEX		* WaveFormat			= 0;
		::llc::vcu0_t				AudioView				= {};
		uint32_t				LoopStart				= 0;
		uint32_t				LoopLength				= 0;
		const uint32_t			* Seek					= 0;	// Note: XMA Seek data is Big-Endian
		uint32_t				SeekCount				= 0;
	};						
							
	HRESULT					LoadWAVAudioInMemoryEx	(::llc::vcu0_t wavData, WAVData & result) noexcept;
	HRESULT					LoadWAVAudioFromFileEx	(::llc::vcsc_t szFileName, ::llc::au0_t & wavData, WAVData & result) noexcept;
}

#endif // LLC_WINDOWS

#endif // WAVFILEREADER_H_02397492830