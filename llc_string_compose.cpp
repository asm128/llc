#include "llc_string_compose.h"

// -*- Generic string enclosing -*-
llc::err_t	llc::appendNclosd			(::llc::asc_t & output, ::llc::vcst_t textToEnclose) 												{ return ::llc::append_strings(output, '{', textToEnclose,'}'); }
llc::err_t	llc::appendBraced			(::llc::asc_t & output, ::llc::vcst_t textToEnclose) 												{ return ::llc::append_strings(output, '[', textToEnclose,']'); }
llc::err_t	llc::appendQuoted			(::llc::asc_t & output, ::llc::vcst_t textToEnclose) 												{ return ::llc::append_strings(output, '"', textToEnclose,'"'); }
llc::err_t	llc::appendGtlted			(::llc::asc_t & output, ::llc::vcst_t textToEnclose) 												{ return ::llc::append_strings(output, '<', textToEnclose,'>'); }
llc::err_t	llc::appendNclosd			(::llc::asc_t & output, ::llc::vcst_t textToQuote, ::llc::vcst_t openChars, ::llc::vcst_t closeChars)	{ return ::llc::append_strings(output, openChars, textToQuote, closeChars); }
llc::err_t	llc::appendNclosd			(::llc::asc_t & output, ::llc::vcst_t textToQuote, char openChar, char closeChar)					{ return ::llc::append_strings(output, openChar, textToQuote, closeChar); }
llc::err_t	llc::appendNclosd			(::llc::asc_t & output, ::llc::vcst_t textToQuote, char encloserChar)								{ return ::llc::append_strings(output, encloserChar, textToQuote, encloserChar); }
llc::err_t	llc::appendNclosdPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToQuote, char prefix, char encloser) 					{ return ::llc::append_strings(output, prefix, encloser, textToQuote, encloser); }
llc::err_t	llc::appendNclosdPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToQuote, char prefix, char openCh, char closeCh) 		{ return ::llc::append_strings(output, prefix, openCh, textToQuote, closeCh); }
llc::err_t	llc::appendNclosdPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToEnclose, bool separate, char separator)				{ return separate ? ::llc::appendNclosdPrefixed(output, textToEnclose, separator, '{', '}') : ::llc::appendNclosd(output, textToEnclose); }
llc::err_t	llc::appendBracedPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToEnclose, bool separate, char separator)				{ return separate ? ::llc::appendNclosdPrefixed(output, textToEnclose, separator, '[', ']') : ::llc::appendBraced(output, textToEnclose); }
llc::err_t	llc::appendQuotedPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToEnclose, bool separate, char separator)				{ return separate ? ::llc::appendNclosdPrefixed(output, textToEnclose, separator, '"')		: ::llc::appendQuoted(output, textToEnclose); }
llc::err_t	llc::appendGtltedPrefixed	(::llc::asc_t & output, ::llc::vcst_t textToEnclose, bool separate, char separator)				{ return separate ? ::llc::appendNclosdPrefixed(output, textToEnclose, separator, '<', '>') : ::llc::appendGtlted(output, textToEnclose); }
