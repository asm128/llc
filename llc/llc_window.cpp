#include "llc_window.h"

::llc::error_t			llc::fullScreenExit			(::llc::SWindow & window) {
	if(false == window.FullScreen)
		return 0;

	window.Size				= window.WindowedWindowRect.Dimensions();
#if defined(LLC_WINDOWS)
	HWND						windowHandle				= window.PlatformDetail.WindowHandle;
	DWORD						style						= GetWindowLong(windowHandle, GWL_STYLE);
	SetWindowLong(windowHandle, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
	SetWindowPos(windowHandle, NULL, window.WindowedWindowRect.Left, window.WindowedWindowRect.Top
		, window.WindowedWindowRect.Right  - window.WindowedWindowRect.Left
		, window.WindowedWindowRect.Bottom - window.WindowedWindowRect.Top
		, SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);
#endif // LLC_WINDOWS
	if_true_e(failed(::llc::eventEnqueueScreenResize(window.EventQueue, window.Size)));
	window.FullScreen		= false;
	return 0;
}

::llc::error_t			llc::fullScreenEnter		(::llc::SWindow & window) {
	if(window.FullScreen)
		return 1;

#if defined(LLC_WINDOWS)
	HWND						windowHandle				= window.PlatformDetail.WindowHandle;
	MONITORINFO					monitor_info				= {sizeof(monitor_info)};
	RECT						windowsRect					= {};
	if_true_fef(FALSE == GetWindowRect(windowHandle, &windowsRect), "Cannot get window rect for hWnd(0x%x)", windowHandle);
	window.WindowedWindowRect.Top		= (uint16_t)windowsRect.top		;
	window.WindowedWindowRect.Left		= (uint16_t)windowsRect.left	;
	window.WindowedWindowRect.Right		= (uint16_t)windowsRect.right	;
	window.WindowedWindowRect.Bottom	= (uint16_t)windowsRect.bottom	;
	if_true_fef(FALSE == GetMonitorInfoA(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitor_info), "Cannot get MONITORINFO for hWnd(0x%x)", windowHandle);

	const DWORD					style						= GetWindowLong(windowHandle, GWL_STYLE);
	SetWindowLong(windowHandle, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
	SetWindowPos(windowHandle, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top
		, monitor_info.rcMonitor.right - monitor_info.rcMonitor.left
		, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top
		, SWP_NOOWNERZORDER | SWP_FRAMECHANGED
	);

	window.Size				= 
		{ uint16_t(monitor_info.rcMonitor.right - monitor_info.rcMonitor.left)
		, uint16_t(monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top)
		};
#endif // LLC_WINDOWS
	if_true_e(failed(::llc::eventEnqueueScreenResize(window.EventQueue, window.Size)));
	window.FullScreen		= true;
	return 0;
}

::llc::error_t			llc::windowUpdateTick		(::llc::SWindow& displayInstance)											{
	bool						quit						= false;
#if defined(LLC_WINDOWS)
	::MSG						msg							= {};
	int							counter						= 0;
	while(::PeekMessage(&msg, displayInstance.PlatformDetail.WindowHandle, 0, 0, PM_REMOVE) && (100 > counter++)) {
		::TranslateMessage	(&msg);
		::DispatchMessage	(&msg);
		if(msg.message == WM_QUIT)
			quit = true;
	}
#elif defined(LLC_XCB)
	(void)displayInstance;
#else
	(void)displayInstance;
#endif // LLC_WINDOWS
	return quit ? 1 : 0;
}

::llc::error_t			llc::windowUpdate		(::llc::SWindow& displayInstance)											{
	displayInstance.EventQueue.clear();
	displayInstance.Resized	= false;
	displayInstance.Repaint	= false;
	bool						quit					= false;
#if defined(LLC_WINDOWS)
	::MSG						msg						= {};
	while(::PeekMessage(&msg, displayInstance.PlatformDetail.WindowHandle, 0, 0, PM_NOREMOVE))
		if(1 == windowUpdateTick(displayInstance))
			quit					= true;
#elif defined(LLC_XCB)
	 
#endif // LLC_WINDOWS
	return quit ? 1 : 0;
}

#if defined(LLC_WINDOWS)
::llc::error_t			drawBuffer				(::HDC hdc, ::llc::SWindowPlatformDetail & offscreenDetail, int width, int height, const ::llc::g8bgra& colorArray)				{
	const uint32_t				bytesToCopy				= sizeof(::RGBQUAD) * colorArray.size();
	::llc::n2u1_c				metricsSource			= colorArray.metrics().u1();
	::llc::n2u1_c				prevSize				= {(uint16_t)offscreenDetail.BitmapInfo.bmiHeader.biWidth, (uint16_t)offscreenDetail.BitmapInfo.bmiHeader.biHeight};
	if( metricsSource.x != offscreenDetail.BitmapInfo.bmiHeader.biWidth 
	 || metricsSource.y != offscreenDetail.BitmapInfo.bmiHeader.biHeight
	 || width  != offscreenDetail.BitmapInfo.bmiHeader.biWidth 
	 || height != offscreenDetail.BitmapInfo.bmiHeader.biHeight
	 || 0 == offscreenDetail.IntermediateDeviceContext
	 || 0 == offscreenDetail.IntermediateBitmap
	) {
		offscreenDetail.BitmapInfoSize			= sizeof(::BITMAPINFO);
		//memcpy(&offscreenDetail.BitmapInfo->bmiColors[1], colorArray[0].begin(), metricsSource.x * metricsSource.y * sizeof(::llc::bgra));

		offscreenDetail.BitmapInfo.bmiHeader				= {sizeof(::BITMAPINFO)};
		offscreenDetail.BitmapInfo.bmiHeader.biWidth		= colorArray.metrics().x;
		offscreenDetail.BitmapInfo.bmiHeader.biHeight		= colorArray.metrics().y;
		offscreenDetail.BitmapInfo.bmiHeader.biPlanes		= 1;
		offscreenDetail.BitmapInfo.bmiHeader.biBitCount		= 32;
		offscreenDetail.BitmapInfo.bmiHeader.biCompression	= BI_RGB;
		offscreenDetail.BitmapInfo.bmiHeader.biSizeImage	= bytesToCopy;

		if(offscreenDetail.IntermediateBitmap)
			DeleteObject(offscreenDetail.IntermediateBitmap);

		if(offscreenDetail.IntermediateDeviceContext)
			DeleteDC(offscreenDetail.IntermediateDeviceContext);

		::llc::SWindowPlatformDetail::TOSWindowNativeTexel		* pixelBits						= 0;
		offscreenDetail.IntermediateDeviceContext			= ::CreateCompatibleDC(hdc);    // <- note, we're creating, so it needs to be destroyed
		if_true_fef(0 == (offscreenDetail.IntermediateBitmap		= ::CreateDIBSection(offscreenDetail.IntermediateDeviceContext, &offscreenDetail.BitmapInfo, DIB_RGB_COLORS, (void**) &pixelBits, NULL, 0)), "%s", "Failed to create intermediate dib section.");
		offscreenDetail.PixelBits							= {pixelBits, colorArray.area()};
	}
	const uint32_t lineWidth	= metricsSource.x * sizeof(::llc::bgra);
	const uint32_t lastLine		= metricsSource.y - 1;
	const ::llc::bgra									* dest										= colorArray.begin();
	for(uint32_t y = 0, yMax = metricsSource.y; y < yMax; ++y)
		memcpy(&offscreenDetail.PixelBits[y * metricsSource.x], &dest[(lastLine - y) * metricsSource.x], lineWidth);

	::HBITMAP									hBmpOld										= (::HBITMAP)::SelectObject(offscreenDetail.IntermediateDeviceContext, offscreenDetail.IntermediateBitmap);    // <- altering state
	if_true_ef(FALSE == ::BitBlt(hdc, 0, 0
		, ::llc::min(metricsSource.x, (uint16_t)offscreenDetail.BitmapInfo.bmiHeader.biWidth)
		, ::llc::min(metricsSource.y, (uint16_t)offscreenDetail.BitmapInfo.bmiHeader.biHeight)
		, offscreenDetail.IntermediateDeviceContext, 0, 0, SRCCOPY)
		, "%s", "Not sure why would this happen but probably due to mismanagement of the target size or the system resources. I've had it failing when I acquired the device too much and never released it."
	);
	//::SetStretchBltMode(hdc, COLORONCOLOR);
	//if_true_e(FALSE == ::StretchBlt(hdc, 0, 0, width, height, offscreenDetail.IntermediateDeviceContext, 0, 0, colorArray.metrics().x, colorArray.metrics().y, SRCCOPY), "%s", "Not sure why would this happen but probably due to mismanagement of the target size or the system resources. I've had it failing when I acquired the device too much and never released it.");
	::SelectObject(hdc, hBmpOld);	// put the old bitmap back in the DC (restore state)
	return 0;
}
#endif // LLC_WINDOWS

::llc::error_t			llc::windowPresentTarget					(::llc::SWindow& displayInstance, const ::llc::g8bgra& targetToPresent)		{
	(void)displayInstance; (void)targetToPresent;
#if defined(LLC_WINDOWS)
	::HWND					windowHandle								= displayInstance.PlatformDetail.WindowHandle;
	if_null_vwf(1, windowHandle, "%s", "presentTarget called without a valid window handle set for the main window.");
	::HDC					dc											= ::GetDC(windowHandle);
	if_true_fef(0 == dc, "%s", "Failed to retrieve device context from the provided window handle.");
	if_true_ef(failed(::drawBuffer(dc, displayInstance.PlatformDetail, displayInstance.Size.x, displayInstance.Size.y, targetToPresent)), "%s", "Not sure why this would happen.");
	::ReleaseDC(windowHandle, dc);
#elif defined(LLC_XCB)
	(void)displayInstance; (void)targetToPresent;
#else // ??
	(void)displayInstance; (void)targetToPresent;
#endif // LLC_WINDOWS
	return 0;
}
