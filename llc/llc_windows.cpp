#include "llc_windows.h"

#if defined(LLC_WINDOWS)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <Windows.h>

::llc::apod<char>		llc::getWindowsErrorAsString	(const int32_t lastError)					{	// Get the error message, if any.
	if(0 == lastError)
		return {};
	char						* messageBuffer					= nullptr;
	u2_c				size							= FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, (DWORD)lastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	ve_if(::llc::apod<char>(), nullptr == messageBuffer)
	else {
		const ::llc::apod<char>		message							= ::llc::view<const char>{messageBuffer, size >= 2 ? size - 2 : size};
		LocalFree(messageBuffer);
		return message;
	}
}

::llc::error_t			llc::wcstombs					(::llc::apod<char> & output, const ::llc::view<wchar_t> input)	{
	if(0 == input.size())
		return 0;

	int							sizeNeededForMultiByte			= WideCharToMultiByte(CP_UTF8, 0, input.begin(), int(input.size()), nullptr, 0, nullptr, nullptr);
	if(0 == sizeNeededForMultiByte)
		return 0;

	::llc::apod<char>			converted;
	llc_necs(converted.resize(sizeNeededForMultiByte));
	WideCharToMultiByte(CP_UTF8, 0, input.begin(), input.size(), &converted[0], int(converted.size()), nullptr, nullptr);
	llc_necs(output.append(converted));
	return sizeNeededForMultiByte;
}

::llc::error_t			llc::mbstowcs					(::llc::apod<wchar_t> & output, ::llc::vcsc_c input)	{
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