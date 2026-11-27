#include "llc_enum.h"
#include "llc_reader.h"

#ifndef LLC_PARSE_H_23627
#define LLC_PARSE_H_23627

namespace llc
{
	stxp	vcst_t	DIGITS_HEX						= "0123456789abcdef";
	stxp	vcst_t	DIGITS_DECIMAL					= "0123456789";
	err_t			parseArbitraryBaseInteger		(u2_t base, vcst_t symbolList, vcst_t sourceChars, u3_t * number_);
	tplt<tpnm _tInt>	
	inline	err_t	parseIntegerDecimal				(vcst_t sourceChars, _tInt & number_)	{
		u3_t				number							= 0; 
		err_t				countDigits; 
		if_fail_fe(countDigits = parseArbitraryBaseInteger(10, vcst_t{"0123456789"}, sourceChars, &number)); 
		number_ = (_tInt)number; 
		return countDigits; 
	}

	tplt<tpnm _tInt>	
	inline	err_t	parseIntegerHexadecimal			(vcst_t sourceChars, _tInt & number_)	{
		u3_t				number							= 0; 
		err_t countDigits;
		if_fail_fe(countDigits = parseArbitraryBaseInteger(16, vcst_t{"0123456789abcdef"}, sourceChars, &number)); 
		number_ = (_tInt)number; 
		return countDigits; 
	}

	GDEFINE_ENUM_TYPE(STRIP_LITERAL_TYPE, int8_t);
	GDEFINE_ENUM_VALUE(STRIP_LITERAL_TYPE, LITERAL	, 0);
	GDEFINE_ENUM_VALUE(STRIP_LITERAL_TYPE, TOKEN	, 1);
	GDEFINE_ENUM_VALUE(STRIP_LITERAL_TYPE, COUNT	, 2);
	GDEFINE_ENUM_VALUE(STRIP_LITERAL_TYPE, UNKNOWN	, -1);
#pragma pack(push, 1)
	tydf SToken<STRIP_LITERAL_TYPE> SStripLiteralType;

	stct SStripLiteralState : public SReaderState<STRIP_LITERAL_TYPE> {
		bool				InsideToken						= false;
		s2_t				BracketsToSkip					= 0;
	};
#pragma pack(pop)
	err_t				stripLiteralParse				(SStripLiteralState	& stateReading	, apod<SStripLiteralType> & out_types, vcst_t in_format);
	err_t				stripLiteralParseStep			(SStripLiteralState	& work_state	, apod<SStripLiteralType> & out_types, vcst_t in_format);
	err_t				stripLiteralGetViews			(aobj<vcst_t>	& out_views, const view<const SStripLiteralType> & in_resultOfParser, vcst_t in_format);

	bool				isSpaceCharacter				(const char characterToTest);
	err_t				skipToNextCharacter				(u2_t & indexCurrentChar, vcst_t expression);

} // namespace

#endif // LLC_PARSE_H_23627
