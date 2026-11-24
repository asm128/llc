#include "llc_array_ptr.h"
#include "llc_label.h"
#include "llc_apod_serialize.h"

#ifndef LLC_LINEAR_MAP_POBJ_H_23627
#define LLC_LINEAR_MAP_POBJ_H_23627

namespace llc
{
	tplt <tpnm _tElementWithSaveLoad>
	struct llc_linear_map_pobj {
		typedef _tElementWithSaveLoad	T;
		typedef ::llc::pobj<T>			TPObj;

		::llc::aobj<TPObj>			Elements		= {};
		::llc::avcsc_t				Names			= {};

		inline	const TPObj&		operator[]		(::llc::u2_t index)	const	{ return Elements[index]; }
		inline	TPObj&				operator[]		(::llc::u2_t index)			{ return Elements[index]; }
		inline	const TPObj&		operator[]		(::llc::vcst_t key)	const	{ return Elements[Names.find(key)]; }
		inline	TPObj&				operator[]		(::llc::vcst_t key)			{ return Elements[Names.find(key)]; }

		inxp	::llc::u2_t			size			()											const	{ return Names.size(); }
		inline	::llc::err_t		find			(::llc::vcst_t key, ::llc::u2_t offset = 0)	const	{ return Names.find(key, offset); }

		::llc::err_t				push_back		(const ::llc::pobj<T> & instance, ::llc::vcst_t name = {})	{
			::llc::err_t				index			= 0;
			if_fail_fe(index = Names.push_back(name));
			cnst ::llc::err_t		elementIndex	= Elements.push_back(instance);
			if(::llc::failed(elementIndex)) {
				Names.pop_back();
				return elementIndex;
			}
			return index;
		}

		inline	::llc::err_t		Create			(const ::llc::vcst_t name = {})									{ return push_back({}, name); }
		::llc::err_t				Delete			(::llc::u2_t index)												{
			if_true_fef(index >= size(), "Invalid index:%u. Element count:%u.", index, size());
			if_fail_fe(Names.remove_unordered(index));
			return Elements.remove_unordered(index);
		}

		::llc::err_t				Clone			(::llc::u2_t index)												{
			if_true_fef(index >= size(), "Invalid index:%u. Element count:%u.", index, size());
			int32_t							iNew;
			if_fail_fef(iNew = Create(Names[index]), "index: %i", index);
			const TPObj						& srcElement	= Elements[index];
			if(srcElement) {
				TPObj							& newElement	= Elements[iNew];
				if(!newElement && 0 == newElement.create()) {
					Delete(iNew);
					return -1;
				}
				*newElement					= *srcElement;
			}
			return iNew;
		}

		::llc::err_t				Save			(::llc::au0_t & output) const {
			if_true_fef(Names.size() != Elements.size(), "Name count:%u. Element count:%u.", Names.size(), Elements.size());
			::llc::au0_t				serialized		= {};
			cnst ::llc::u2_t		elementCount	= Elements.size();
			if_fail_fe(::llc::savePOD(serialized, elementCount));
			for(::llc::u2_t iEntity = 0; iEntity < Elements.size(); ++iEntity) {
				if_true_fef(0 == Elements[iEntity], "Null element at index:%u.", iEntity);
				if_fail_fef(Elements[iEntity]->Save(serialized), "iEntity: %i", iEntity);
				if_fail_fef(llc::saveView(serialized, Names[iEntity]), "iEntity: %i", iEntity);
			}
			if_fail_fe(output.append(serialized));
			info_printf("Saved %s, %i", "Elements", Elements.size());
			return 0;
		}

		::llc::err_t				Load			(::llc::vcu0_t & input) {
			::llc::vcu0_t			inputToRead		= input;
			::llc::u2_t			elementCount	= {};
			if_fail_fe(::llc::loadPOD(inputToRead, elementCount));
			::llc::aobj<TPObj>		loadedElements	= {};
			::llc::avcsc_t			loadedNames		= {};
			for(::llc::u2_t iEntity = 0; iEntity < elementCount; ++iEntity) {
				TPObj						element			= {};
				if_true_fef(0 == element.create(), "Failed to create element at index:%u.", iEntity);
				if_fail_fef(element->Load(inputToRead), "iEntity: %i", iEntity);
				::llc::vcst_t			name				= {};
				if_fail_fef(llc::loadLabel(inputToRead, name), "iEntity: %i", iEntity);
				if_fail_fe(loadedElements.push_back(element));
				if_fail_fe(loadedNames.push_back(name));
			}
			Elements				= loadedElements;
			Names					= loadedNames;
			input					= inputToRead;
			return 0;
		}
	};

} // namespace

#endif // LLC_LINEAR_MAP_POBJ_H_23627
