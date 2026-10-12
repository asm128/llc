#define LLC_AVOID_LOCAL_APPLICATION_MODEL_EXECUTABLE_RUNTIME
#include "llc_app_impl.h"

struct SApplication {
	::llc::u2_t	UpdateCount	= {};
	::llc::u2_t	DrawCount	= {};

	SApplication(::llc::SRuntimeValues &) {}
};

::llc::err_t setup(SApplication & app) {
	app.UpdateCount	= 0;
	app.DrawCount	= 0;
	rtrn 0;
}

::llc::err_t cleanup(SApplication &) { rtrn 0; }

::llc::err_t update(SApplication & app, bool systemRequestedExit) {
	++app.UpdateCount;
	rtrn systemRequestedExit ? ::llc::APPLICATION_STATE_EXIT : ::llc::APPLICATION_STATE_NORMAL;
}

::llc::err_t draw(SApplication & app) {
	++app.DrawCount;
	rtrn 0;
}

LLC_DEFINE_APPLICATION_ENTRY_POINT_MT(SApplication, "LLC test module");
