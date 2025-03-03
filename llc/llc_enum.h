// These sources are best viewed in Visual Studio 2017 or newer with a screen of at least 1920x1080 screen, Consolas font and zoom set to 81 %.
// Ogarnd - Copyright (c) 2016-2024
#include "llc_array_pod.h"
#include "llc_array_obj.h"

#ifndef LLC_ENUM_H_23627
#define LLC_ENUM_H_23627

#define enum_printf verbose_printf

namespace llc
{
	//stxp	vcs				UNDEFINED_ENUM_TYPE_STR					= LLC_CXS("Undefined enumeration type.");
	stxp	vcs				INVALID_ENUM_VALUE_STR		= LLC_CXS("INVALID");
	stxp	vcs				UNDEFINED_ENUM_VALUE_STR	= LLC_CXS("Undefined enumeration value.");
	stxp	vcs				UNRESOLVED_ENUM_LABEL_STR	= LLC_CXS("Unresolved enumeration value name.");
	stxp	vcs				UNRESOLVED_ENUM_NAME_STR	= LLC_CXS("Enum definition name not set.");

	// This tplt is intended to store the name of an enumeration, the values of such enumeration and a string representing each value.
	// The implementation separates names from values for improving search speed by reducing the memory usage when performing searches for names/values.
	tpl_tstct enum_definition {
		tdfT(_t);
		stxp	T					INVALID_VALUE			= (T)(-1);

		::llc::vcsc_t					Name					= UNRESOLVED_ENUM_NAME_STR;
		::llc::apod<T>					Values					= {};
		::llc::avcsc_t					Names					= {};
		::llc::avcsc_t					Titles					= {};
		::llc::avcsc_t					Descriptions			= {};

		stin	enum_definition<T>&	get				()											{
			static	enum_definition<T>		valueRegistry;
			return valueRegistry;
		}
		stin	T					init					(::llc::vcsc_c & enumName)						{
			enum_definition<T>&				instanceHere			= get();

			if( instanceHere.Name != enumName || (instanceHere.Values.size() && (instanceHere.Values[0] != INVALID_VALUE)) )
				verbose_printf("Initializing enumeration type: '%s'.", enumName.begin());

			if(instanceHere.Name.size() != enumName.size())
				instanceHere.Name			= enumName;

			return INVALID_VALUE;
		}
		err_t				get_value				(::llc::vcsc_c & name, T & value)		const			{
			for(uint32_t i=0, count = Names.size(); i<count; ++i)
				if(name == Names[i]) {
					value				= Values[i];
					return 0;
				}
			enum_printf("Enumeration value not found! Name: %s.", ::llc::toString(name).begin());
			value				= INVALID_VALUE;
			return -1;
		}
		err_t				get_value				(const char* name, T & value)				const			{
			for(uint32_t i=0, count = Names.size(); i<count; ++i)
				if(0 == ::strcmp(name, Names[i].begin())) {
					value				= Values[i];
					return 0;
				}
			enum_printf("Enumeration value not found! Name: %s.", name);
			value				= INVALID_VALUE;
			return -1;
		}
		T					get_value				(::llc::vcsc_c & name)					const			{
			for(uint32_t i=0, count = Names.size(); i<count; ++i) {
				if(name == Names[i])
					return Values[i];
			}
			enum_printf("Enumeration value not found! Name: %s.", ::llc::toString(name).begin());
			return INVALID_VALUE;
		}
		err_t				get_value_by_index		(uint32_t index, T & value)					const			{
			if(index < Values.size()) {
				value				= Values[index];
				return 0;
			}
			enum_printf("Enumeration index out of range! Index: 0x%" LLC_FMT_U2 ".", index);
			value				= INVALID_VALUE;
			return -1;
		}
		T					get_value_by_index		(uint32_t index)					const			{
			retval_gwarn_if(INVALID_VALUE, index >= Values.size(), "Enumeration index out of range! Index: 0x%" LLC_FMT_U2 ".", index);
			return Values[index];
		}
		err_t				get_label_by_index		(uint32_t index, ::llc::vcsc_t & value)		const			{
			if(index < Names.size()) {
				value				= Names[index];
				return 0;
			}
			value				= ::llc::UNDEFINED_ENUM_VALUE_STR;
			enum_printf("Enumeration index out of range! Index: 0x%" LLC_FMT_U2 ".", index);
			return -1;
		}
		::llc::vcsc_t			get_label_by_index		(uint32_t index)					const			{
			if(index < Names.size())
				return Names[index];
			enum_printf("Enumeration index out of range! Index: 0x%" LLC_FMT_U2 ".", index);
			return ::llc::UNDEFINED_ENUM_VALUE_STR;
		}
		err_t				get_value_index			(::llc::vcsc_c & name, int32_t & index)	const			{
			for(uint32_t i=0, count = Names.size(); i < count; ++i)
				if(name == Names[i]) {
					index				= (int32_t)i;
					return index;
				}
			enum_printf("Enumeration value not found! Name: %s.", name.begin());
			return index				= -1;
		}
		int32_t				get_value_index			(::llc::vcsc_c & name)					const			{
			for(uint32_t i=0, count = Names.size(); i < count; ++i) {
				if(name == Names[i])
					return (int32_t)i;
			}
			enum_printf("Enumeration value not found! Name: %s.", name.begin());
			return -1;
		}
		err_t				get_value_index			(const T & value, int32_t & index)		const			{
			for(uint32_t i=0, count = Names.size(); i < count; ++i)
				if(value == Values[i]) {
					index					= (int32_t)i;
					return 0;
				}
			enum_printf("Enumeration value not found! Value: 0x%llX.", (uint64_t)value);
			return index					= -1;
		}
		int32_t				get_value_index			(const T & value)				const			{
			for(uint32_t i=0, count = Names.size(); i < count; ++i) {
				if(value == Values[i])
					return (int32_t)i;
			}
			enum_printf("Enumeration value not found! Value: %llX.", (uint64_t)value);
			return -1;
		}
		err_t				get_value_label			(const T & value, ::llc::vcsc_t & name)	const			{
			for(uint32_t i=0, count = Values.size(); i < count; ++i)
				if(value == Values[i]) {
					name				= Names[i];
					return 0;
				}
			enum_printf("Enumeration value not found! Value: 0x%llX.", (uint64_t)value);
			name				= ::llc::UNRESOLVED_ENUM_LABEL_STR;
			return -1;
		}
		err_t				get_value_desc			(const T & value, ::llc::vcsc_t & name)	const			{
			for(uint32_t i=0, count = Values.size(); i < count; ++i)
				if(value == Values[i]) {
					name				= Descriptions[i];
					return 0;
				}
			enum_printf("Enumeration value not found! Value: 0x%llX.", (uint64_t)value);
			name				= ::llc::UNRESOLVED_ENUM_LABEL_STR;
			return -1;
		}
		::llc::vcsc_c&	get_value_label			(const T & value)				const			{
			for(uint32_t i=0, count = Values.size(); i < count; ++i) {
				if(value == Values[i])
					return Names[i];
			}
			enum_printf("Enumeration value not found! Value: 0x%llX.", (uint64_t)value);
			return ::llc::UNRESOLVED_ENUM_LABEL_STR;
		}
		::llc::vcsc_c&	get_value_desc			(const T & value)				const			{
			for(uint32_t i=0, count = Values.size(); i < count; ++i) {
				if(value == Values[i])
					return Descriptions[i];
			}
			enum_printf("Enumeration value not found! Value: 0x%llX.", (uint64_t)value);
			return ::llc::UNRESOLVED_ENUM_LABEL_STR;
		}
		err_t				add_value				(const T & value, ::llc::vcsc_c & name, ::llc::vcsc_c & title, ::llc::vcsc_c & description)	{
			for(uint32_t i=0, count = Values.size(); i < count; ++i)
				if(Values[i] == value) {
					rww_if(name != Names[i], "Enumeration value already defined! Type: '%s'. Value: 0x%llX. Previous name: %s. New name: %s. Second definition ignored..."
				, Name		.begin()
				, (uint64_t)value
				, Names[i]	.begin()
				, name		.begin()
				);
					return 0;	// Found same name and value combination. This is normal when values are defined as static const.
				}
			uint32_t				newIndex				= Values.push_back(value);
			llc_necs(newIndex);
			llc_necs(Names.push_back(name));
			llc_necs(Titles.push_back(title));
			llc_necs(Descriptions.push_back(description));
			verbose_printf("Added new value to enumeration definition. Enum name: %s. Index: %.02u, Value: 0x%llX. Value name: %s."
				, Name.begin()
				, (uint32_t)newIndex
				, (uint64_t)value
				, name.begin()
				);
			return newIndex;
		}
		err_t				add_value_auto			(::llc::vcsc_c & name, ::llc::vcsc_c & title, ::llc::vcsc_c & description)	{
			for(uint32_t i=0, count = Names.size(); i < count; ++i) {
				ree_if(name == Names[i], "Enumeration value already defined! Type: '%s'. Value: 0x%llX. Previous name: %s. New name: %s. Second definition ignored..."
					, Name		.begin()
					, (uint64_t)Values[i]
					, Names[i]	.begin()
					, name		.begin()
					);
			}
			const T					value					= (T)Values.size();
			uint32_t				newIndex				= Values.push_back(value);
			llc_necs(newIndex);
			llc_necs(Names.push_back(name));
			llc_necs(Titles.push_back(title));
			llc_necs(Descriptions.push_back(description));
			verbose_printf("Added new value to enumeration definition. Enum name: %s. Index: %.02u, Value: 0x%llX. Value name: %s.", Name.begin(), (uint32_t)newIndex, (uint64_t)value, name.begin());
			return newIndex;
		}
	};

	tplt <tpnm TEnum>	inline	::llc::enum_definition<TEnum>&	get_enum	()					noexcept	{ return ::llc::enum_definition<TEnum>::get();	}
	tplt <tpnm TEnum>	inline	::llc::enum_definition<TEnum>&	get_enum	(const TEnum & )	noexcept	{ return ::llc::enum_definition<TEnum>::get();	}

	// This type is used to initialize an enumeration value.
	tpl_t struct genum_value {
		tdfT(_t);

		T					Value					= ::llc::enum_definition<T>::INVALID_VALUE;
		::llc::vcsc_t			Name					= INVALID_ENUM_VALUE_STR;
		::llc::vcsc_t			Title					= INVALID_ENUM_VALUE_STR;
		::llc::vcsc_t			Description				= INVALID_ENUM_VALUE_STR;
		//
		inxp				genum_value				()										= default;
		inxp				genum_value				(const genum_value & other)				= default;
							genum_value				(const T & value)																						: Value((T)value), Name(::llc::get_enum<T>().get_value_name(value))				{}
							genum_value				(const T & value, ::llc::vcsc_c & name)																: Value((T)value), Name(name), Title(name), Description(name)			{ ::llc::get_enum<T>().add_value(value, name, name, name);			}
							genum_value				(const T & value, ::llc::vcsc_c & name, ::llc::vcsc_c & description)								: Value((T)value), Name(name), Title(name), Description(description)	{ ::llc::get_enum<T>().add_value(value, name, name, description);		}
							genum_value				(const T & value, ::llc::vcsc_c & name, ::llc::vcsc_c & title, ::llc::vcsc_c & description)	: Value((T)value), Name(name), Title(title), Description(description)	{ ::llc::get_enum<T>().add_value(value, name, title, description);	}

		inxp	oper	const	T&				()			const	{ return Value; }
	};

	tplt <tpnm TEnum, size_t nameLen>
	TEnum						get_value			(const char (&valueLabel)[nameLen])		{ return ::llc::get_enum<TEnum>().get_value(::llc::vcst_t{valueLabel}); }
	tplt <tpnm TEnum>	TEnum	get_value			(::llc::vcsc_c & valueLabel)			{ return ::llc::get_enum<TEnum>().get_value(valueLabel); }

	tplt <tpnm TEnum>	TEnum	get_value_camelcased(::llc::vcsc_c & uncased)			{
		::llc::asc_t					camelCased;
		::llc::camelCase(uncased, camelCased);
		return ::llc::get_enum<TEnum>().get_value(camelCased);
	}
	tplt<tpnm TEnum>	ndsi	uint32_t		get_value_count		()							nxpt	{ return ::llc::get_enum<TEnum>().Values.size(); }
	tplt<tpnm TEnum>	ndsi	::llc::vcsc_c&	get_value_label		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_label(statusBit); }
	tplt<tpnm TEnum>	ndsi	::llc::vcsc_c&	get_value_namev		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_label(statusBit); }
	tplt<tpnm TEnum>	ndsi	sc_c*			get_value_namep		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_label(statusBit).begin(); }
	tplt<tpnm TEnum>	ndsi	::llc::vcsc_c&	get_value_descv		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_desc (statusBit); }
	tplt<tpnm TEnum>	ndsi	sc_c*			get_value_descp		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_desc (statusBit).begin(); }
	tplt<tpnm TEnum>	ndsi	::llc::vcvsc_c&	get_value_labels	()							nxpt	{ return ::llc::get_enum<TEnum>().Names; }
	tplt<tpnm TEnum>	ndsi	::llc::vcvsc_c&	get_value_names		()							nxpt	{ return ::llc::get_enum<TEnum>().Names; }
	tplt<tpnm TEnum>	ndsi	::llc::vcvsc_c&	get_value_descs		()							nxpt	{ return ::llc::get_enum<TEnum>().Names; }
	tplt<tpnm TEnum>	ndsi	int32_t			get_value_index		(const TEnum & statusBit)			{ return ::llc::get_enum<TEnum>().get_value_index(statusBit); }
	tplt<tpnm TEnum>	ndsi	::llc::vcsc_c&	get_enum_namev		()							nxpt	{ return ::llc::get_enum<TEnum>().Name;			}
	tplt<tpnm TEnum>	ndsi	::llc::vcsc_c&	get_enum_namev		(const TEnum & )			nxpt	{ return ::llc::get_enum<TEnum>().Name;			}
	tplt<tpnm TEnum>	ndsi	sc_c*			get_enum_namep		()							nxpt	{ return ::llc::get_enum<TEnum>().Name.begin();	}
	tplt<tpnm TEnum>	ndsi	sc_c*			get_enum_namep		(const TEnum & )			nxpt	{ return ::llc::get_enum<TEnum>().Name.begin();	}

	tplTstct genum_value_auto {
		T						Value				= ::llc::enum_definition<T>::INVALID_VALUE;
		::llc::vcsc_t			Name				= INVALID_ENUM_VALUE_STR;
		::llc::vcsc_t			Title				= INVALID_ENUM_VALUE_STR;
		::llc::vcsc_t			Description			= INVALID_ENUM_VALUE_STR;
		//
		inxp				genum_value_auto	()									= default;
		inxp				genum_value_auto	(const genum_value_auto & other)	= default;
							genum_value_auto	(::llc::vcsc_c & name)														: Value((T)0), Name(name), Title(name), Description(name)			{ ::llc::get_enum<T>().add_value_auto(name, name, name);			Value = ::llc::get_value<T>(name); }
							genum_value_auto	(::llc::vcsc_c & name, ::llc::vcsc_c & description)							: Value((T)0), Name(name), Title(name), Description(description)	{ ::llc::get_enum<T>().add_value_auto(name, name, description);		Value = ::llc::get_value<T>(name); }
							genum_value_auto	(::llc::vcsc_c & name, ::llc::vcsc_c & title, ::llc::vcsc_c & description)	: Value((T)0), Name(name), Title(title), Description(description)	{ ::llc::get_enum<T>().add_value_auto(name, title, description);	Value = ::llc::get_value<T>(name); }

		inxp	oper	const	T&		()			const	{ return Value; }
	};

} // namespace

// Defines the enumeration type, the invalid value (-1) and the flag operators
#define GDEFINE_ENUM_TYPE(EnumName, IntType)																	\
	enum EnumName : IntType {};																					\
	static	::llc::u2_c		__sei_##EnumName##enumInit	= ::llc::enum_definition<EnumName>::init(#EnumName);	\
	sinx	EnumName		oper &	(EnumName  a, EnumName b)	noexcept	{ return (EnumName)		(a & (IntType)b);				}	\
	sinx	EnumName		oper ~	(EnumName  a)				noexcept	{ return (EnumName)		(~(IntType)a);					}	\
	sinx	EnumName		oper ^	(EnumName  a, EnumName b)	noexcept	{ return (EnumName)		(a ^ (IntType)b);				}	\
	stin	EnumName&		oper |=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	( ((IntType&)a) |= (IntType)b); }	\
	stin	EnumName&		oper &=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	( ((IntType&)a) &= (IntType)b); }	\
	stin	EnumName&		oper ^=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	( ((IntType&)a) ^= (IntType)b); }	\
	sinx	EnumName		oper |	(EnumName  a, EnumName b)	noexcept	{ return (EnumName)		(a | (IntType)b);				}
//
#define GDEFINE_ENUM_STRUCT(EnumName, IntType)																								\
	struct EnumName {																														\
		IntType				Value			;																								\
		inline	oper		IntType&		()			nxpt	{ return Value; }															\
		inxp	oper		const IntType&	()	const	nxpt	{ return Value; }															\
	};																																		\
	static	::llc::u2_c		__sei_##EnumName##enumInit	= ::llc::enum_definition<EnumName>::init(#EnumName);								\
	sinx	EnumName		oper &	(EnumName  a, EnumName b)	noexcept	{ return {a & (IntType)b};				}	\
	sinx	EnumName		oper ~	(EnumName  a)				noexcept	{ return {~(IntType)a};					}	\
	sinx	EnumName		oper ^	(EnumName  a, EnumName b)	noexcept	{ return {a ^ (IntType)b};				}	\
	stin	EnumName&		oper |=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	{ ((IntType&)a) |= (IntType)b}; }	\
	stin	EnumName&		oper &=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	{ ((IntType&)a) &= (IntType)b}; }	\
	stin	EnumName&		oper ^=	(EnumName &a, EnumName b)	noexcept	{ return (EnumName&)	{ ((IntType&)a) ^= (IntType)b}; }	\
	sinx	EnumName		oper |	(EnumName  a, EnumName b)	noexcept	{ return (EnumName)		{a | (IntType)b};				}
//
#ifdef LLC_ATMEL
#	define GDEFINE_ENUM_VALUE(EnumName, ValueName, EnumValue)					\
	stxp	const EnumName	EnumName##_##ValueName	= (EnumName)(EnumValue);	\
	//static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, F(#ValueName)})

#	define GDEFINE_ENUM_VALUED(EnumName, ValueName, EnumValue, EnumDescription)		\
	stxp	const EnumName	EnumName##_##ValueName	= (EnumName)(EnumValue);		\
	//static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, F(#ValueName)})

#	define GDEFINE_ENUM_VALTD(EnumName, ValueName, EnumValue, EnumTitle, EnumDescription)	\
	stxp	const EnumName	EnumName##_##ValueName	= (EnumName)(EnumValue);				\
	//static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, F(#ValueName)})
	
#else
#	define GDEFINE_ENUM_VALUE(EnumName, ValueName, EnumValue)							\
	stxp	const EnumName	EnumName##_##ValueName			= (EnumName)(EnumValue);	\
	static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, #ValueName})

#	define GDEFINE_ENUM_AVALUE(EnumName, ValueName) static	const EnumName	EnumName##_##ValueName	= (EnumName)::llc::genum_value_auto<EnumName>({szof(#ValueName) - 1, #ValueName})

#	define GDEFINE_ENUM_VALUED(EnumName, ValueName, EnumValue, EnumDescription)				\
	stxp	const EnumName	EnumName##_##ValueName			= (EnumName)(EnumValue);	\
	static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, #ValueName}, {szof(EnumDescription) - 1, EnumDescription})

#	define GDEFINE_ENUM_VALTD(EnumName, ValueName, EnumValue, EnumTitle, EnumDescription)				\
	stxp	const EnumName	EnumName##_##ValueName			= (EnumName)(EnumValue);	\
	static	const EnumName	__sei_##EnumName##_##ValueName	= (EnumName)::llc::genum_value<EnumName>((EnumName)(EnumValue), {szof(#ValueName) - 1, #ValueName}, {szof(EnumTitle) - 1, EnumTitle}, {szof(EnumDescription) - 1, EnumDescription})
#endif

#define GDEFINE_FLAG_TYPE		GDEFINE_ENUM_TYPE
#define GDEFINE_FLAG_VALUE		GDEFINE_ENUM_VALUE

#if defined(LLC_WINDOWS)
#	pragma warning(disable : 4063)	// On Windows, using enum types like we do cause the compiler to throw a warning when the warning level is set to 4
#endif

#define llc_warning_unhandled_value(_enumValue)			warning_printf("Unhandled'%s' value:(0x%X)(%" LLC_FMT_S2 ")(%c) %s"	, ::llc::get_enum_namep LLCREP3(_enumValue) ? char(_enumValue) : '?', ::llc::get_value_namep(_enumValue))
#define llc_enum_valued_log(_funcPrintf, _enumValue)	_funcPrintf("'%s':(0x%X)(%" LLC_FMT_S2 ")(%c)'%s'-'%s'"				, ::llc::get_enum_namep LLCREP3(_enumValue) ? char(_enumValue) : '?', ::llc::get_value_namep(_enumValue), ::llc::get_value_descp(_enumValue))
#define llc_enum_value_info(_enumValue)					llc_enum_value_log	(info_printf	, _enumValue)
#define llc_enum_value_warning(_enumValue)				llc_enum_value_log	(warning_printf	, _enumValue)
#define llc_enum_value_error(_enumValue)				llc_enum_value_log	(error_printf	, _enumValue)
#define llc_enum_valued_info(_enumValue)				llc_enum_valued_log	(info_printf	, _enumValue)
#define llc_enum_valued_warning(_enumValue)				llc_enum_valued_log	(warning_printf	, _enumValue)
#define llc_enum_valued_error(_enumValue)				llc_enum_valued_log	(error_printf	, _enumValue)


namespace llc
{
	GDEFINE_ENUM_TYPE (RESULT, int8_t);
	GDEFINE_ENUM_VALUE(RESULT, Ok                , int8_t(::llc::OS_OK                ));
	GDEFINE_ENUM_VALUE(RESULT, Error             , int8_t(::llc::OS_ERROR             ));
	GDEFINE_ENUM_VALUE(RESULT, Busy              , int8_t(::llc::OS_BUSY              ));
	GDEFINE_ENUM_VALUE(RESULT, Timeout           , int8_t(::llc::OS_TIMEOUT           ));
	GDEFINE_ENUM_VALUE(RESULT, Full              , int8_t(::llc::OS_FULL              ));
	GDEFINE_ENUM_VALUE(RESULT, Empty             , int8_t(::llc::OS_EMPTY             ));
	GDEFINE_ENUM_VALUE(RESULT, Overrun           , int8_t(::llc::OS_OVERRUN           ));
	GDEFINE_ENUM_VALUE(RESULT, Not_available     , int8_t(::llc::OS_NOT_AVAILABLE     ));
	GDEFINE_ENUM_VALUE(RESULT, Not_found         , int8_t(::llc::OS_NOT_FOUND         ));
	GDEFINE_ENUM_VALUE(RESULT, Invalid_parameter , int8_t(::llc::OS_INVALID_PARAMETER ));
	GDEFINE_ENUM_VALUE(RESULT, Forbidden         , int8_t(::llc::OS_FORBIDDEN         ));
	GDEFINE_ENUM_VALUE(RESULT, Restart           , int8_t(::llc::OS_RESTART           ));
	GDEFINE_ENUM_VALUE(RESULT, Wake_up           , int8_t(::llc::OS_WAKE_UP           ));
	GDEFINE_ENUM_VALUE(RESULT, Sleep             , int8_t(::llc::OS_SLEEP             ));
	GDEFINE_ENUM_VALUE(RESULT, Offline           , int8_t(::llc::OS_OFFLINE           ));
	GDEFINE_ENUM_VALUE(RESULT, Disconnected      , int8_t(::llc::OS_DISCONNECTED      ));
	GDEFINE_ENUM_VALUE(RESULT, Connecting        , int8_t(::llc::OS_CONNECTING        ));
	GDEFINE_ENUM_VALUE(RESULT, Connected         , int8_t(::llc::OS_CONNECTED         ));
	GDEFINE_ENUM_VALUE(RESULT, Missing_data      , int8_t(::llc::OS_MISSING_DATA      ));
	GDEFINE_ENUM_VALUE(RESULT, No_Memory         , int8_t(::llc::OS_NO_MEMORY         ));
	GDEFINE_ENUM_VALUE(RESULT, Read              , int8_t(::llc::OS_READ              ));
	GDEFINE_ENUM_VALUE(RESULT, Write             , int8_t(::llc::OS_WRITE             ));

	GDEFINE_ENUM_TYPE (COMMAND, int8_t);
	GDEFINE_ENUM_VALUE(COMMAND, Ok                , RESULT_Ok                * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Error             , RESULT_Error             * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Busy              , RESULT_Busy              * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Timeout           , RESULT_Timeout           * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Full              , RESULT_Full              * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Empty             , RESULT_Empty             * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Overrun           , RESULT_Overrun           * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Not_available     , RESULT_Not_available     * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Not_found         , RESULT_Not_found         * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Invalid_parameter , RESULT_Invalid_parameter * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Forbidden         , RESULT_Forbidden         * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Restart           , RESULT_Restart           * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Wake_up           , RESULT_Wake_up           * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Sleep             , RESULT_Sleep             * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Offline           , RESULT_Offline           * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Disconnected      , RESULT_Disconnected      * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Connecting        , RESULT_Connecting        * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Connected         , RESULT_Connected         * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Missing_data      , RESULT_Missing_data      * -1);
	GDEFINE_ENUM_VALUE(COMMAND, No_Memory         , RESULT_No_Memory         * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Read              , RESULT_Read              * -1);
	GDEFINE_ENUM_VALUE(COMMAND, Write             , RESULT_Write             * -1);
}

#endif // LLC_ENUM_H_23627
