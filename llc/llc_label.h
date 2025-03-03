#include "llc_view.h"
#include "llc_view_serialize.h"

#ifndef LLC_LABEL_H_23627
#define LLC_LABEL_H_23627

namespace llc
{
	struct label_statics;

	class CLabelManager;
	class label : public vcsc_t {
		CLabelManager		* LabelManager	= 0;
	public:
		inxp				label			()								noexcept	= default;
							label			(const label & other)			noexcept	= default;
							label			(const char* str, uint32_t len)	noexcept;

		label&				oper=		(const label & other)			noexcept	= default;

		tplt<size_t _stringLength>
		inline 				label			(const char (&str)[_stringLength], uint32_t count = (uint32_t)-1)	noexcept	: label(&str[0], ::llc::min((uint32_t)_stringLength, count))	{}
		inline 				label			(const ::llc::vcst_t & other)											noexcept	: label(other.begin(), other.size())							{}

		inline	oper	::llc::vcst_t		()								const				{ return {Data, Count}; }

				bool		oper==		(const ::llc::vcst_t & other)		const	noexcept;
				bool		oper==		(const label & other)			const	noexcept;
		inline	bool		oper!=		(const label & other)			const	noexcept	{ return !oper==(other); }
	};

	stin ::llc::error_t	loadLabel		(::llc::vcu0_t & input, ::llc::vcsc_t & output) { 
		uint32_t					bytesRead		= 0;
		llc_necs(bytesRead = ::llc::viewRead(output, input)); 
		llc_necs(input.slice(input, bytesRead));
		output					= ::llc::label(output);
		return bytesRead;
	}
}

#endif // LLC_LABEL_H_23627
