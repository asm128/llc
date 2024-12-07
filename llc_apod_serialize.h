#include "llc_array_pod.h"

#include "llc_keyval.h"

#include "llc_view_serialize.h"

#ifndef LLC_APOD_SERIALIZE_H_23627
#define LLC_APOD_SERIALIZE_H_23627

namespace llc
{
	tplT	err_t			saveView			(au0_t & output, cnst view<T> & viewToSerialize)	{
		cnst packedu32				counterValue		= viewToSerialize.size();
		llc_necs(output.append(counterValue.tplt cu8<vcu0_t>()));
		llc_necs(output.append(viewToSerialize.cu8()));
		return counterValue.ValueWidth() + viewToSerialize.byte_count();
	}
	tplT	err_t			saveUIntView		(au0_t & output, cnst view<T> & viewToSerialize)	{
		u2_c						offset				= output.size();
		cnst packedu32				counterValue		= viewToSerialize.size();
		llc_necs(output.append(counterValue.tplt cu8<vcu0_t>()));
		for(cnst packed_uint<T> valpkd : viewToSerialize)
			llc_necs(output.append(valpkd.tplt cu8<vcu0_t>()));
		return output.size() - offset;
	}
	tplt<>	inln	err_t	saveView	<u1_t>	(au0_t & output, vu1_c  & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }
	tplt<>	inln	err_t	saveView	<u2_t>	(au0_t & output, vu2_c  & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }
	tplt<>	inln	err_t	saveView	<u3_t>	(au0_t & output, vu3_c  & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }
	tplt<>	inln	err_t	saveView	<u1_c>	(au0_t & output, vcu1_c & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }
	tplt<>	inln	err_t	saveView	<u2_c>	(au0_t & output, vcu2_c & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }
	tplt<>	inln	err_t	saveView	<u3_c>	(au0_t & output, vcu3_c & viewToSerialize)		{ return saveUIntView(output, viewToSerialize); }

	tplTstin		err_t	saveView			(as0_t & output, cnst view<T> & headerToWrite)	{ return saveView(*(au0_t*)&output, headerToWrite); }
	tplT			err_t	savePOD				(au0_t & output, cnst T & input)	{ llc_necs(output.append((cnst uint8_t*)&input, szof(T))); return szof(T); }
	tplTstin		err_t	savePOD				(as0_t & output, cnst T & input)	{ return savePOD(*(au0_t*)&output, input); }
	tplT			err_t	saveUInt			(au0_t & output, cnst T & input)	{ 
		cnst packed_uint<T>			packedInput			= input; 
		llc_necs(output.append(packedInput.cu8())); 
		return packedInput.ValueWidth(); 
	}
	tplT			err_t			loadView			(vcu0_t & input, apod<T> & output) { 
		view<cnst T>					readView			= {}; 
		uint32_t						bytesRead			= 0;
		llc_necs(bytesRead = viewRead(readView, input)); 
		llc_necs(input.slice(input, bytesRead));
		output						= readView;// llc_necs(output.append(readView)); 
		return 0;
	}
	tplTstin err_t	loadView			(vcs0_t & input, apod<T> & output) { return loadView(*(vcu0_t*)& input, output); }
	tplTstin err_t	loadView			(vcsc_t  & input, apod<T> & output) { return loadView(*(vcu0_t*)& input, output); }

	err_t						keyValConstStringSerialize	(cnst view<cnst TKeyValConstChar> & keyVals, vcvsc_c & keysToSave, au0_t & output);
} // namespace

#endif // LLC_APOD_SERIALIZE_H_23627
