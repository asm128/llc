#include "llc_test_core.h"
#include "llc_apod_minmax.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(ARRAY_MINMAX_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, OK					,  0, "All aminmax<> and minmax<> tests passed.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ALIAS_TYPE			,  1, "An aminmax<> alias did not select its current LLC scalar type.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, FROM_MIN				,  2, "minmax<>::from() produced the wrong minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, FROM_MAX				,  3, "minmax<>::from() produced the wrong maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, LENGTH				,  4, "minmax<>::Length() produced the wrong interval length.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, MIDDLE				,  5, "minmax<>::Middle() produced the wrong midpoint.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CLAMP_LOW			,  6, "minmax<>::Clamp() did not clamp a value below the interval.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CLAMP_INSIDE		,  7, "minmax<>::Clamp() changed a value inside the interval.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CLAMP_HIGH			,  8, "minmax<>::Clamp() did not clamp a value above the interval.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, WEIGHTED				,  9, "minmax<>::Weighted() produced the wrong value.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, WEIGHT				, 10, "minmax<>::Weight() produced the wrong normalized position.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, WEIGHT_CLAMP_LOW	, 11, "minmax<>::WeightClamp() did not clamp below the interval.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, WEIGHT_CLAMP_HIGH	, 12, "minmax<>::WeightClamp() did not clamp above the interval.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CAST_MIN				, 13, "minmax<>::Cast() changed the minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CAST_MAX				, 14, "minmax<>::Cast() changed the maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, SET_MIN				, 15, "minmax<>::Set() produced the wrong minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, SET_MAX				, 16, "minmax<>::Set() produced the wrong maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, FROM_MEMBER_MIN		, 17, "minmax<>::From() produced the wrong minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, FROM_MEMBER_MAX		, 18, "minmax<>::From() produced the wrong maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_SIZE			, 19, "aminmax<> stored the wrong number of ranges.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_FIRST_MIN		, 20, "aminmax<> changed the first range minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_FIRST_MAX		, 21, "aminmax<> changed the first range maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_SECOND_MIN	, 22, "aminmax<> changed the second range minimum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_SECOND_MAX	, 23, "aminmax<> changed the second range maximum.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_COPY_STORAGE	, 24, "Copying aminmax<> shared its backing storage.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_COPY_VALUE	, 25, "Copying aminmax<> changed a stored endpoint.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, ARRAY_COPY_INDEPENDENCE, 26, "Mutating a copied aminmax<> changed its source.");
GDEFINE_ENUM_VALUED(ARRAY_MINMAX_TEST_RESULT, CONST_ALIAS_TYPE		, 27, "An aminmax<> const alias did not preserve constness.");

tplt<tpnm T>
sttc ::llc::err_t testMinMaxType(::llc::ATestError & errors) {
	static_assert(::std::is_same_v<::llc::aminmax<T>, ::llc::apod<::llc::minmax<T>>>);
	cnst ::llc::minmax<T> single = ::llc::minmax<T>::from(T(7));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_FROM_MIN, single.Min != T(7), "Scalar bits:%u, minimum:%g, expected:7.", bcof(T), ::llc::f3_t(single.Min));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_FROM_MAX, single.Max != T(7), "Scalar bits:%u, maximum:%g, expected:7.", bcof(T), ::llc::f3_t(single.Max));

	::llc::minmax<T> interval = {T(2), T(10)};
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_LENGTH, interval.Length() != T(8), "Scalar bits:%u, length:%g, expected:8.", bcof(T), ::llc::f3_t(interval.Length()));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_MIDDLE, interval.Middle() != T(6), "Scalar bits:%u, midpoint:%g, expected:6.", bcof(T), ::llc::f3_t(interval.Middle()));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_CLAMP_LOW, interval.Clamp(T(0)) != T(2), "Scalar bits:%u, clamped low:%g, expected:2.", bcof(T), ::llc::f3_t(interval.Clamp(T(0))));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_CLAMP_INSIDE, interval.Clamp(T(6)) != T(6), "Scalar bits:%u, clamped inside:%g, expected:6.", bcof(T), ::llc::f3_t(interval.Clamp(T(6))));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_CLAMP_HIGH, interval.Clamp(T(12)) != T(10), "Scalar bits:%u, clamped high:%g, expected:10.", bcof(T), ::llc::f3_t(interval.Clamp(T(12))));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_WEIGHTED, interval.Weighted(.25) != T(4), "Scalar bits:%u, weighted value:%g, expected:4.", bcof(T), ::llc::f3_t(interval.Weighted(.25)));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_WEIGHT, interval.Weight(T(4)) != .25, "Scalar bits:%u, weight:%g, expected:0.25.", bcof(T), interval.Weight(T(4)));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_WEIGHT_CLAMP_LOW, interval.WeightClamp(T(0)) != 0, "Scalar bits:%u, low clamped weight:%g, expected:0.", bcof(T), interval.WeightClamp(T(0)));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_WEIGHT_CLAMP_HIGH, interval.WeightClamp(T(12)) != 1, "Scalar bits:%u, high clamped weight:%g, expected:1.", bcof(T), interval.WeightClamp(T(12)));
	cnst ::llc::minmax<::llc::s3_t> converted = interval.s3();
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_CAST_MIN, converted.Min != 2, "Scalar bits:%u, converted minimum:%" LLC_FMT_S3 ", expected:2.", bcof(T), converted.Min);
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_CAST_MAX, converted.Max != 10, "Scalar bits:%u, converted maximum:%" LLC_FMT_S3 ", expected:10.", bcof(T), converted.Max);
	interval.Set(T(5));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_SET_MIN, interval.Min != T(5), "Scalar bits:%u, Set minimum:%g, expected:5.", bcof(T), ::llc::f3_t(interval.Min));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_SET_MAX, interval.Max != T(5), "Scalar bits:%u, Set maximum:%g, expected:5.", bcof(T), ::llc::f3_t(interval.Max));
	interval.From(T(9));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_FROM_MEMBER_MIN, interval.Min != T(9), "Scalar bits:%u, From minimum:%g, expected:9.", bcof(T), ::llc::f3_t(interval.Min));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_FROM_MEMBER_MAX, interval.Max != T(9), "Scalar bits:%u, From maximum:%g, expected:9.", bcof(T), ::llc::f3_t(interval.Max));

	::llc::aminmax<T> ranges = {{T(2), T(10)}, {T(20), T(30)}};
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_SIZE, ranges.size() != 2, "Scalar bits:%u, range count:%u, expected:2.", bcof(T), ranges.size());
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_FIRST_MIN, ranges[0].Min != T(2), "Scalar bits:%u, first minimum:%g, expected:2.", bcof(T), ::llc::f3_t(ranges[0].Min));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_FIRST_MAX, ranges[0].Max != T(10), "Scalar bits:%u, first maximum:%g, expected:10.", bcof(T), ::llc::f3_t(ranges[0].Max));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_SECOND_MIN, ranges[1].Min != T(20), "Scalar bits:%u, second minimum:%g, expected:20.", bcof(T), ::llc::f3_t(ranges[1].Min));
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_SECOND_MAX, ranges[1].Max != T(30), "Scalar bits:%u, second maximum:%g, expected:30.", bcof(T), ::llc::f3_t(ranges[1].Max));
	::llc::aminmax<T> copied = ranges;
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_COPY_STORAGE, copied.begin() == ranges.begin(), "Scalar bits:%u, copied storage:%p, source storage:%p.", bcof(T), copied.begin(), ranges.begin());
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_COPY_VALUE, copied[1].Max != ranges[1].Max, "Scalar bits:%u, copied second maximum:%g, expected:%g.", bcof(T), ::llc::f3_t(copied[1].Max), ::llc::f3_t(ranges[1].Max));
	copied[0].Min = T(3);
	LLC_TEST_CHECKF(errors, ARRAY_MINMAX_TEST_RESULT_ARRAY_COPY_INDEPENDENCE, ranges[0].Min != T(2), "Scalar bits:%u, source minimum:%g, expected:2.", bcof(T), ::llc::f3_t(ranges[0].Min));
	return 0;
}

::llc::err_t testArrayMinMax(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxsc_t, ::llc::aminmax<::llc::sc_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxuc_t, ::llc::aminmax<::llc::uc_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu0_t, ::llc::aminmax<::llc::u0_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu1_t, ::llc::aminmax<::llc::u1_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu2_t, ::llc::aminmax<::llc::u2_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu3_t, ::llc::aminmax<::llc::u3_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs0_t, ::llc::aminmax<::llc::s0_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs1_t, ::llc::aminmax<::llc::s1_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs2_t, ::llc::aminmax<::llc::s2_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs3_t, ::llc::aminmax<::llc::s3_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxf2_t, ::llc::aminmax<::llc::f2_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxf3_t, ::llc::aminmax<::llc::f3_t>>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxsc_c, cnst ::llc::aminmaxsc_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxuc_c, cnst ::llc::aminmaxuc_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu0_c, cnst ::llc::aminmaxu0_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu1_c, cnst ::llc::aminmaxu1_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu2_c, cnst ::llc::aminmaxu2_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxu3_c, cnst ::llc::aminmaxu3_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs0_c, cnst ::llc::aminmaxs0_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs1_c, cnst ::llc::aminmaxs1_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs2_c, cnst ::llc::aminmaxs2_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxs3_c, cnst ::llc::aminmaxs3_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxf2_c, cnst ::llc::aminmaxf2_t>));
	LLC_TEST_CHECK(errors, ARRAY_MINMAX_TEST_RESULT_CONST_ALIAS_TYPE, (false == ::std::is_same_v<::llc::aminmaxf3_c, cnst ::llc::aminmaxf3_t>));
	if_fail_fe(testMinMaxType<::llc::sc_t>(errors));
	if_fail_fe(testMinMaxType<::llc::uc_t>(errors));
	if_fail_fe(::llc::testTypePack(::llc::integer_bounds_types{}, [&](auto type) { rtrn testMinMaxType<typename decltype(type)::type>(errors); }));
	if_fail_fe(testMinMaxType<::llc::f2_t>(errors));
	return testMinMaxType<::llc::f3_t>(errors);
}
