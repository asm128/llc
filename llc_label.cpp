#include "llc_label.h"
#include "llc_label_manager.h"

static	::llc::CLabelManager*	labelManagerSingleton		()			noexcept	{ 
	static	::llc::CLabelManager	instanceLabelManager; 
	return &instanceLabelManager; 
}

		::llc::label::label			(const char * text, uint32_t stringLen)	noexcept	: LabelManager(::labelManagerSingleton())	{ 
	e_if_failed(LabelManager->View(text, stringLen, *this), "text: %s.", text); 
}

bool	::llc::label::oper==	(const ::llc::vcst_t & other)		const	noexcept	{
	return (Data == other.begin()) 	? true
		: (Count != other.size()) 	? false 
		: 0 == memcmp(Data, other.begin(), Count)
		;
}

bool	::llc::label::oper==	(const label & other)			const	noexcept	{
	return (Data == other.Data) 	? true
		: (Count != other.Count) 	? false
		: (LabelManager == other.LabelManager) ? false 
		: 0 == memcmp(Data, other.Data, Count)
		;
}
