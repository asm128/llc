#include "llc_string_compose.h"

// -*- Generic string enclosing -*-
llc::err_t	llc::appendNclosd			(asc_t & output, vcst_t textToEnclose) 											{ return append_strings(output, '{', textToEnclose,'}'); }
llc::err_t	llc::appendBraced			(asc_t & output, vcst_t textToEnclose) 											{ return append_strings(output, '[', textToEnclose,']'); }
llc::err_t	llc::appendQuoted			(asc_t & output, vcst_t textToEnclose) 											{ return append_strings(output, '"', textToEnclose,'"'); }
llc::err_t	llc::appendGtlted			(asc_t & output, vcst_t textToEnclose) 											{ return append_strings(output, '<', textToEnclose,'>'); }
llc::err_t	llc::appendNclosd			(asc_t & output, vcst_t textToQuote, vcst_t openChars, vcst_t closeChars)		{ return append_strings(output, openChars, textToQuote, closeChars); }
llc::err_t	llc::appendNclosd			(asc_t & output, vcst_t textToQuote, char openChar, char closeChar)				{ return append_strings(output, openChar, textToQuote, closeChar); }
llc::err_t	llc::appendNclosd			(asc_t & output, vcst_t textToQuote, char encloserChar)							{ return append_strings(output, encloserChar, textToQuote, encloserChar); }
llc::err_t	llc::appendNclosdPrefixed	(asc_t & output, vcst_t textToQuote, char prefix, char encloser) 				{ return append_strings(output, prefix, encloser, textToQuote, encloser); }
llc::err_t	llc::appendNclosdPrefixed	(asc_t & output, vcst_t textToQuote, char prefix, char openCh, char closeCh)	{ return append_strings(output, prefix, openCh, textToQuote, closeCh); }
llc::err_t	llc::appendNclosdPrefixed	(asc_t & output, vcst_t textToEnclose, bool separate, char separator)			{ return separate ? appendNclosdPrefixed(output, textToEnclose, separator, '{', '}') : appendNclosd(output, textToEnclose); }
llc::err_t	llc::appendBracedPrefixed	(asc_t & output, vcst_t textToEnclose, bool separate, char separator)			{ return separate ? appendNclosdPrefixed(output, textToEnclose, separator, '[', ']') : appendBraced(output, textToEnclose); }
llc::err_t	llc::appendQuotedPrefixed	(asc_t & output, vcst_t textToEnclose, bool separate, char separator)			{ return separate ? appendNclosdPrefixed(output, textToEnclose, separator, '"')		: appendQuoted(output, textToEnclose); }
llc::err_t	llc::appendGtltedPrefixed	(asc_t & output, vcst_t textToEnclose, bool separate, char separator)			{ return separate ? appendNclosdPrefixed(output, textToEnclose, separator, '<', '>') : appendGtlted(output, textToEnclose); }
