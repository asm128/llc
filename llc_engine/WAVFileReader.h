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
	LLC_USING_APOD();
	LLC_USING_VIEW();
	LLC_USING_TYPEINT();

	HRESULT					LoadWAVAudioInMemory	(vcu0_t wavData, _Outptr_ const WAVEFORMATEX* * wfx, vcu0_t & audioView) noexcept;
	HRESULT					LoadWAVAudioFromFile	(vcst_t szFileName, au0_t & wavData, const WAVEFORMATEX* * wfx, vcu0_t & audioView) noexcept;
							
	struct WAVData {		
		const WAVEFORMATEX		* WaveFormat			= 0;
		vcu0_t				AudioView				= {};
		u2_t				LoopStart				= 0;
		u2_t				LoopLength				= 0;
		const u2_t			* Seek					= 0;	// Note: XMA Seek data is Big-Endian
		u2_t				SeekCount				= 0;
	};						
							
	HRESULT					LoadWAVAudioInMemoryEx	(vcu0_t wavData, WAVData & result) noexcept;
	HRESULT					LoadWAVAudioFromFileEx	(vcst_t szFileName, au0_t & wavData, WAVData & result) noexcept;
}

#endif // LLC_WINDOWS

#endif // WAVFILEREADER_H_02397492830