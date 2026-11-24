#include "llc_array_pod.h"
#include "llc_view_serialize.h"

#ifndef LLC_APOD_SERIALIZE_H_23627
#define LLC_APOD_SERIALIZE_H_23627

namespace llc
{
	tplt<tpnm TOutput, tpnm T, u0_t widthField>
	err_t					savePacked					(TOutput & output, cnst packed_uint<T, widthField> & packedInput) {
		u2_c						offset						= output.size();
		if_fail_fe(output.resize(offset + packedInput.ValueWidth()));
		memcpy(&output[offset], &packedInput, packedInput.ValueWidth());
		return packedInput.ValueWidth();
	}

	tplT			err_t	saveView					(au0_t & output, cnst view<T> & viewToSerialize)	{
		cnst packedu32				counterValue				= viewToSerialize.size();
		if_fail_fe(savePacked(output, counterValue));
		if_fail_fe(output.append(viewToSerialize.cu8()));
		return counterValue.ValueWidth() + viewToSerialize.byte_count();
	}
	//tplT			err_t	saveUIntView		(au0_t & output, cnst view<T> & viewToSerialize)	{
	//	u2_c						offset				= output.size();
	//	cnst packedu32				counterValue		= viewToSerialize.size();
	//	if_fail_fe(output.append(counterValue.tplt cu8<vcu0_t>()));
	//	for(cnst packed_uint<T> valpkd : viewToSerialize)
	//		if_fail_fe(output.append(valpkd.tplt cu8<vcu0_t>()));
	//	return output.size() - offset;
	//}
	//tplt<>	inln	err_t	saveView	<u1_t>	(au0_t & output, vu1_c  & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }
	//tplt<>	inln	err_t	saveView	<u2_t>	(au0_t & output, vu2_c  & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }
	//tplt<>	inln	err_t	saveView	<u3_t>	(au0_t & output, vu3_c  & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }
	//tplt<>	inln	err_t	saveView	<u1_c>	(au0_t & output, vcu1_c & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }
	//tplt<>	inln	err_t	saveView	<u2_c>	(au0_t & output, vcu2_c & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }
	//tplt<>	inln	err_t	saveView	<u3_c>	(au0_t & output, vcu3_c & viewToSerialize)			{ return saveUIntView(output, viewToSerialize); }

	tplTstin		err_t	saveView					(as0_t & output, cnst view<T> & headerToWrite)	{ return saveView(*(au0_t*)&output, headerToWrite); }
	tplT			err_t	savePOD						(au0_t & output, cnst T & input)				{ if_fail_fe(output.append((cnst u0_t*)&input, szof(T))); return szof(T); }
	tplTstin		err_t	savePOD						(as0_t & output, cnst T & input)				{ return savePOD(*(au0_t*)&output, input); }
	tplt<tpnm TMap, tpnm TSave>
	err_t					saveMap						(au0_t & output, cnst TMap & map, TSave save) {
		u2_c						count						= map.size();
		if_fail_fe(savePOD(output, count));
		for(u2_t index = 0; index < count; ++index)
			if_fail_fe(save(output, index, map[index]));
		return 0;
	}
	tplTstin	err_t	save		(au0_t & output, cnst T & input)						{ rtrn savePOD(output, input); }
	tplTstin	err_t	save		(au0_t & output, cnst view<cnst T> & input)		{ rtrn saveView(output, input); }
	tplTstin	err_t	save		(au0_t & output, cnst apod<T> & input)			{ rtrn saveView(output, input); }
	tplTstin	err_t	save		(au0_t & output, cnst view<cnst view<T>> & input) {
		u2_c oldSize = output.size();
		if_fail_fe(input.for_each([&](cnst view<T> & entry) { rtrn saveView(output, entry); }));
		rtrn output.size() - oldSize;
	}
	tplTstin	err_t	save		(au0_t & output, cnst view<cnst view<cnst T>> & input) {
		u2_c oldSize = output.size();
		if_fail_fe(input.for_each([&](cnst view<cnst T> & entry) { rtrn saveView(output, entry); }));
		rtrn output.size() - oldSize;
	}
	tpl_vtArgs requires(sizeof...(_tArgs) > 1)
	err_t	save		(au0_t & output, _tArgs&&... args) {
		err_t err = 0;
		cnst s2_t results[] = {(err = failed(err) ? err : save(output, args))..., 0};
		rtrn failed(err) ? err : sum(vcs2_t{results});
	}
	tplT			err_t	saveUInt					(au0_t & output, cnst T & input)				{ 
		cnst packed_uint<T>			packedInput					= input; 
		return savePacked(output, packedInput); 
	}
	tplT			err_t	loadView					(vcu0_t & input, apod<T> & output) { 
		view<cnst T>				readView					= {}; 
		err_t						bytesRead					= 0;
		if_fail_fe(bytesRead = viewRead(readView, input)); 
		if_fail_fe(input.slice(input, bytesRead));
		output					= readView;// if_true_fe(output.append(readView)); 
		return 0;
	}
	tplTstin		err_t	loadView					(vcs0_t & input, apod<T> & output)				{ return loadView(*(vcu0_t*)& input, output); }
	tplTInTOut err_t load(view<cnst TIn> & input, apod<TOut> & output) { rtrn loadView(input, output); }
	tplTInTArgs requires(sizeof...(TArgs) > 1)
	err_t load(view<cnst TIn> & input, TArgs &... output) {
		err_t result = 0;
		((result = failed(result) ? result : load(input, output)), ...);
		rtrn failed(result) ? result : 0;
	}

	//err_t					keyValConstStringSerialize	(view<cnst keyval<vcst_t>> keyVals, view<cnst vcst_t> keysToSave, au0_t & output);
} // namespace

#endif // LLC_APOD_SERIALIZE_H_23627
