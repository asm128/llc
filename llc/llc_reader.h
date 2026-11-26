#include "llc_array_pod.h"
#include "llc_slice.h"

#ifndef LLC_READER_H_261125
#define LLC_READER_H_261125

nmsp llc
{
#pragma pack(push, 1)
	tplt<tpnm TToken>
	stct SToken {
		static_assert(1 == szof(TToken), "Reader token types must use one byte.");

		s2_t		ParentIndex;
		TToken		Type	: 6;
		TToken				: 2;
		sliceu2_t	Span;
	};

	tplt<tpnm TToken>
	stct SReaderState {
		static_assert(szof(SToken<TToken>) == szof(s2_t) + szof(u0_t) + szof(sliceu2_t), "Reader tokens must keep one packed type byte.");

		u2_t			IndexCurrentChar	= 0;
		s2_t			IndexCurrentElement	= -1;
		SToken<TToken>	* CurrentElement	= 0;
		s2_t			NestLevel			= 0;
		sc_t			CharCurrent			= 0;
		b8_t			Escaping			: 1;
		b8_t			InsideString		: 1;
		b8_t			ExpectingSeparator	: 1;
		b8_t			DoneReading			: 1;

						SReaderState		()
			: Escaping			(false)
			, InsideString		(false)
			, ExpectingSeparator(false)
			, DoneReading		(false)
			{}
	};
#pragma pack(pop)

	tplt<tpnm TToken>
	stin err_t readerStateParseStep(SReaderState<TToken> & state, vcst_t text) {
		if(state.IndexCurrentChar >= text.size())
			rtrn -1;
		state.CharCurrent		= text[state.IndexCurrentChar];
		rtrn 0;
	}

	tplt<tpnm TToken>
	stct SReader {
		apod<SToken<TToken>>	Token;

		err_t	Reset	() {
			Token.clear();
			rtrn 0;
		}
	};
} // nmsp

#endif // LLC_READER_H_261125
