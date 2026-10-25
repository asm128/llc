#include "llc_string_compose.h"

#ifndef LLC_APPEND_JSON_H
#define LLC_APPEND_JSON_H

namespace llc
{
				err_t	appendOpenKey	(asc_t & output, vcst_t key								, b8_t prependComma = false);
				err_t	appendKeyValue	(asc_t & output, vcst_t key, vcst_t value				, b8_t prependComma = false);
				err_t	appendKeyObject	(asc_t & output, vcst_t key, vcst_t valuesNotEnclosed	, b8_t prependComma = false);
				err_t	appendKeyList	(asc_t & output, vcst_t key, vcst_t valuesNotEnclosed	, b8_t prependComma = false);
				err_t	appendKeyString	(asc_t & output, vcst_t key, vcst_t value				, b8_t prependComma = false);


} // namespace

#endif // LLC_APPEND_JSON_H