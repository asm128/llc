#include <llc_view.h>

using namespace llc;

//  * Data			= 0;
//  Count			= 0;

tpl_t
sttc	llc::err_t  testViewConstructors	() { 
	cnst _t testData0[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; 
	
	{ llc::view<_t> view0; if_true_fef(view0. size() || view0.begin(), "%u || %p", view0. size(), view0.begin()); } //	  nxpt	= dflt;
	{ llc::view<cnst _t> view0{testData0}		 ; } //										  nxpt	: Data(elements), Count(N)			
	{ llc::view<cnst _t> view0{0U, testData0}	 ; } // (u2_t elementCount, T (&elements)[N])			: Data(elements), Count(::llc::min(N, elementCount))
	{ llc::view<cnst _t> view0{testData0}		 ; } // (TVCst & other)						  nxpt	= dflt;					
	{ llc::view<cnst _t> view0{&testData0[0], 10} ; } // (T * elements, u2_t elementCount)				: Data(elements), Count(elementCount)				
	{ llc::view<cnst _t> view0{testData0}		 ; } // (T (&elements)[N], u2_t elementCount)			: Data(elements), Count(::llc::min(N, elementCount))
	return 0;
}

tpl_t	sttc	llc::err_t  testViewCastConst		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn {Data, Count}; }
tpl_t	sttc	llc::err_t  testViewSubscript		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(u2_t index)																			*/ ; return view0.size(); } //		{
tpl_t	sttc	llc::err_t  testViewSubscriptConst	()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(u2_t index)																			*/ ; return view0.size(); } // cnst	{
tpl_t	sttc	llc::err_t  testViewOperatorNotEqual()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(TCVCs & other)																			*/ ; return view0.size(); } // cnst	{ rtrn  !operator==(other); } // I had to add this f
tpl_t	sttc	llc::err_t  testViewOperatorEqual	()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(TCVCs & other)																			*/ ; return view0.size(); } // cnst	{
tpl_t	sttc	llc::err_t  testViewByte_count		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn u2_t(Count * sizeof(T));	
//tpl_t	sttc	llc::err_t  testViewByte_count		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::view<T> & viewToTest)														*/ ; return view0.size(); } // nxpt	
tpl_t	sttc	llc::err_t  testViewBit_count		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn byte_count() * 8ULL;		
tpl_t	sttc	llc::err_t  testViewC				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // nxpt	{ rtrn {(sc_t*)Data, byte_count(
tpl_t	sttc	llc::err_t  testViewU8				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // nxpt	{ rtrn {(u0_t*)Data, byte_count(
tpl_t	sttc	llc::err_t  testViewCC				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn {(sc_c*)Data, byte_count(
tpl_t	sttc	llc::err_t  testViewCU8				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn {(u0_c*)Data, byte_count(
tpl_t	sttc	llc::err_t  testViewSize			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn Count;			}
//tpl_t	sttc	llc::err_t  testViewSize			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::view<T> & viewToTest)														*/ ; return view0.size(); } // nxpt	
tpl_t	sttc	llc::err_t  testViewBegin			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn Data;			}
//tpl_t	sttc	llc::err_t  testViewBegin			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // nxpt	{ rtrn Data;			}
tpl_t	sttc	llc::err_t  testViewEnd				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // csnx	{ rtrn begin() + Count;	}
//tpl_t	sttc	llc::err_t  testViewEnd				()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } // nxpt	{ rtrn begin() + Count;	}
tpl_t	sttc	llc::err_t  testViewSlice			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(TV & out, u2_t offset, u2_t count = (u2_t)-1)											*/ ; return view0.size(); } //		{
//tpl_t	sttc	llc::err_t  testViewSlice			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(TCstV & out, u2_t offset, u2_t count = (u2_t)-1)										*/ ; return view0.size(); } // cnst	{
tpl_t	sttc	llc::err_t  testViewRevert			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*()																						*/ ; return view0.size(); } //		{
tpl_t	sttc	llc::err_t  testViewFill			()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst T & value, u2_t offset = 0, u2_t stop = 0xFFFFFFFFU)								*/ ; return view0.size(); } //		{ 
tpl_t	sttc	llc::err_t  testViewFor_each		()  { cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncForEach		<T> & funcForEach, u2_t offset = 0)						*/ ; return view0.size(); } //					
//tpl_t sttc	llc::err_t	testViewFor_each		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncForEachConst	<T> & funcForEach, u2_t offset = 0)						*/ ; return view0.size(); } // cnst	
//tpl_t sttc	llc::err_t	testViewFor_each		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncForEach		<T> & funcForEach, u2_t offset, u2_t stop)				*/ ; return view0.size(); } //		
//tpl_t sttc	llc::err_t	testViewFor_each		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncForEachConst	<T> & funcForEach, u2_t offset, u2_t stop)				*/ ; return view0.size(); } // cnst	
tpl_t	sttc	llc::err_t	testViewEnumerate		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncEnumerate	 <T> & funcForEach, u2_t offset = 0)						*/ ; return view0.size(); } //		
//tpl_t sttc	llc::err_t	testViewEnumerate		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncEnumerate	 <T> & funcForEach, u2_t offset, u2_t stop)					*/ ; return view0.size(); } //		
//tpl_t sttc	llc::err_t	testViewEnumerate		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncEnumerateConst<T> & funcForEach, u2_t offset = 0)						*/ ; return view0.size(); } // cnst	
//tpl_t sttc	llc::err_t	testViewEnumerate		()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst ::llc::TFuncEnumerateConst<T> & funcForEach, u2_t offset, u2_t stop)				*/ ; return view0.size(); } // cnst	
tpl_t	sttc	llc::err_t	testViewFind			()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst FBool<T&>		& funcForEach	, u2_t offset = 0)									*/ ; return view0.size(); } //		
//tpl_t sttc	llc::err_t	testViewFind			()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst FBool<TCnst&>	& funcForEach	, u2_t offset = 0)									*/ ; return view0.size(); } // cnst	
//tpl_t sttc	llc::err_t	testViewFind			()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst T				& value			, u2_t offset = 0)									*/ ; return view0.size(); } // cnst	
tpl_t	sttc	llc::err_t	testViewMax				()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(_tMax & maxFound, cnst FTransform<_tMax, TCnst &> & funcComparand, u2_t offset = 0)	*/ ; return view0.size(); } // cnst	
//tpl_t sttc	llc::err_t	testViewMax				()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst FTransform<_tMax, TCnst &> & funcComparand, u2_t offset = 0)						*/ ; return view0.size(); } // cnst	
tpl_t	sttc	llc::err_t	testViewMin				()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(_tMax & minFound, cnst FTransform<_tMax, TCnst &> & funcComparand, u2_t offset = 0)	*/ ; return view0.size(); } // cnst	
//tpl_t sttc	llc::err_t	testViewMin				()	{ cnst _t testData0[] = {1}; llc::view<cnst _t> view0{testData0}; /*(cnst FTransform<_tMax, TCnst &> & funcComparand, u2_t offset = 0)						*/ ; return view0.size(); } // cnst	

sttc	llc::err_t  testViewConstructors	() { 
	if_fail_fe(testViewConstructors<u0_t>()); // 
	if_fail_fe(testViewConstructors<u1_t>()); // 
	if_fail_fe(testViewConstructors<u2_t>()); // 
	if_fail_fe(testViewConstructors<u3_t>()); // 
	if_fail_fe(testViewConstructors<uc_t>()); // 
	if_fail_fe(testViewConstructors<s0_t>()); // 
	if_fail_fe(testViewConstructors<s1_t>()); // 
	if_fail_fe(testViewConstructors<s2_t>()); // 
	if_fail_fe(testViewConstructors<s3_t>()); // 
	if_fail_fe(testViewConstructors<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewCastConst			() { 
	if_fail_fe(testViewCastConst<u0_t>()); // 
	if_fail_fe(testViewCastConst<u1_t>()); // 
	if_fail_fe(testViewCastConst<u2_t>()); // 
	if_fail_fe(testViewCastConst<u3_t>()); // 
	if_fail_fe(testViewCastConst<uc_t>()); // 
	if_fail_fe(testViewCastConst<s0_t>()); // 
	if_fail_fe(testViewCastConst<s1_t>()); // 
	if_fail_fe(testViewCastConst<s2_t>()); // 
	if_fail_fe(testViewCastConst<s3_t>()); // 
	if_fail_fe(testViewCastConst<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewSubscript			() { 
	if_fail_fe(testViewSubscript<u0_t>()); // 
	if_fail_fe(testViewSubscript<u1_t>()); // 
	if_fail_fe(testViewSubscript<u2_t>()); // 
	if_fail_fe(testViewSubscript<u3_t>()); // 
	if_fail_fe(testViewSubscript<uc_t>()); // 
	if_fail_fe(testViewSubscript<s0_t>()); // 
	if_fail_fe(testViewSubscript<s1_t>()); // 
	if_fail_fe(testViewSubscript<s2_t>()); // 
	if_fail_fe(testViewSubscript<s3_t>()); // 
	if_fail_fe(testViewSubscript<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewSubscriptConst		() { 
	if_fail_fe(testViewSubscriptConst<u0_t>()); // 
	if_fail_fe(testViewSubscriptConst<u1_t>()); // 
	if_fail_fe(testViewSubscriptConst<u2_t>()); // 
	if_fail_fe(testViewSubscriptConst<u3_t>()); // 
	if_fail_fe(testViewSubscriptConst<uc_t>()); // 
	if_fail_fe(testViewSubscriptConst<s0_t>()); // 
	if_fail_fe(testViewSubscriptConst<s1_t>()); // 
	if_fail_fe(testViewSubscriptConst<s2_t>()); // 
	if_fail_fe(testViewSubscriptConst<s3_t>()); // 
	if_fail_fe(testViewSubscriptConst<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewOperatorNotEqual	() { 
	if_fail_fe(testViewOperatorNotEqual<u0_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<u1_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<u2_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<u3_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<uc_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<s0_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<s1_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<s2_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<s3_t>()); // 
	if_fail_fe(testViewOperatorNotEqual<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewOperatorEqual		() { 
	if_fail_fe(testViewOperatorEqual<u0_t>()); // 
	if_fail_fe(testViewOperatorEqual<u1_t>()); // 
	if_fail_fe(testViewOperatorEqual<u2_t>()); // 
	if_fail_fe(testViewOperatorEqual<u3_t>()); // 
	if_fail_fe(testViewOperatorEqual<uc_t>()); // 
	if_fail_fe(testViewOperatorEqual<s0_t>()); // 
	if_fail_fe(testViewOperatorEqual<s1_t>()); // 
	if_fail_fe(testViewOperatorEqual<s2_t>()); // 
	if_fail_fe(testViewOperatorEqual<s3_t>()); // 
	if_fail_fe(testViewOperatorEqual<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewByte_count			() { 
	if_fail_fe(testViewByte_count<u0_t>()); // 
	if_fail_fe(testViewByte_count<u1_t>()); // 
	if_fail_fe(testViewByte_count<u2_t>()); // 
	if_fail_fe(testViewByte_count<u3_t>()); // 
	if_fail_fe(testViewByte_count<uc_t>()); // 
	if_fail_fe(testViewByte_count<s0_t>()); // 
	if_fail_fe(testViewByte_count<s1_t>()); // 
	if_fail_fe(testViewByte_count<s2_t>()); // 
	if_fail_fe(testViewByte_count<s3_t>()); // 
	if_fail_fe(testViewByte_count<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewBit_count			() { 
	if_fail_fe(testViewBit_count<u0_t>()); // 
	if_fail_fe(testViewBit_count<u1_t>()); // 
	if_fail_fe(testViewBit_count<u2_t>()); // 
	if_fail_fe(testViewBit_count<u3_t>()); // 
	if_fail_fe(testViewBit_count<uc_t>()); // 
	if_fail_fe(testViewBit_count<s0_t>()); // 
	if_fail_fe(testViewBit_count<s1_t>()); // 
	if_fail_fe(testViewBit_count<s2_t>()); // 
	if_fail_fe(testViewBit_count<s3_t>()); // 
	if_fail_fe(testViewBit_count<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewC					() { 
	if_fail_fe(testViewC<u0_t>()); // 
	if_fail_fe(testViewC<u1_t>()); // 
	if_fail_fe(testViewC<u2_t>()); // 
	if_fail_fe(testViewC<u3_t>()); // 
	if_fail_fe(testViewC<uc_t>()); // 
	if_fail_fe(testViewC<s0_t>()); // 
	if_fail_fe(testViewC<s1_t>()); // 
	if_fail_fe(testViewC<s2_t>()); // 
	if_fail_fe(testViewC<s3_t>()); // 
	if_fail_fe(testViewC<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewU8					() { 
	if_fail_fe(testViewU8<u0_t>()); // 
	if_fail_fe(testViewU8<u1_t>()); // 
	if_fail_fe(testViewU8<u2_t>()); // 
	if_fail_fe(testViewU8<u3_t>()); // 
	if_fail_fe(testViewU8<uc_t>()); // 
	if_fail_fe(testViewU8<s0_t>()); // 
	if_fail_fe(testViewU8<s1_t>()); // 
	if_fail_fe(testViewU8<s2_t>()); // 
	if_fail_fe(testViewU8<s3_t>()); // 
	if_fail_fe(testViewU8<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewCc					() { 
	if_fail_fe(testViewCC<u0_t>()); // 
	if_fail_fe(testViewCC<u1_t>()); // 
	if_fail_fe(testViewCC<u2_t>()); // 
	if_fail_fe(testViewCC<u3_t>()); // 
	if_fail_fe(testViewCC<uc_t>()); // 
	if_fail_fe(testViewCC<s0_t>()); // 
	if_fail_fe(testViewCC<s1_t>()); // 
	if_fail_fe(testViewCC<s2_t>()); // 
	if_fail_fe(testViewCC<s3_t>()); // 
	if_fail_fe(testViewCC<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewCu8					() { 
	if_fail_fe(testViewCU8<u0_t>()); // 
	if_fail_fe(testViewCU8<u1_t>()); // 
	if_fail_fe(testViewCU8<u2_t>()); // 
	if_fail_fe(testViewCU8<u3_t>()); // 
	if_fail_fe(testViewCU8<uc_t>()); // 
	if_fail_fe(testViewCU8<s0_t>()); // 
	if_fail_fe(testViewCU8<s1_t>()); // 
	if_fail_fe(testViewCU8<s2_t>()); // 
	if_fail_fe(testViewCU8<s3_t>()); // 
	if_fail_fe(testViewCU8<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewSize				() { 
	if_fail_fe(testViewSize<u0_t>()); // 
	if_fail_fe(testViewSize<u1_t>()); // 
	if_fail_fe(testViewSize<u2_t>()); // 
	if_fail_fe(testViewSize<u3_t>()); // 
	if_fail_fe(testViewSize<uc_t>()); // 
	if_fail_fe(testViewSize<s0_t>()); // 
	if_fail_fe(testViewSize<s1_t>()); // 
	if_fail_fe(testViewSize<s2_t>()); // 
	if_fail_fe(testViewSize<s3_t>()); // 
	if_fail_fe(testViewSize<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewBegin				() { 
	if_fail_fe(testViewBegin<u0_t>()); // 
	if_fail_fe(testViewBegin<u1_t>()); // 
	if_fail_fe(testViewBegin<u2_t>()); // 
	if_fail_fe(testViewBegin<u3_t>()); // 
	if_fail_fe(testViewBegin<uc_t>()); // 
	if_fail_fe(testViewBegin<s0_t>()); // 
	if_fail_fe(testViewBegin<s1_t>()); // 
	if_fail_fe(testViewBegin<s2_t>()); // 
	if_fail_fe(testViewBegin<s3_t>()); // 
	if_fail_fe(testViewBegin<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewEnd					() { 
	if_fail_fe(testViewEnd<u0_t>()); // 
	if_fail_fe(testViewEnd<u1_t>()); // 
	if_fail_fe(testViewEnd<u2_t>()); // 
	if_fail_fe(testViewEnd<u3_t>()); // 
	if_fail_fe(testViewEnd<uc_t>()); // 
	if_fail_fe(testViewEnd<s0_t>()); // 
	if_fail_fe(testViewEnd<s1_t>()); // 
	if_fail_fe(testViewEnd<s2_t>()); // 
	if_fail_fe(testViewEnd<s3_t>()); // 
	if_fail_fe(testViewEnd<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewSlice				() { 
	if_fail_fe(testViewSlice<u0_t>()); // 
	if_fail_fe(testViewSlice<u1_t>()); // 
	if_fail_fe(testViewSlice<u2_t>()); // 
	if_fail_fe(testViewSlice<u3_t>()); // 
	if_fail_fe(testViewSlice<uc_t>()); // 
	if_fail_fe(testViewSlice<s0_t>()); // 
	if_fail_fe(testViewSlice<s1_t>()); // 
	if_fail_fe(testViewSlice<s2_t>()); // 
	if_fail_fe(testViewSlice<s3_t>()); // 
	if_fail_fe(testViewSlice<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewRevert				() { 
	if_fail_fe(testViewRevert<u0_t>()); // 
	if_fail_fe(testViewRevert<u1_t>()); // 
	if_fail_fe(testViewRevert<u2_t>()); // 
	if_fail_fe(testViewRevert<u3_t>()); // 
	if_fail_fe(testViewRevert<uc_t>()); // 
	if_fail_fe(testViewRevert<s0_t>()); // 
	if_fail_fe(testViewRevert<s1_t>()); // 
	if_fail_fe(testViewRevert<s2_t>()); // 
	if_fail_fe(testViewRevert<s3_t>()); // 
	if_fail_fe(testViewRevert<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewFill				() { 
	if_fail_fe(testViewFill<u0_t>()); // 
	if_fail_fe(testViewFill<u1_t>()); // 
	if_fail_fe(testViewFill<u2_t>()); // 
	if_fail_fe(testViewFill<u3_t>()); // 
	if_fail_fe(testViewFill<uc_t>()); // 
	if_fail_fe(testViewFill<s0_t>()); // 
	if_fail_fe(testViewFill<s1_t>()); // 
	if_fail_fe(testViewFill<s2_t>()); // 
	if_fail_fe(testViewFill<s3_t>()); // 
	if_fail_fe(testViewFill<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewFor_each			() { 
	if_fail_fe(testViewFor_each<u0_t>()); // 
	if_fail_fe(testViewFor_each<u1_t>()); // 
	if_fail_fe(testViewFor_each<u2_t>()); // 
	if_fail_fe(testViewFor_each<u3_t>()); // 
	if_fail_fe(testViewFor_each<uc_t>()); // 
	if_fail_fe(testViewFor_each<s0_t>()); // 
	if_fail_fe(testViewFor_each<s1_t>()); // 
	if_fail_fe(testViewFor_each<s2_t>()); // 
	if_fail_fe(testViewFor_each<s3_t>()); // 
	if_fail_fe(testViewFor_each<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewEnumerate			() { 
	if_fail_fe(testViewEnumerate<u0_t>()); // 
	if_fail_fe(testViewEnumerate<u1_t>()); // 
	if_fail_fe(testViewEnumerate<u2_t>()); // 
	if_fail_fe(testViewEnumerate<u3_t>()); // 
	if_fail_fe(testViewEnumerate<uc_t>()); // 
	if_fail_fe(testViewEnumerate<s0_t>()); // 
	if_fail_fe(testViewEnumerate<s1_t>()); // 
	if_fail_fe(testViewEnumerate<s2_t>()); // 
	if_fail_fe(testViewEnumerate<s3_t>()); // 
	if_fail_fe(testViewEnumerate<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewFind				() { 
	if_fail_fe(testViewFind<u0_t>()); // 
	if_fail_fe(testViewFind<u1_t>()); // 
	if_fail_fe(testViewFind<u2_t>()); // 
	if_fail_fe(testViewFind<u3_t>()); // 
	if_fail_fe(testViewFind<uc_t>()); // 
	if_fail_fe(testViewFind<s0_t>()); // 
	if_fail_fe(testViewFind<s1_t>()); // 
	if_fail_fe(testViewFind<s2_t>()); // 
	if_fail_fe(testViewFind<s3_t>()); // 
	if_fail_fe(testViewFind<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewMax					() { 
	if_fail_fe(testViewMax<u0_t>()); // 
	if_fail_fe(testViewMax<u1_t>()); // 
	if_fail_fe(testViewMax<u2_t>()); // 
	if_fail_fe(testViewMax<u3_t>()); // 
	if_fail_fe(testViewMax<uc_t>()); // 
	if_fail_fe(testViewMax<s0_t>()); // 
	if_fail_fe(testViewMax<s1_t>()); // 
	if_fail_fe(testViewMax<s2_t>()); // 
	if_fail_fe(testViewMax<s3_t>()); // 
	if_fail_fe(testViewMax<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t	testViewMin					() { 
	if_fail_fe(testViewMin<u0_t>()); // 
	if_fail_fe(testViewMin<u1_t>()); // 
	if_fail_fe(testViewMin<u2_t>()); // 
	if_fail_fe(testViewMin<u3_t>()); // 
	if_fail_fe(testViewMin<uc_t>()); // 
	if_fail_fe(testViewMin<s0_t>()); // 
	if_fail_fe(testViewMin<s1_t>()); // 
	if_fail_fe(testViewMin<s2_t>()); // 
	if_fail_fe(testViewMin<s3_t>()); // 
	if_fail_fe(testViewMin<sc_t>()); // 
	//if_fail_fe(testViewConstructors<b8_t>()); // 
	return 0;
}
sttc	llc::err_t  runViewTests				() {
	if_fail_fe(testViewConstructors		());
	if_fail_fe(testViewCastConst		());
	if_fail_fe(testViewSubscript		());
	if_fail_fe(testViewSubscriptConst	());
	if_fail_fe(testViewOperatorNotEqual ());
	if_fail_fe(testViewOperatorEqual	());
	if_fail_fe(testViewByte_count		());
	if_fail_fe(testViewBit_count		());
	if_fail_fe(testViewC				());
	if_fail_fe(testViewU8				());
	if_fail_fe(testViewCc				());
	if_fail_fe(testViewU8				());
	if_fail_fe(testViewCu8				());
	if_fail_fe(testViewSize				());
	if_fail_fe(testViewBegin			());
	if_fail_fe(testViewEnd				());
	if_fail_fe(testViewSlice			());
	if_fail_fe(testViewRevert			());
	if_fail_fe(testViewFill				());
	if_fail_fe(testViewFor_each			());
	if_fail_fe(testViewEnumerate		());
	if_fail_fe(testViewFind				());
	if_fail_fe(testViewMax				());
	if_fail_fe(testViewMin				());
	return 0;
}

int WINAPI WinMain 
	( _In_		HINSTANCE	// hInstance
	, _In_opt_	HINSTANCE	// hPrevInstance
	, _In_		LPSTR		// lpCmdLine
	, _In_		int			// nShowCmd
	) {
	return 0;
}


int main() {
	info_printf("# Testing view<> struct");
	runViewTests();
	return 0;
}

