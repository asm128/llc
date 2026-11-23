#include "llc_gui_control_table.h"
#include "llc_array.h"
#include "llc_png.h"

nmsp dry
{
    LLC_USING_TYPEINT();
    LLC_USING_VIEW();
    LLC_USING_ARRAY();
    LLC_USING_ASTATIC();
    LLC_USING_N2();
    LLC_USING_TRI2();
    LLC_USING_RECT();
    LLC_USING_RECT2();
    LLC_USING_MINMAX2();

    enum SCREEN : u0_t 
        { SCREEN_ROOT           
        , SCREEN_HUD            
        , SCREEN_MENU           
        , SCREEN_MODAL          
        , SCREEN_HOME           
        , SCREEN_LOAD_FILAMENT  
        , SCREEN_INFO           
        , SCREEN_CONFIG         
        , SCREEN_COUNT
        };
    //GDEFINE_ENUM_VALUE(SCREEN, , 4);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_MENU, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, DIALOG                    , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, HOME                    , 1);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, LOAD_FILAMENT            , 2);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, INFO                    , 3);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, CONFIG                    , 4);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MENU, COUNT                    , 5);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_HUD, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, DIALOG                    , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, CALENDAR                    , 1);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, DOOR                        , 2);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, ETHERNET                    , 3);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, WIFI                        , 4);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, RFID                        , 5);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, SERIAL                    , 6);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_HUD, COUNT                    , 7);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_MODAL, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, DIALOG                    , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, CLOSE                    , 1);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, YES                    , 2);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, NO                        , 3);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, LOAD                    , 4);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, UNLOAD                    , 5);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, CANCEL                    , 6);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, ICON_START                , 7);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, ICON_STOP                , 8);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_MODAL, COUNT                    , 9);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_LOAD_FILAMENT, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_LOAD_FILAMENT, DIALOG            , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_LOAD_FILAMENT, COUNT            , 1);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_SPOOL_INFO, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, DIALOG            ,  0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, TITLE                ,  1);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_BRAND        ,  2);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_MATERIAL    ,  3);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_COLOR        ,  4);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_NOTES        ,  5);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_DENSITY        ,  6);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_DIAMETER    ,  7);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, LABEL_SPOOL_SIZE    ,  8);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_BRAND        ,  9);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_MATERIAL    , 10);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_COLOR        , 11);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_NOTES        , 12);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_DENSITY        , 13);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_DIAMETER    , 14);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, VALUE_SPOOL_SIZE    , 15);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_SPOOL_INFO, COUNT                , 16);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_INFO, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_INFO, DIALOG                    , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_INFO, COUNT                    , 1);

    GDEFINE_ENUM_TYPE(SCREEN_CONTROL_CONFIG, u0_t );
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_CONFIG, DIALOG                , 0);
    GDEFINE_ENUM_VALUE(SCREEN_CONTROL_CONFIG, COUNT                    , 1);

    GDEFINE_ENUM_TYPE(MODAL, u0_t );
    GDEFINE_ENUM_VALUE(MODAL, CLOSE_DOOR        , 0);
    GDEFINE_ENUM_VALUE(MODAL, SCAN_SPOOL        , 1);
    GDEFINE_ENUM_VALUE(MODAL, DRYING_COMPLETE    , 2);
    GDEFINE_ENUM_VALUE(MODAL, DRYING_STOP        , 3);

    struct SControlParams {
        ::llc::GUI_COLOR_MODE  ColorMode     ;
        ::llc::cid_t           Id            ;
        ::llc::cid_t           IdParent      ;
        b8_t                   Design        ;
        b8_t                   NoBackground  ;
        b8_t                   NoHoverEffect ;
        b8_t                   FrameOut      ;
        ::llc::ALIGN           Align         ;
        rect2s1_t              Area          ;
        rectu1_t               Border        ;
        rectu1_t               Margin        ;
        vcst_t                 Text          ;
        ::llc::ALIGN           TextAlign     ;
        ::llc::ALIGN           IconAlign     ;
        vcst_t                 IconPathOff   ;
        vcst_t                 IconPathOn    ;
        n2b8_t                 AdjustToText  ;
    };
    enum PALETTE : u1_t
        { PALETTE_MENU_NORMAL                   = 400 + (u0_t)::llc::GUI_CONTROL_PALETTE_NORMAL                                   
        , PALETTE_MENU_DISABLED                 = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DISABLED             
        , PALETTE_MENU_HOVER                    = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_HOVER                
        , PALETTE_MENU_PRESSED                  = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_PRESSED              
        , PALETTE_MENU_DRAGGING                 = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DRAGGING             
        , PALETTE_MENU_SELECTED                 = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED             
        , PALETTE_MENU_SELECTED_DISABLED        = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED     
        , PALETTE_MENU_SELECTED_HOVER           = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER       
        , PALETTE_MENU_SELECTED_PRESSED         = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED        
        , PALETTE_MENU_SELECTED_DRAGGING        = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DRAGGING    
        , PALETTE_MENU_OUTDATED                 = PALETTE_MENU_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_OUTDATED             
        , PALETTE_START_NORMAL                  = 420 + (u0_t)::llc::GUI_CONTROL_PALETTE_NORMAL                                   
        , PALETTE_START_DISABLED                = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DISABLED             
        , PALETTE_START_HOVER                   = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_HOVER                 
        , PALETTE_START_PRESSED                 = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_PRESSED              
        , PALETTE_START_DRAGGING                = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DRAGGING             
        , PALETTE_START_SELECTED                = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED              
        , PALETTE_START_SELECTED_DISABLED       = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED    
        , PALETTE_START_SELECTED_HOVER          = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER       
        , PALETTE_START_SELECTED_PRESSED        = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED     
        , PALETTE_START_SELECTED_DRAGGING       = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DRAGGING    
        , PALETTE_START_OUTDATED                = PALETTE_START_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_OUTDATED             
        , PALETTE_CANCEL_NORMAL                 = 440 + (u0_t)::llc::GUI_CONTROL_PALETTE_NORMAL                                   
        , PALETTE_CANCEL_DISABLED               = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DISABLED                 
        , PALETTE_CANCEL_HOVER                  = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_HOVER                 
        , PALETTE_CANCEL_PRESSED                = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_PRESSED                 
        , PALETTE_CANCEL_DRAGGING               = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DRAGGING                 
        , PALETTE_CANCEL_SELECTED               = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED              
        , PALETTE_CANCEL_SELECTED_DISABLED      = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED      
        , PALETTE_CANCEL_SELECTED_HOVER         = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER           
        , PALETTE_CANCEL_SELECTED_PRESSED       = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED         
        , PALETTE_CANCEL_SELECTED_DRAGGING      = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DRAGGING        
        , PALETTE_CANCEL_OUTDATED               = PALETTE_CANCEL_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_OUTDATED                 
        , PALETTE_OK_NORMAL                     = 460 + (u0_t)::llc::GUI_CONTROL_PALETTE_NORMAL                                   
        , PALETTE_OK_DISABLED                   = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DISABLED                   
        , PALETTE_OK_HOVER                      = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_HOVER                   
        , PALETTE_OK_PRESSED                    = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_PRESSED                   
        , PALETTE_OK_DRAGGING                   = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_DRAGGING                   
        , PALETTE_OK_SELECTED                   = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED                   
        , PALETTE_OK_SELECTED_DISABLED          = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DISABLED       
        , PALETTE_OK_SELECTED_HOVER             = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_HOVER           
        , PALETTE_OK_SELECTED_PRESSED           = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_PRESSED           
        , PALETTE_OK_SELECTED_DRAGGING          = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_SELECTED_DRAGGING                   
        , PALETTE_OK_OUTDATED                   = PALETTE_OK_NORMAL + (u0_t)::llc::GUI_CONTROL_PALETTE_OUTDATED                   
        , PALETTE_COUNT                         = 1 + PALETTE_OK_OUTDATED                                                       
        };
	::llc::err_t					setupPalettes	(::llc::SControlColors	& colors );
	struct SControlImages {
		::llc::img8bgra		On	;
		::llc::img8bgra		Off	;
	};
    
	template<size_t _countControls>
	struct SScreen {
		::llc::astatic<::llc::cid_t, _countControls>			ControlIds;
		::llc::astatic<::dry::SControlImages, _countControls>	ControlIcons;
    
		inln											SScreen			()	{ ::llc::view<::llc::cid_t>{ControlIds}.fill(-1); }
	};
    
	template<typename _tScreen, size_t _countControls>
	sttc llc::err_t						setupScreenFromControlParams	(::llc::SControlTable & controls, ::llc::cid_t rootControlIndex, _tScreen & screen, cnst ::dry::SControlParams (&controlSetupList)[_countControls], ::llc::vs1_t defaultColors, ::llc::b8_t loadImages = true)	{
		::llc::SPNGData									pngCache				= {};
		for(u2_t  iControl = 0; iControl < _countControls; ++iControl) {
			cnst SControlParams							& controlSetup			= controlSetupList[iControl];
			cnst ::llc::cid_t									controlIndex
				= screen.ControlIds[controlSetup.Id]	= ::llc::controlCreate(controls, defaultColors);
    
			::llc::SControlPlacement						& control				= controls.Placement	[controlIndex];
			::llc::SControlText								& controlText			= controls.Text			[controlIndex];
			::llc::SControlDraw								& controlDraw			= controls.Draw			[controlIndex];
			::llc::SControlImage							& controlImage			= controls.Images		[controlIndex];
			::llc::SControlState							& controlState			= controls.States		[controlIndex];
			::llc::SControlConstraints						& controlConstraints	= controls.Constraints	[controlIndex];
			control.Align								= controlSetup.Align		;
			control.Area								= controlSetup.Area			;
			control.Border								= controlSetup.Border		;
			control.Margin								= controlSetup.Margin		;
			controlImage.ImageAlign						= controlSetup.IconAlign	;
			controlText.Text							= controlSetup.Text			;
			controlText.Align							= controlSetup.TextAlign	;
			controlConstraints.AttachSizeToText			= controlSetup.AdjustToText	;
			controlDraw.NoBorder							= controlSetup.Design		;
			controlDraw.ColorMode						= controlSetup.ColorMode	;
			controlDraw.NoClient							= controlSetup.NoBackground;
			controlDraw.FrameOut							= controlSetup.FrameOut;
			if(controlSetup.NoHoverEffect)
				controlState.Mask							= ::llc::bit_clear(controlState.Mask, ::llc::GUI_CONTROL_FLAG_Hovered);
			::llc::controlSetParent(controls, controlIndex, (controlSetup.IdParent == -1) ? rootControlIndex : screen.ControlIds[controlSetup.IdParent]);
    
			if(loadImages && (controlSetup.IconPathOff == controlSetup.IconPathOn) && controlSetup.IconPathOn.size()) {
				if_fail_cef(::llc::pngLoadFile(pngCache, controlSetup.IconPathOff, screen.ControlIcons[controlSetup.Id].Off), "Failed to load icon: '%.*s'", controlSetup.IconPathOff.begin());
				controlImage.Image							= screen.ControlIcons[controlSetup.Id].Off.View;
				screen.ControlIcons[controlSetup.Id].On.View	= screen.ControlIcons[controlSetup.Id].Off.View;
			}
			else if(loadImages) {
				if(controlSetup.IconPathOn.size()) {
					if_fail_cef(::llc::pngLoadFile(pngCache, controlSetup.IconPathOn , screen.ControlIcons[controlSetup.Id].On), "Failed to load icon: '%.*s'", controlSetup.IconPathOn.begin());
					controlImage.Image							= screen.ControlIcons[controlSetup.Id].On.View;
				}
				if(controlSetup.IconPathOff.size()) {
					if_fail_cef(::llc::pngLoadFile(pngCache, controlSetup.IconPathOff, screen.ControlIcons[controlSetup.Id].Off), "Failed to load icon: '%.*s'", controlSetup.IconPathOff.begin());
					controlImage.Image							= screen.ControlIcons[controlSetup.Id].Off.View;
				}
			}
		}
		return 0;
	}

} // nmsp

#define USE_3D_CONTROLS

#if defined USE_3D_CONTROLS
#	define DRY_MENU_COLOR_MODE	::llc::GUI_COLOR_MODE_3D
#	define BACKGROUND_3D_ONLY	false
#else
#	define DRY_MENU_COLOR_MODE ::llc::GUI_COLOR_MODE_FLAT
#	define BACKGROUND_3D_ONLY	true
#endif

nmsp dry
{
    stxp cnst ::dry::SControlParams            control_list_hud            []    =
        {    {DRY_MENU_COLOR_MODE           , ::dry::SCREEN_CONTROL_HUD_DIALOG        , -1                                ,  true, true, false, false, ::llc::ALIGN_CENTER_TOP    , {{}, {0, 40}}, {1, 1, 1, 1}, {4, 4, 4, 4}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_DOOR        , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER_RIGHT    , {{ 10, 0}, {48, 40}}, {}, {}, ""                , ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon_lock-off.024.png"        , "../smart-data/icons/icon_lock-on.024.png"        , {}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_ETHERNET    , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER_LEFT    , {{  0, 0}, {48, 40}}, {}, {}, ""                , ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon_ethernet-off.024.png"    , "../smart-data/icons/icon_ethernet-on.024.png"    , {}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_WIFI        , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER_LEFT    , {{ 48, 0}, {48, 40}}, {}, {}, ""                , ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon-wifi-01.024.png"        , "../smart-data/icons/icon-wifi-01.024.png"        , {}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_RFID        , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER_LEFT    , {{ 96, 0}, {48, 40}}, {}, {}, ""                , ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon_rfid.024.png"            , "../smart-data/icons/icon_rfid.024.png"            , {}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_SERIAL        , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER_LEFT    , {{144, 0}, {48, 40}}, {}, {}, "Serial"        , ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "", "", {true,}}
        ,    {::llc::GUI_COLOR_MODE_Flat    , ::dry::SCREEN_CONTROL_HUD_CALENDAR    , ::dry::SCREEN_CONTROL_HUD_DIALOG    , false, true, false, false, ::llc::ALIGN_CENTER        , {{  0, 0}, {32, 40}}, {}, {}, "Jan 01 | 23:59", ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "", "", {true,}}
        };
    stxp cnst ::dry::SControlParams            control_list_menu            []    =
        {    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_DIALOG            , -1                                ,  true, true                , false, false, ::llc::ALIGN_CENTER_BOTTOM    , {{}, {  0, 24}}, {0, 1, 0, 1}, {}}
        //,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_HOME            , ::dry::SCREEN_CONTROL_MENU_DIALOG    , false, BACKGROUND_3D_ONLY , false, false, ::llc::ALIGN_CENTER_LEFT    , {{0, 0}            , {128, 40}}, {2, 2, 2, 2}, {6, 6, 10, 6}, "Simplified View", ::llc::ALIGN_CENTER}
        //,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_HOME            , ::dry::SCREEN_CONTROL_MENU_DIALOG    , false, BACKGROUND_3D_ONLY , false, false, ::llc::ALIGN_CENTER_LEFT    , {{0, 0}            , { 88, 40}}, {2, 2, 2, 2}, {6, 6, 10, 6}, "Home"            , ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_LEFT, "../smart-data/icons/icon_menu-home.024.png"    }
        //,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_LOAD_FILAMENT    , ::dry::SCREEN_CONTROL_MENU_DIALOG    , false, BACKGROUND_3D_ONLY , false, false, ::llc::ALIGN_CENTER_LEFT    , {{88, 0}            , {168, 40}}, {2, 2, 2, 2}, {6, 6, 10, 6}, "Load filament"    , ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_LEFT, "../smart-data/icons/icon_menu-filament.024.png"}
        //,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_INFO            , ::dry::SCREEN_CONTROL_MENU_DIALOG    , false, BACKGROUND_3D_ONLY , false, false, ::llc::ALIGN_CENTER_LEFT    , {{88+168, 0}        , { 88, 40}}, {2, 2, 2, 2}, {6, 6, 10, 6}, "Info"            , ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_LEFT, "../smart-data/icons/icon_menu-info.024.png"    }
        //,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MENU_CONFIG            , ::dry::SCREEN_CONTROL_MENU_DIALOG    , false, BACKGROUND_3D_ONLY , false, false, ::llc::ALIGN_CENTER_LEFT    , {{88+168+88, 0}    , {104, 40}}, {2, 2, 2, 2}, {6, 6, 10, 6}, "Config"            , ::llc::ALIGN_CENTER_RIGHT, ::llc::ALIGN_LEFT, "../smart-data/icons/icon_menu-config.024.png"    }
        };
    stxp cnst ::dry::SControlParams            control_list_modal            []    =
        {    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_DIALOG        , -1                                , false, false,  true,  true, ::llc::ALIGN_CENTER        , {{}, {}}, {2, 2, 2, 2}, {}, "Close the door correctly to start drying"    , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_ICON_START    , ::dry::SCREEN_CONTROL_MODAL_DIALOG, true,  true, false, false    , ::llc::ALIGN_CENTER_TOP    , {{  0, 16}, {48, 48}}, {}, {}, "", ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon_play.032.png"}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_ICON_STOP        , ::dry::SCREEN_CONTROL_MODAL_DIALOG, true,  true, false, false    , ::llc::ALIGN_CENTER_TOP    , {{  0, 16}, {48, 48}}, {}, {}, "", ::llc::ALIGN_CENTER, ::llc::ALIGN_CENTER, "../smart-data/icons/icon_stop.032.png"}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_YES            , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{-64, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "Yes"    , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_NO            , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{ 64, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "No"        , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_CLOSE            , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{  0, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "OK"        , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_LOAD            , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{-96, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "Load"    , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_UNLOAD        , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{-96, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "Unload"    , ::llc::ALIGN_CENTER}
        ,    {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_MODAL_CANCEL        , ::dry::SCREEN_CONTROL_MODAL_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_BOTTOM, {{ 96, 24}, {72, 40}}, {3, 3, 3, 3}, {}, "Cancel"    , ::llc::ALIGN_CENTER}
        };
    stxp cnst ::dry::SControlParams            control_list_spool_info        []    =
        {     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG            , -1,  true, true, true, true, ::llc::ALIGN_CENTER_TOP}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_TITLE            , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, true, false, true, true, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 0        }, { 64, 30}}, {3, 3, 3, 3}, {}, "Spool Scanned!", ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_BRAND        , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 1 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Brand"        , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_MATERIAL    , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 2 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Material"    , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_COLOR        , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 3 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Color"        , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_SPOOL_SIZE    , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 4 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Spool Size"        , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_DENSITY    , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 5 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Density"    , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_DIAMETER    , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 6 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Diameter"    , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_NOTES        , ::dry::SCREEN_CONTROL_SPOOL_INFO_DIALOG, false, false, false, false, ::llc::ALIGN_CENTER_TOP, {{0, 24 * 7 + 6    }, {256, 24}}, {1, 1, 1, 1}, {2,}, "Notes"    , ::llc::ALIGN_CENTER_LEFT}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_BRAND        , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_BRAND        , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "Kodak"                , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_MATERIAL    , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_MATERIAL    , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "ABS"                    , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_COLOR        , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_COLOR        , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "Red"                    , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_NOTES        , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_NOTES        , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "Filament prototype"    , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_DENSITY    , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_DENSITY    , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "?"                    , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_DIAMETER    , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_DIAMETER    , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "1.75mm"                , ::llc::ALIGN_CENTER}
        ,     {DRY_MENU_COLOR_MODE, ::dry::SCREEN_CONTROL_SPOOL_INFO_VALUE_SPOOL_SIZE    , ::dry::SCREEN_CONTROL_SPOOL_INFO_LABEL_SPOOL_SIZE    , true, false, true, false, ::llc::ALIGN_CENTER_RIGHT, {{}, {72, 22}}, {1, 1, 1, 1}, {}, "750g"                    , ::llc::ALIGN_CENTER}
        };
    stxp cnst ::dry::SControlParams            control_list_load_filament    []    =
        {    {::llc::GUI_COLOR_MODE_Flat, ::dry::SCREEN_CONTROL_LOAD_FILAMENT_DIALOG    , -1,  true, true}
        };
    stxp cnst ::dry::SControlParams            control_list_info            []    =
        {    {::llc::GUI_COLOR_MODE_Flat, ::dry::SCREEN_CONTROL_INFO_DIALOG    , -1,  true, true}
        };
    stxp cnst ::dry::SControlParams            control_list_config            []    =
        {    {::llc::GUI_COLOR_MODE_Flat, ::dry::SCREEN_CONTROL_CONFIG_DIALOG, -1,  true, true}
        };

} // nmsp
