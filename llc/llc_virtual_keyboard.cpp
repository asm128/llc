#define LLC_INFO_PRINTF_ENABLED

#include "llc_virtual_keyboard.h"

::llc::err_t			llc::virtualKeyboardHandleEvent	(const ::llc::SVirtualKeyboard & vk, ::llc::cid_t iControl, ::llc::apod<::llc::SVirtualKeyboardEvent> & events) {
	const ::llc::cid_t			idKeyFirst			= vk.IdRoot + 1;
	const ::llc::cid_t			idKeyStop			= ::llc::cid_t(idKeyFirst + vk.Keys.size());
	if(::llc::in_range<::llc::cid_t>(iControl, idKeyFirst, idKeyStop)) {
		const int32_t				iKey				= iControl - idKeyFirst;
		const uint16_t				key					= vk.Keys[iKey];
		info_printf("Virtual key %i: %c (%i)", iKey, key, key);
		return 1 + events.push_back({::llc::VK_EVENT_RELEASE, ::llc::VK_SCANCODE(key)});
	}
	else if(::llc::in_range<int32_t>(iControl, idKeyStop, idKeyStop + ::llc::get_value_count<::llc::VK_SCANCODE>())) {
		const ::llc::VK_SCANCODE	key					= ::llc::VK_SCANCODE(iControl - idKeyStop);
		info_printf("Virtual scancode: %s (%i)", ::llc::get_value_label(key).begin(), key);
		return 1 + events.push_back({::llc::VK_EVENT_EDIT, key});
	}
	return 0;
}
