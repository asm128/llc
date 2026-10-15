#include "llc_event_input.h"
#include "llc_event_screen.h"
#include "llc_input.h"

#ifndef LLC_ENGINE_WINDOW_H_23627
#define LLC_ENGINE_WINDOW_H_23627

namespace llc
{
	stct SEngineWindow {
		TQueueSystemEvent			EventQueue		= {};
		SInput						Input			= {};
		n2u1_t						PreviousSize	= {};
		n2u1_t						Size			= {640, 360};
		bool						Resized			= false;
		bool						Repaint			= false;
		bool						NoDraw			= false;
		bool						Closed			= true;

									SEngineWindow	() = default;
									SEngineWindow	(cnst SEngineWindow&) = delete;
		SEngineWindow&				oper=			(cnst SEngineWindow&) = delete;
	};

	void	engineWindowBeginFrame	(SEngineWindow& window) nxpt;
	err_t	engineWindowResize		(SEngineWindow& window, n2u1_t size) nxpt;
}

#endif // LLC_ENGINE_WINDOW_H_23627
