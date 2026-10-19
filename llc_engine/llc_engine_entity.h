#include "llc_array_ptr.h"
#include "llc_label.h"

#include "llc_apod_serialize.h"

#ifndef LLC_ENGINE_ENTITY_H
#define LLC_ENGINE_ENTITY_H

namespace llc 
{
#pragma pack(push, 1)
	typedef	uint32_t		eid_t;
	stacxpr	eid_t			EID_INVALID		= eid_t(-1);

	typedef	const eid_t		ceid_t;
	typedef	apod<eid_t>		aeid;
	typedef	view<eid_t>		veid;
	typedef	view<ceid_t>	vceid;
	typedef	papod<eid_t>	paeid;
	struct SEntity {
		uint32_t				RenderNode		= (uint32_t)-1;
		uint32_t				RigidBody		= (uint32_t)-1;
		::llc::eid_t			Parent			= EID_INVALID;
	};
#pragma pack(pop)

	struct SVirtualEntityManager {
		::llc::apod<SEntity>			Entities		= {};
		::llc::avcsc_t					Names			= {};

		::llc::aobj<::llc::paeid>		Children		= {};

		const ::llc::SEntity&			operator[]		(::llc::eid_t id)	const	{ return Entities[id]; }
		SEntity&						operator[]		(::llc::eid_t id)			{ return Entities[id]; }

		uint32_t						size			()					const	{ return Names.size(); }
		uint32_t						clear			()							{ ::llc::clear(Entities, Names, Children); return 0; }

		::llc::error_t					Create			() {
			Children.push_back({});
			Names.push_back({});
			return Entities.push_back({});
		}

		::llc::error_t					Delete			(::llc::eid_t id) {
			Children.remove_unordered(id);
			Names.remove_unordered(id);
			return Entities.remove_unordered(id);
		}

		inline	::llc::error_t			GetIndexParent	(::llc::eid_t id)					const	{ return Entities[id].Parent; }
		inline	::llc::error_t			GetIndexChild	(::llc::eid_t id, int32_t iChild)	const	{ return (*Children[id])[iChild]; }
		inline	const ::llc::SEntity&	GetParent		(::llc::eid_t id)					const	{ return Entities[GetIndexParent(id)]; }
		inline	const ::llc::SEntity&	GetChild		(::llc::eid_t id, int32_t iChild)	const	{ return Entities[GetIndexChild	(id, iChild)]; }
		inline	SEntity&				GetParent		(::llc::eid_t id)							{ return Entities[GetIndexParent(id)]; }
		inline	SEntity&				GetChild		(::llc::eid_t id, int32_t iChild)			{ return Entities[GetIndexChild	(id, iChild)]; }
		inline	::llc::error_t			GetChildCount	(uint32_t iEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? children->size() : 0; }
		inline	::llc::vceid			GetChildren		(uint32_t iEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? ::llc::vceid{*children} : ::llc::vceid{}; }
		inline	::llc::veid				GetChildren		(uint32_t iEntity)							{ ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? *children : ::llc::veid{}; }


		::llc::error_t					SetParent		(::llc::eid_t id, ::llc::eid_t idParent)		{
			if_true_fef(idParent != ::llc::EID_INVALID && idParent >= Entities.size(), "Invalid parent for entity %i, %i", id, idParent);
			::llc::eid_t						& oldParent		= Entities[id].Parent;
			if(oldParent < Entities.size() && Children[oldParent]) {
				const int32_t						oldIndex		= Children[oldParent]->find(id);
				if(oldIndex >= 0)
					llc_necall(Children[oldParent]->remove(oldIndex), "oldParent: %i, oldIndex: %i.", oldParent, oldIndex);
			}
			oldParent						= idParent;
			return (0 > (int32_t)idParent) ? 0 : Children[idParent]->push_back(id);
		}

		::llc::error_t					Save			(::llc::au0_t & output) const { 
			llc_necs(llc::saveView(output, Entities));
			for(uint32_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				llc_necall(llc::saveView(output, Children[iEntity] ? ::llc::vcu32{*Children[iEntity]} : ::llc::vcu32{}), "iEntity: %i", iEntity);
				llc_necall(llc::saveView(output, Names[iEntity]), "iEntity: %i", iEntity);
			}
			info_printf("Saved %s, %i", "Entities", Children.size());
			return 0;
		}

		::llc::error_t					Load			(::llc::vcu0_t & input) {
			llc_necs(llc::loadView(input, Entities));
			llc_necs(Children	.resize(Entities.size()));
			llc_necs(Names		.resize(Entities.size()));
			for(uint32_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				if(!Children[iEntity])
					Children[iEntity].create();

				llc_necall(llc::loadView(input, *Children[iEntity]), "iEntity: %i", iEntity);
				::llc::apod<sc_t>					name;
				llc_necall(llc::loadView(input, name), "iEntity: %i", iEntity);
				Names[iEntity]					= ::llc::label(::llc::vcst_t{name});
			}

			return 0; 
		}

	};
} // namespace

#endif // LLC_ENGINE_ENTITY_H
