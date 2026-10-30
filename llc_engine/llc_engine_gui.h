#include "llc_engine.h"
#include "llc_gui.h"

#ifndef LLC_ENGINE_GUI_H_23627
#define LLC_ENGINE_GUI_H_23627

namespace llc 
{
	GDEFINE_ENUM_TYPE(GUI_ENTITY, uint8_t);
	GDEFINE_ENUM_VALUE(GUI_ENTITY, Label	, 0);
	GDEFINE_ENUM_VALUE(GUI_ENTITY, Button	, 1);
	GDEFINE_ENUM_VALUE(GUI_ENTITY, Menu		, 2);
	GDEFINE_ENUM_VALUE(GUI_ENTITY, Viewport	, 3);
	GDEFINE_ENUM_VALUE(GUI_ENTITY, Palette	, 4);

	GDEFINE_ENUM_TYPE(UI3D_CONTROL_AREA, uint8_t);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Total		, 0);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Client	, 1);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Left		, 0x80 + ::llc::ALIGN_LEFT		);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Top		, 0x80 + ::llc::ALIGN_TOP		);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Right		, 0x80 + ::llc::ALIGN_RIGHT		);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, Bottom	, 0x80 + ::llc::ALIGN_BOTTOM	);
	GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, COUNT		, 6);
	//GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, LeftBottom	, 6);
	//GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, TopLeft		, 7);
	//GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, RightTop	, 8);
	//GDEFINE_ENUM_VALUE(UI3D_CONTROL_AREA, BottomRight	, 9);

	struct SEngineGUI	{
		::llc::SEngine		Engine			= {};	// We're going to use this in order to take advantage of the d3d implementation we already have.
		::llc::SGUI			GUI				= {};
		::llc::aeid			Entities		= {};
		
		inline				SEngineGUI		()	{ Reset(); }

		::llc::error_t		Reset			()	{
			Engine				= {};
			GUI					= {};
			Entities.clear();

			::llc::SParamsGrid		params			= {};
			params.CellCount	= {4, 4};
			params.Origin		= {1, 1};
			params.Size			= {2, 2};
			if_fail_fe(Entities.resize(::llc::get_value_count<GUI_ENTITY>()));
			if_fail_fe(Entities[GUI_ENTITY_Label]		= Engine.CreateGrid(params));
			if_fail_fe(Entities[GUI_ENTITY_Button]	= Engine.CreateGrid(params));

			if_fail_fe(Entities[GUI_ENTITY_Menu]		= Engine.Clone(Entities[GUI_ENTITY_Button], false, false, false));
			if_fail_fe(Entities[GUI_ENTITY_Viewport]	= Engine.Clone(Entities[GUI_ENTITY_Button], false, false, false));
			if_fail_fe(Entities[GUI_ENTITY_Palette]	= Engine.Clone(Entities[GUI_ENTITY_Button], false, false, false));

			if_true_ve(-1, 0 != ::llc::controlCreate(GUI));
			return 0;
		}

		::llc::eid_t	CreateControl	()	{ return 0; }
		::llc::eid_t	CreateMenu		()	{ return 0; }
		::llc::eid_t	CreateViewport	()	{ return 0; }
	};
} // namespace

#endif // LLC_ENGINE_GUI_H_23627

//#ifdef CreateDialog
//#	define LLC_CreateDialog_REDEFINE
//#	undef CreateDialog
//#endif
//#ifdef LLC_CreateDialog_REDEFINE
//#	ifdef UNICODE
//#		define CreateDialog  CreateDialogW
//#	else
//	#	define CreateDialog  CreateDialogA
//#	endif // !UNICODE
//#endif

