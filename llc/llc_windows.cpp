#include "llc_windows.h"

#if defined(LLC_WINDOWS)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>

::llc::string			llc::getWindowsErrorAsString	(const int32_t lastError)					{	// Get the error message, if any.
	if(0 == lastError)
		return {};
	char						* messageBuffer					= nullptr;
	u2_c				size							= FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, (DWORD)lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	ve_if(::llc::string(), nullptr == messageBuffer)
	else {
		const ::llc::string			message							= ::llc::vcst_t{messageBuffer, size >= 2 ? size - 2 : size};
		LocalFree(messageBuffer);
		return message;
	}
}

::llc::error_t			llc::wcstombs					(::llc::string & output, const ::llc::view<wchar_t> input)	{
	if(0 == input.size())
		return 0;

	int							sizeNeededForMultiByte			= WideCharToMultiByte(CP_UTF8, 0, input.begin(), int(input.size()), nullptr, 0, nullptr, nullptr);
	if(0 == sizeNeededForMultiByte)
		return 0;

	::llc::string				converted;
	llc_necs(converted.resize(sizeNeededForMultiByte));
	WideCharToMultiByte(CP_UTF8, 0, input.begin(), input.size(), &converted[0], int(converted.size()), nullptr, nullptr);
	llc_necs(output.append(converted));
	return sizeNeededForMultiByte;
}

::llc::error_t			llc::mbstowcs					(::llc::apod<wchar_t> & output, ::llc::vcst_t input)	{
	if(0 == input.size())
		return 0;

	::llc::apod<wchar_t>		converted;
	converted.resize(input.size());
	llc_hrcall(MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, &input[0], input.size(), &converted[0], converted.size()));
	converted.resize((uint32_t)wcslen(converted.begin()));
	output.append(converted);
	return converted.size();
}

#endif

// `d:\dev_extras\battleground`
// `d:\dev_extras\blitdb`
// `d:\dev_extras\blitter`
// `d:\dev_extras\ced`
// `d:\dev_extras\ced_data`
// `d:\dev_extras\demo`
// `d:\dev_extras\gpftw`
// `d:\dev_extras\gpftw_advanced`
// `d:\dev_extras\gpftw_expert`
// `d:\dev_extras\gpftw_master`
// `d:\dev_extras\gpftw_professional`
// `d:\dev_extras\gpk`
// `d:\dev_extras\gpk_data`
// `d:\dev_extras\gpk_games`
// `d:\dev_extras\gpk_samples`
// `d:\dev_extras\kitsurpg`
// `d:\dev_extras\lilia`
// `d:\dev_extras\llc`
// `d:\dev_extras\llt`
// `d:\dev_extras\neutralizer`
// `d:\dev_extras\nwol`
// `d:\dev_extras\nwol_samples`
// `d:\dev_extras\the_one`

// `d:\dev_extras\obj`
// `d:\dev_extras\Win32.Debug`
// `d:\dev_extras\Win32.Release`
// `d:\dev_extras\x64.Debug`
// `d:\dev_extras\x64.Release`
