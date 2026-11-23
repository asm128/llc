#include "llc_index_catalog.h"

#ifndef TEST_LLC_INDEX_CATALOG_H
#define TEST_LLC_INDEX_CATALOG_H

stct SIndexCatalog {
	::llc::SJSONFile					Source					= {};
	::llc::aobj<::llc::SItemViews>		Item					= {};
	::llc::aobj<::llc::SWordIndices>	Word					= {};
	::llc::SWordDictData				WordData				= {};

	::llc::err_t	Load			(::llc::vcst_t fileName, ::llc::AD_SHOP_CATEGORY category, ::llc::vcst_t language);
	::llc::err_t	Resolve			(::llc::vcst_t input, ::llc::au2_t & itemIndices) cnst;
	::llc::err_t	SourceValue		(::llc::u2_t itemIndex, ::llc::vcst_t expression, ::llc::vcst_t & output) cnst;
};

#endif // TEST_LLC_INDEX_CATALOG_H
