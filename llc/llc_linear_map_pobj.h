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

		inline	const TPObj&		operator[]		(uint32_t index)	const	{ return Elements[index]; }
		inline	TPObj&				operator[]		(uint32_t index)			{ return Elements[index]; }
		inline	const TPObj&		operator[]		(::llc::vcst_t key)	const	{ return Elements[Names.find(key)]; }
		inline	TPObj&				operator[]		(::llc::vcst_t key)			{ return Elements[Names.find(key)]; }

		inlcxpr	uint32_t			size			()											const	{ return Names.size(); }
		inline	::llc::error_t		find			(::llc::vcst_t key, uint32_t offset = 0)	const	{ return Names.find(key, offset); }

		::llc::error_t				push_back		(const ::llc::pobj<T> & instance, ::llc::vcst_t name = {})	{
			::llc::err_t				index			= 0;
			llc_necs(index = Names.push_back(name));
			cnst ::llc::err_t		elementIndex	= Elements.push_back(instance);
			if(::llc::failed(elementIndex)) {
				Names.pop_back();
				return elementIndex;
			}
			return index;
		}

		inline	::llc::error_t		Create			(const ::llc::vcst_t name = {})									{ return push_back({}, name); }
		::llc::error_t				Delete			(uint32_t index)												{
			if_true_fef(index >= size(), "Invalid index:%u. Element count:%u.", index, size());
			llc_necs(Names.remove_unordered(index));
			return Elements.remove_unordered(index);
		}

		::llc::error_t				Clone			(uint32_t index)												{
			if_true_fef(index >= size(), "Invalid index:%u. Element count:%u.", index, size());
			int32_t							iNew;
			llc_necall(iNew = Create(Names[index]), "index: %i", index);
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

		::llc::error_t				Save			(::llc::au0_t & output) const {
			if_true_fef(Names.size() != Elements.size(), "Name count:%u. Element count:%u.", Names.size(), Elements.size());
			::llc::au0_t				serialized		= {};
			cnst ::llc::u2_t		elementCount	= Elements.size();
			llc_necs(::llc::savePOD(serialized, elementCount));
			for(uint32_t iEntity = 0; iEntity < Elements.size(); ++iEntity) {
				if_true_fef(0 == Elements[iEntity], "Null element at index:%u.", iEntity);
				llc_necall(Elements[iEntity]->Save(serialized), "iEntity: %i", iEntity);
				llc_necall(llc::saveView(serialized, Names[iEntity]), "iEntity: %i", iEntity);
			}
			llc_necs(output.append(serialized));
			info_printf("Saved %s, %i", "Elements", Elements.size());
			return 0;
		}

		::llc::error_t				Load			(::llc::vcu0_t & input) {
			::llc::vcu0_t			inputToRead		= input;
			::llc::u2_t			elementCount	= {};
			llc_necs(::llc::loadPOD(inputToRead, elementCount));
			::llc::aobj<TPObj>		loadedElements	= {};
			::llc::avcsc_t			loadedNames		= {};
			for(uint32_t iEntity = 0; iEntity < elementCount; ++iEntity) {
				TPObj						element			= {};
				if_true_fef(0 == element.create(), "Failed to create element at index:%u.", iEntity);
				llc_necall(element->Load(inputToRead), "iEntity: %i", iEntity);
				::llc::vcst_t			name				= {};
				llc_necall(llc::loadLabel(inputToRead, name), "iEntity: %i", iEntity);
				llc_necs(loadedElements.push_back(element));
				llc_necs(loadedNames.push_back(name));
			}
			Elements				= loadedElements;
			Names					= loadedNames;
			input					= inputToRead;
			return 0;
		}
	};

} // namespace

#endif // LLC_LINEAR_MAP_POBJ_H_23627
