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
		block_container_nts<BLOCK_SIZE>	Characters;
		vcst_t							Empty;

	public:	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		au2_t							Counts;
		apod<const char*>			Texts;

											~CLabelManager				()																{
			for(uint32_t iText = 0; iText < Texts.size(); ++iText)
				verbose_printf("Label found: %.*s.", (int)Counts[iText], Texts[iText]);
		}
		inline								CLabelManager				()																{ 
			Characters	.push_sequence("", 0U, Empty); 
			Counts		.push_back(0); 
			Texts		.push_back(Empty.begin()); 
		}

		error_t						Save		(au0_t & output)						const	{
			if_true_fe(Counts.size() != Texts.size());
			llc_necs(llc::saveView(output, Counts));
			for(uint32_t iArray = 0; iArray < Counts.size(); ++iArray)
				llc_necs(output.append((const uint8_t*)Texts[iArray], Counts[iArray]));

			return 0;
		}

		error_t						Load		(vcu0_t & input) {
			vcu0_t						inputToRead	= input;
			au2_t						counts		= {};
			apod<sc_c *>			texts		= {};
			block_container_nts<BLOCK_SIZE>	characters	= {};
			vcst_t						empty		= {};
			if_fail_fe(loadView(inputToRead, counts));
			if_zero_fe(counts.size());
			if_true_fe(counts[0]);
			u2_t offsetByte = {};
			for(u2_t iText = 0; iText < counts.size(); ++iText) {
				u2_c elementCount = counts[iText];
				if_true_fef(elementCount > inputToRead.size() - offsetByte, LLC_FMT_GT_U2, elementCount, inputToRead.size() - offsetByte);
				vcst_t text = {elementCount ? (cnst sc_t*)&inputToRead[offsetByte] : "", elementCount};
				vcst_t stored = {};
				if_fail_fe(characters.push_sequence(text.begin(), text.size(), stored));
				if(0 == iText)
					empty = stored;
				if_fail_fe(texts.push_back(stored.begin()));
				offsetByte += elementCount;
			}
			if_fail_fe(inputToRead.slice(inputToRead, offsetByte));
			Characters	= characters;
			Counts		= counts;
			Texts		= texts;
			Empty		= empty;
			input		= inputToRead;
			rtrn 0;
		}

		inline	error_t				Size		()					const	noexcept	{ return Texts.size(); }
		inline	vcst_t					View		(uint32_t index)	const				{ return {Texts[index], Counts[index]}; }
		inline	error_t				View		(const char* elements, uint16_t count)	{ vcst_t out_view; return View(elements, count, out_view); }

		error_t						Index		(vcst_t elements) {
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

		error_t						View		(const char* text, uint32_t textLen, vcst_t & out_view)		{
			if(0 == textLen || 0 == text || 0 == text[0]) {
				out_view							= Empty;
				return 0;
			}

			uint32_t								ntslen						= 0;
			for(u2_c countChars = min(textLen, CLabelManager::BLOCK_SIZE - 1); ntslen < countChars; ++ntslen)
				if(0 == text[ntslen])
					break;

			u2_c							viewIndex					= (uint32_t)Index({text, ntslen});
			if(viewIndex < Texts.size()) {
				out_view							= {Texts[viewIndex], Counts[viewIndex]};
				return viewIndex;
			}

			llc_necs(Characters.push_sequence(text, ntslen, out_view));
			llc_necs(Texts.push_back(out_view.begin()));
			cnst err_t iCount = Counts.push_back(out_view.size());
			if(0 > iCount)
				Texts.pop_back();
			rtrn iCount;
		}
	};
}

#endif // LLC_LABEL_MANAGER_H_23627
