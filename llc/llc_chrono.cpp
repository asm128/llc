#include "llc_chrono.h"
#include "llc_expression.h"
#include <time.h>

#ifndef LLC_ATMEL
#	include <chrono>
#endif

//    auto t0 = std::chrono::high_resolution_clock::now();
//    auto nanosec = t0.time_since_epoch();
//
//    std::cout << nanosec.count() << " nanoseconds since epoch" "\n";
//    std::cout << nanosec.count()/(1000000000.0 *60.0 *60.0) << " hours since epoch" "\n";
namespace llc 
{
#if defined(LLC_ATMEL)
	int64_t		timeCurrent		()	{ return time(0); }
	int64_t		timeCurrentInMs	()	{ return time(0) * 1000; }
	int64_t		timeCurrentInUs	()	{ return time(0) * 1000000; }
#else
	using ::std::chrono::time_point;
	using ::std::chrono::system_clock;
	using ::std::chrono::microseconds;
	using ::std::chrono::milliseconds;
	using ::std::chrono::duration_cast;

#	if defined(LLC_ANDROID) || defined(LLC_LINUX)
	long long	timeCurrent		()	{ time_point<system_clock> nowclock = system_clock::now(); return system_clock::to_time_t(nowclock); }
	long long	timeCurrentInUs	()	{ time_point<system_clock> nowclock = system_clock::now(); return duration_cast<microseconds>(nowclock.time_since_epoch()).count(); }
	long long	timeCurrentInMs	()	{ time_point<system_clock> nowclock = system_clock::now(); return duration_cast<milliseconds>(nowclock.time_since_epoch()).count(); }
#	else
	int64_t		timeCurrent		()	{ time_point<system_clock> nowclock = system_clock::now(); return system_clock::to_time_t(nowclock); }
	int64_t		timeCurrentInUs	()	{ time_point<system_clock> nowclock = system_clock::now(); return duration_cast<microseconds>(nowclock.time_since_epoch()).count(); }
	int64_t		timeCurrentInMs	()	{ time_point<system_clock> nowclock = system_clock::now(); return duration_cast<milliseconds>(nowclock.time_since_epoch()).count(); }
#	endif
#endif
} // namespace

::llc::err_t llc::timeStringFormat(::llc::vcst_t format, ::llc::view<cnst ::llc::SStripLiteralType> tokens, ::llc::s3_t timeInMs, ::llc::asc_t & output) {
	::llc::s3_t seconds = timeInMs / 1000, milliseconds = timeInMs % 1000;
	if(milliseconds < 0) { --seconds; milliseconds += 1000; }
	cnst ::time_t time = (::time_t)seconds;
	if_true_fef((::llc::s3_t)time != seconds, "Time is outside the platform range: %" LLC_FMT_S3 ".", seconds);
	::tm local = {};
#if defined(LLC_WINDOWS)
	if_true_fe(localtime_s(&local, &time));
#elif defined(LLC_ATMEL)
	cnst ::tm * converted = ::localtime(&time);
	if_null_fe(converted);
	local = *converted;
#else
	if_null_fe(::localtime_r(&time, &local));
#endif
	cnst ::llc::u2_t values[] = {
		(::llc::u2_t)((local.tm_year + 1900) % 100), (::llc::u2_t)(local.tm_mon + 1), (::llc::u2_t)local.tm_mday,
		(::llc::u2_t)local.tm_hour, (::llc::u2_t)local.tm_min, (::llc::u2_t)local.tm_sec, (::llc::u2_t)(milliseconds / 10)
	};
	::llc::sc_t digits[7][2] = {};
	for(::llc::u2_t i = 0; i < ::llc::size(values); ++i) {
		digits[i][0] = (::llc::sc_t)('0' + values[i] / 10);
		digits[i][1] = (::llc::sc_t)('0' + values[i] % 10);
	}
	cnst ::llc::keyval<::llc::vcst_t> bindings[] = {
		{"yy", {digits[0], 2}}, {"MM", {digits[1], 2}}, {"dd", {digits[2], 2}}, {"hh", {digits[3], 2}},
		{"mm", {digits[4], 2}}, {"ss", {digits[5], 2}}, {"ff", {digits[6], 2}}
	};
	rtrn ::llc::expressionStringFormat(format, tokens, [&](::llc::vcst_c & key, ::llc::vcst_t & resolved) {
		rtrn ::llc::find(key, ::llc::view<cnst ::llc::keyval<::llc::vcst_t>>{bindings}, resolved);
	}, output);
}

::llc::err_t llc::timeStringFormat(::llc::vcst_t format, ::llc::s3_t timeInMs, ::llc::asc_t & output) {
	::llc::apod<::llc::SStripLiteralType> tokens;
	::llc::SStripLiteralState state;
	if_fail_fe(::llc::stripLiteralParse(state, tokens, format));
	rtrn ::llc::timeStringFormat(format, tokens, timeInMs, output);
}
