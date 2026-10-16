/// Copyright 2016-2017 - asm128
#include "llc_type_identifier.h"
#include "llc_datatype.h"
#include "llc_view.h"
#include "llc_module.h"

#ifndef LLC_MEMBER_REGISTRY_H_23627
#define LLC_MEMBER_REGISTRY_H_23627

#define LLCM_REGISTRY_NAME		llc_member_registry
#define LLCM_NAME(_memberName)	_llc_info_##_memberName##_	// This mangles the member name for use as the type name.

#define LLCM(_nameSpace, _memberType, _memberName, _dataTypeId, _displayName, _memberDescription)				\
	struct LLCM_NAME(_memberName)	{						\
		typedef _nameSpace::_memberType			TMember;	\
		stincxp	const sc_t*										get_member_namespace			()							noexcept	{ return #_nameSpace;						}	\
		stincxp	const sc_t*										get_member_type_name			()							noexcept	{ return #_memberType;						}	\
		stincxp	const sc_t*										get_member_name					()							noexcept	{ return #_memberName;						}	\
		stincxp	const sc_t*										get_member_display_name			()							noexcept	{ return _displayName;						}	\
		stincxp	const sc_t*										get_member_description			()							noexcept	{ return _memberDescription;				}	\
		stincxp	::llc::DATA_TYPE									get_member_data_type_id			()							noexcept	{ return _dataTypeId;						}	\
		stincxp	::llc::u2_t											get_member_size					()							noexcept	{ return sizeof(_nameSpace::_memberType);	}	\
	};												\
	_nameSpace::_memberType		_memberName

#define LLCFM(_nameSpace, _memberType, _memberName, _dataTypeId, _displayName, _memberDescription)				\
	struct LLCM_NAME(_memberName)	{						\
		typedef LLC_MODULE_FUNCTION_NAME(_memberType)		TMember;	\
		stincxp	const sc_t*										get_member_namespace			()							noexcept	{ return #_nameSpace;						}	\
		stincxp	const sc_t*										get_member_type_name			()							noexcept	{ return #_memberType;						}	\
		stincxp	const sc_t*										get_member_name					()							noexcept	{ return #_memberName;						}	\
		stincxp	const sc_t*										get_member_display_name			()							noexcept	{ return _displayName;						}	\
		stincxp	const sc_t*										get_member_description			()							noexcept	{ return _memberDescription;				}	\
		stincxp	::llc::DATA_TYPE									get_member_data_type_id			()							noexcept	{ return _dataTypeId;						}	\
		stincxp	::llc::u2_t											get_member_size					()							noexcept	{ return sizeof(LLC_MODULE_FUNCTION_NAME(_memberType));	}	\
	};												\
	LLC_MODULE_FUNCTION_NAME(_memberType)		_memberName

namespace llc {
#pragma pack(push, 1)
	tplt <tpnm... _tArgs>	struct member_registry {
		stincxp	const ::llc::u2_t						get_member_count				()							noexcept	{ return (::llc::u2_t)sizeof...(_tArgs); }
		stainli	const view<const STypeIdentifier	>&	get_types						()										{ static const STypeIdentifier	results[] = {{{_tArgs::get_member_namespace(), (::llc::u2_t)-1}, {_tArgs::get_member_type_name(), (::llc::u2_t)-1}}...,	{}}; static const view<const STypeIdentifier	> results_view = {get_member_count(), results}; return results_view; }
		stainli	const view<const label			>&	get_names						()										{ static const label				results[] = {{_tArgs::get_member_name			(), (::llc::u2_t)-1}...,	""}; static const view<const label			> results_view = {get_member_count(), results}; return results_view; }
		stainli	const view<const label			>&	get_display_names				()										{ static const label				results[] = {{_tArgs::get_member_display_name	(), (::llc::u2_t)-1}...,	""}; static const view<const label			> results_view = {get_member_count(), results}; return results_view; }
		stainli	const view<const label			>&	get_descriptions				()										{ static const label				results[] = {{_tArgs::get_member_description	(), (::llc::u2_t)-1}...,	""}; static const view<const label			> results_view = {get_member_count(), results}; return results_view; }
		stainli	const view<const DATA_TYPE		>	get_data_type_ids				()							noexcept	{ static const DATA_TYPE			results[] = {_tArgs::get_member_data_type_id	()...,	DATA_TYPE_UNKNOWN}; return {get_member_count(), results}; }
		stainli	const view<const ::llc::u2_t		>&	get_member_sizes				()							noexcept	{ static const ::llc::u2_t				results[] = {(::llc::u2_t)_tArgs::get_member_size	()...,	0}; static const view<const ::llc::u2_t				> results_view = {get_member_count(), results}; return results_view; }
	}; // struct
#	define LLCM_NAMED_REGISTRY(registryName, ...)	typedef ::llc::member_registry<__VA_ARGS__> TRegistry;		static	const TRegistry&	get_member_registry	()	{ static const TRegistry registryName; return registryName;	}

	tplt<tpnm _tStruct>
	inline	auto &						get_member_registry()	{ return _tStruct::get_member_registry(); }
#pragma pack(pop)
} // namespace

#define	LLCMFND(_nameSpace, _memberType, _memberName)				LLCFM(_nameSpace, _memberType, _memberName, ::llc::DATA_TYPE_FUN	, #_memberName, #_memberName)
#define	LLCMOND(_nameSpace, _memberType, _memberName)				LLCM(_nameSpace, _memberType, _memberName, ::llc::DATA_TYPE_OBJ		, #_memberName, #_memberName)
#define	LLCMBND(_nameSpace, _memberType, _memberName, _dataTypeId)	LLCM(_nameSpace, _memberType, _memberName, _dataTypeId	, #_memberName, #_memberName)
#define LLCM_REGISTRY(...)											LLCM_NAMED_REGISTRY(LLCM_REGISTRY_NAME, __VA_ARGS__)
#define LLCM_GET_MEMBER_REGISTRY(objectType)						objectType::get_member_registry()

#endif // LLC_MEMBER_REGISTRY_H_23627
