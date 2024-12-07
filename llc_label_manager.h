#include "llc_block_container_nts.h"

#ifndef LLC_LABEL_MANAGER_H_23627
#define LLC_LABEL_MANAGER_H_23627

namespace llc
{
	class CLabelManager	{
#if defined(LLC_ARDUINO)
		stxp	u2_c	BLOCK_SIZE	= 1024 >> 1;
#elif defined(LLC_ESP32)
		stxp	u2_c	BLOCK_SIZE	= 1024 * 4;
#else
		stxp	u2_c	BLOCK_SIZE	= 1024 * 64;
#endif
		::llc::block_container_nts<BLOCK_SIZE>	Characters;
		::llc::vcsc_t							Empty;

	public:	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		::llc::au2_t							Counts;
		::llc::apod<const char*>			Texts;

											~CLabelManager				()																{
			for(uint32_t iText = 0; iText < Texts.size(); ++iText)
				verbose_printf("Label found: %s.", Texts[iText]);
		}
		inline								CLabelManager				()																{ 
			Characters	.push_sequence("", 0U, Empty); 
			Counts		.push_back(0); 
			Texts		.push_back(Empty.begin()); 
		}

		::llc::error_t						Save		(::llc::au0_t & output)						const	{
			llc_necs(llc::saveView(output, Counts));
			for(uint32_t iArray = 0; iArray < Counts.size(); ++iArray)
				llc_necs(output.append((const uint8_t*)Texts[iArray], Counts[iArray]));

			return 0;
		}

		::llc::error_t						Load		(::llc::vcu0_t & input) {
			llc_necs(llc::loadView(input, Counts));
			::llc::vcsc_t					out_view; 
			uint32_t					offsetByte					= 0;
			for(uint32_t iArray = 0; iArray < Counts.size(); ++iArray) {
				uint32_t					elementCount				= Counts[iArray];
				llc_necs(Characters.push_sequence((const char*)&input[offsetByte], elementCount, out_view));
				offsetByte				+= elementCount;
				llc_necs(Texts.push_back(out_view.begin()));
			}
			input					= {input.begin(), input.size() - offsetByte};
			return 0;
		}

		inline	::llc::error_t				Size		()					const	noexcept	{ return Texts.size(); }
		inline	::llc::vcsc_t					View		(uint32_t index)	const				{ return {Texts[index], Counts[index]}; }
		inline	::llc::error_t				View		(const char* elements, uint16_t count)	{ ::llc::vcsc_t out_view; return View(elements, count, out_view); }

		::llc::error_t						Index		(::llc::vcsc_c & elements) {
			ree_if(elements.size() >= CLabelManager::BLOCK_SIZE, "Data too large: %" LLC_FMT_U2 ".", elements.size());

			for(uint32_t iView = 0, countLabels = Texts.size(); iView < countLabels; ++iView) {
				if(elements.size() != Counts[iView])
					continue;

				const char								* pStored					= Texts[iView];
				if(0 == memcmp(pStored, elements.begin(), elements.byte_count()))
					return iView;
			}
			return -1;
		}

		::llc::error_t						View		(const char* text, uint32_t textLen, ::llc::vcsc_t & out_view)		{
			if(0 == textLen || 0 == text || 0 == text[0]) {
				out_view							= Empty;
				return 0;
			}

			uint32_t								ntslen						= 0;
			for(u2_c countChars = ::llc::min(textLen, CLabelManager::BLOCK_SIZE - 1); ntslen < countChars; ++ntslen)
				if(0 == text[ntslen])
					break;

			u2_c							viewIndex					= (uint32_t)Index({text, ntslen});
			if(viewIndex < Texts.size()) {
				out_view							= {Texts[viewIndex], Counts[viewIndex]};
				return viewIndex;
			}

			llc_necs(Characters	.push_sequence(text, ntslen, out_view));
			llc_necs(Texts		.push_back(out_view.begin()));
			return Counts		.push_back((uint16_t)out_view.size());
		}
	};
}

#endif // LLC_LABEL_MANAGER_H_23627
