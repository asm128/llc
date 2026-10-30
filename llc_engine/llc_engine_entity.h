#include "llc_array_ptr.h"
#include "llc_label.h"
#include "llc_string.h"

#include "llc_apod_serialize.h"

#ifndef LLC_ENGINE_ENTITY_H
#define LLC_ENGINE_ENTITY_H

namespace llc 
{
#pragma pack(push, 1)
	typedef	::llc::u2_t		eid_t;
	stxp	eid_t			EID_INVALID		= eid_t(-1);

	typedef	const eid_t		ceid_t;
	typedef	apod<eid_t>		aeid;
	typedef	view<eid_t>		veid;
	typedef	view<ceid_t>	vceid;
	typedef	papod<eid_t>	paeid;
	struct SEntity {
		::llc::u2_t				RenderNode		= (::llc::u2_t)-1;
		::llc::u2_t				RigidBody		= (::llc::u2_t)-1;
		::llc::eid_t			Parent			= EID_INVALID;
	};
#pragma pack(pop)

	struct SVirtualEntityManager {
		::llc::apod<SEntity>			Entities		= {};
		::llc::aobj<vcst_t>				Names			= {};

		::llc::aobj<::llc::paeid>		Children		= {};

		const ::llc::SEntity&			operator[]		(::llc::eid_t id)	const	{ return Entities[id]; }
		SEntity&						operator[]		(::llc::eid_t id)			{ return Entities[id]; }

		::llc::u2_t						size			()					const	{ return Names.size(); }
		::llc::u2_t						clear			()							{ ::llc::clear(Entities, Names, Children); return 0; }

		::llc::err_t					Create			() {
			Children.push_back({});
			Names.push_back({});
			return Entities.push_back({});
		}

		::llc::err_t					Delete			(::llc::eid_t id) {
			Children.remove_unordered(id);
			Names.remove_unordered(id);
			return Entities.remove_unordered(id);
		}

		inline	::llc::err_t			GetIndexParent	(::llc::eid_t id)					const	{ return Entities[id].Parent; }
		inline	::llc::err_t			GetIndexChild	(::llc::eid_t id, s2_t iChild)	const	{ return (*Children[id])[iChild]; }
		inline	const ::llc::SEntity&	GetParent		(::llc::eid_t id)					const	{ return Entities[GetIndexParent(id)]; }
		inline	const ::llc::SEntity&	GetChild		(::llc::eid_t id, s2_t iChild)	const	{ return Entities[GetIndexChild	(id, iChild)]; }
		inline	SEntity&				GetParent		(::llc::eid_t id)							{ return Entities[GetIndexParent(id)]; }
		inline	SEntity&				GetChild		(::llc::eid_t id, s2_t iChild)			{ return Entities[GetIndexChild	(id, iChild)]; }
		inline	::llc::err_t			GetChildCount	(::llc::u2_t iEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? children->size() : 0; }
		inline	::llc::vceid			GetChildren		(::llc::u2_t iEntity)					const	{ const ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? ::llc::vceid{*children} : ::llc::vceid{}; }
		inline	::llc::veid				GetChildren		(::llc::u2_t iEntity)							{ ::llc::papod<::llc::eid_t> & children = Children[iEntity]; return children ? *children : ::llc::veid{}; }


		::llc::err_t					SetParent		(::llc::eid_t id, ::llc::eid_t idParent)		{
			if_true_fef(idParent != ::llc::EID_INVALID && idParent >= Entities.size(), "Invalid parent for entity %i, %i", id, idParent);
			::llc::eid_t						& oldParent		= Entities[id].Parent;
			if(oldParent < Entities.size() && Children[oldParent]) {
				const s2_t						oldIndex		= Children[oldParent]->find(id);
				if(oldIndex >= 0)
					if_fail_fef(Children[oldParent]->remove(oldIndex), "oldParent: %i, oldIndex: %i.", oldParent, oldIndex);
			}
			oldParent						= idParent;
			return (0 > (s2_t)idParent) ? 0 : Children[idParent]->push_back(id);
		}

		::llc::err_t					Save			(::llc::au0_t & output) const { 
			if_fail_fe(llc::saveView(output, Entities));
			for(::llc::u2_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				if_fail_fef(llc::saveView(output, Children[iEntity] ? ::llc::vcu2_t{*Children[iEntity]} : ::llc::vcu2_t{}), "iEntity: %i", iEntity);
				if_fail_fef(llc::saveView(output, Names[iEntity]), "iEntity: %i", iEntity);
			}
			info_printf("Saved %s, %i", "Entities", Children.size());
			return 0;
		}

		::llc::err_t					Load			(::llc::vcu0_t & input) {
			if_fail_fe(llc::loadView(input, Entities));
			if_fail_fe(Children	.resize(Entities.size()));
			if_fail_fe(Names		.resize(Entities.size()));
			for(::llc::u2_t iEntity = 0; iEntity < Entities.size(); ++iEntity) {
				if(!Children[iEntity])
					Children[iEntity].create();

				if_fail_fef(llc::loadView(input, *Children[iEntity]), "iEntity: %i", iEntity);
				::llc::string					name;
				if_fail_fef(llc::loadView(input, name), "iEntity: %i", iEntity);
				Names[iEntity]					= ::llc::label(name);
			}

			return 0; 
		}

	};
} // namespace

#endif // LLC_ENGINE_ENTITY_H
