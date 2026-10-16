#include "llc_engine_window.h"

void llc::engineWindowBeginFrame(SEngineWindow& window) nxpt {
	inputNextFrame(window.Input);
	window.EventQueue.clear();
	window.Resized = false;
	window.Repaint = false;
}

::llc::err_t llc::engineWindowResize(SEngineWindow& window, n2u1_t size) nxpt {
	if(0 == size.x || 0 == size.y)
		rtrn OS_INVALID_PARAMETER;
	if(size == window.Size)
		rtrn 0;
	window.PreviousSize = window.Size;
	window.Size = size;
	window.Resized = true;
	window.Repaint = true;
	rtrn eventEnqueueScreenResize(window.EventQueue, size);
}
