#include "llc_label.h"
#include "llc_label_manager.h"

sttc	llc::CLabelManager*	labelManagerSingleton	()	nxpt	{ 
	sttc	::llc::CLabelManager	instanceLabelManager; 
	return &instanceLabelManager; 
}
		llc::label::label	(sc_c * text, u2_t stringLen)	nxpt
	: LabelManager(::labelManagerSingleton()) {
	if_fail_ef(LabelManager->View(text, stringLen, *this), "text: %.*s.", (int)stringLen, text);
}
bool	llc::label::oper==	(vcst_t other)			csnx	{
	return (Data == other.begin()) 	? true
		: (Count != other.size()) 	? false 
		: 0 == memcmp(Data, other.begin(), Count)
		;
}
bool	llc::label::oper==	(cnst label & other)	csnx	{
	return (Data == other.Data) 	? true
		: (Count != other.Count) 	? false
		: (LabelManager == other.LabelManager) ? false 
		: 0 == memcmp(Data, other.Data, Count)
		;
}
