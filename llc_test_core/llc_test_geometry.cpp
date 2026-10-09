#include "llc_align.h"
#include "llc_coord.h"
#include "llc_gauge.h"
#include "llc_line.h"
#include "llc_line2.h"
#include "llc_line3.h"
#include "llc_matrix2.h"
#include "llc_matrix3.h"
#include "llc_minmax_n3.h"
#include "llc_quad.h"
#include "llc_quad2.h"
#include "llc_quad3.h"
#include "llc_range_n2.h"
#include "llc_range_n3.h"
#include "llc_rect.h"
#include "llc_rect2.h"
#include "llc_rect3.h"
#include "llc_slice_n2.h"
#include "llc_slice_n3.h"
#include "llc_sphere.h"
#include "llc_tri2.h"
#include "llc_tri3.h"

#include "llc_test_core.h"

#include <type_traits>
#include <utility>

GDEFINE_ENUM_TYPE(GEOMETRY_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, TYPE_FAMILY	, 0, "A geometry family did not preserve its scalar type or current-width alias.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, VECTOR		, 1, "A vector operation produced the wrong result.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, LINE			, 2, "A line operation produced the wrong result.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, TRIANGLE		, 3, "A triangle operation produced the wrong result.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, REGION		, 4, "A quad, rectangle, range, slice or origin lost its represented values.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, GAUGE			, 5, "A gauge did not preserve its limits, value or weight.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, SPHERE		, 6, "A sphere size or overlap operation produced the wrong result.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, QUATERNION	, 7, "A quaternion operation produced the wrong result.");
GDEFINE_ENUM_VALUED(GEOMETRY_TEST_RESULT, MATRIX		, 8, "A matrix operation produced the wrong result.");

sttc bool geometryDiffers(::llc::f3_t left, ::llc::f3_t right, ::llc::f3_t tolerance = 0.00001) {
	rtrn false == (::llc::abs(left - right) <= tolerance);
}

tplt<tpnm T>
sttc ::llc::err_t geometryCheck(ATestError & errors, GEOMETRY_TEST_RESULT result, cnst ::llc::n2<T> & actual, cnst ::llc::n2<T> & expected, ::llc::vcst_t operation) {
	LLC_TEST_CHECK(errors, result, actual.x != expected.x
		, "%.*s x:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
		, (int)operation.size(), operation.begin(), (::llc::s3_t)actual.x, (::llc::s3_t)expected.x
		);
	LLC_TEST_CHECK(errors, result, actual.y != expected.y
		, "%.*s y:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
		, (int)operation.size(), operation.begin(), (::llc::s3_t)actual.y, (::llc::s3_t)expected.y
		);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t geometryCheck(ATestError & errors, GEOMETRY_TEST_RESULT result, cnst ::llc::n3<T> & actual, cnst ::llc::n3<T> & expected, ::llc::vcst_t operation) {
	LLC_TEST_CHECK(errors, result, actual.x != expected.x
		, "%.*s x:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
		, (int)operation.size(), operation.begin(), (::llc::s3_t)actual.x, (::llc::s3_t)expected.x
		);
	LLC_TEST_CHECK(errors, result, actual.y != expected.y
		, "%.*s y:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
		, (int)operation.size(), operation.begin(), (::llc::s3_t)actual.y, (::llc::s3_t)expected.y
		);
	LLC_TEST_CHECK(errors, result, actual.z != expected.z
		, "%.*s z:%" LLC_FMT_S3 ", expected:%" LLC_FMT_S3 "."
		, (int)operation.size(), operation.begin(), (::llc::s3_t)actual.z, (::llc::s3_t)expected.z
		);
	rtrn 0;
}

tplt<tpnm T, tpnm = void>
stct SGeometryTransforms3 : std::false_type {};

tplt<tpnm T>
stct SGeometryTransforms3<T, std::void_t<decltype(std::declval<cnst T &>().Transform(std::declval<cnst ::llc::n3f2_t &>()))>> : std::true_type {};

tplt<tpnm T, tpnm = void>
stct SGeometryTransformsPoint2 : std::false_type {};

tplt<tpnm T>
stct SGeometryTransformsPoint2<T, std::void_t<decltype(std::declval<cnst T &>().TransformPoint(std::declval<cnst ::llc::n2f2_t &>()))>> : std::true_type {};

tplt<tpnm T>
sttc ::llc::err_t testGeometryScalar(ATestError & errors) {
	static_assert(sizeof(::llc::line<T>) == sizeof(T) * 2);
	static_assert(sizeof(::llc::quad<T>) == sizeof(T) * 4);
	static_assert(sizeof(::llc::rect<T>) == sizeof(T) * 4);
	static_assert(sizeof(::llc::gaugemax<T>) == sizeof(T) * 2);
	static_assert(sizeof(::llc::gaugeminmax<T>) == sizeof(T) * 3);
	static_assert(sizeof(::llc::SOrigin<T>) == sizeof(T) * 9);
	static_assert(sizeof(::llc::quat<T>) == sizeof(T) * 4);
	static_assert(sizeof(::llc::m2<T>) == sizeof(T) * 4);
	static_assert(sizeof(::llc::m3<T>) == sizeof(T) * 9);
	static_assert(sizeof(::llc::m3a2<T>) == sizeof(T) * 9);
	static_assert(sizeof(::llc::m3a3<T>) == sizeof(T) * 9);
	static_assert(sizeof(::llc::m4<T>) == sizeof(T) * 16);
	static_assert(sizeof(::llc::sphere<T>) == sizeof(::llc::f3_t) + sizeof(T) * 3);

	cnst ::llc::n2<T> vector2 = {T(2), T(3)};
	cnst ::llc::n3<T> vector3 = {T(2), T(3), T(4)};
	cnst ::llc::tri2<T> triangle2 = {{T(1), T(2)}, {T(3), T(4)}, {T(5), T(6)}};
	cnst ::llc::tri3<T> triangle3 = {{T(1), T(2), T(3)}, {T(4), T(5), T(6)}, {T(7), T(8), T(9)}};
	cnst ::llc::quad2<T> quad2 = {{T(1), T(2)}, {T(3), T(4)}, {T(5), T(6)}, {T(7), T(8)}};
	cnst ::llc::quad3<T> quad3 = {{T(1), T(2), T(3)}, {T(4), T(5), T(6)}, {T(7), T(8), T(9)}, {T(10), T(11), T(12)}};
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, vector2.Area() != T(6)
		, "n2::Area, scalar size:%u, expected:6, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)vector2.Area());
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, vector3.Volume() != T(24)
		, "n3::Volume, scalar size:%u, expected:24, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)vector3.Volume());
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, triangle2.C.y != T(6)
		, "tri2::C.y, scalar size:%u, expected:6, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)triangle2.C.y);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, triangle3.C.z != T(9)
		, "tri3::C.z, scalar size:%u, expected:9, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)triangle3.C.z);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, quad2.D.y != T(8)
		, "quad2::D.y, scalar size:%u, expected:8, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)quad2.D.y);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_TYPE_FAMILY, quad3.D.z != T(12)
		, "quad3::D.z, scalar size:%u, expected:12, actual:%g.", (::llc::u2_t)sizeof(T), (::llc::f3_t)quad3.D.z);
	rtrn 0;
}

sttc ::llc::err_t testGeometryTypeFamily(ATestError & errors) {
	static_assert(std::is_same_v<::llc::tri2s2_t, ::llc::tri2<::llc::s2_t>>);
	static_assert(std::is_same_v<::llc::tri3s2_t, ::llc::tri3<::llc::s2_t>>);
	static_assert(std::is_same_v<::llc::line2f2_t, ::llc::line2<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::line3f3_t, ::llc::line3<::llc::f3_t>>);
	static_assert(std::is_same_v<::llc::quad2u1_t, ::llc::quad2<::llc::u1_t>>);
	static_assert(std::is_same_v<::llc::quad3s1_t, ::llc::quad3<::llc::s1_t>>);
	static_assert(std::is_same_v<::llc::rect3u3_t, ::llc::rect3<::llc::u3_t>>);
	static_assert(std::is_same_v<::llc::spheref2_t, ::llc::sphere<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::quatf3_t, ::llc::quat<::llc::f3_t>>);
	static_assert(std::is_same_v<::llc::m2f2_t, ::llc::m2<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::m3f2_t, ::llc::m3<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::m3a2f2_t, ::llc::m3a2<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::m3a3f2_t, ::llc::m3a3<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::m4f2_t, ::llc::m4<::llc::f2_t>>);
	static_assert(std::is_same_v<::llc::range3s3_t, ::llc::range<::llc::n3s3_t>>);
	static_assert(std::is_same_v<::llc::minmax3u2_t, ::llc::minmax<::llc::n3u2_t>>);
	static_assert(std::is_same_v<::llc::slice2s2_t, ::llc::slice<::llc::n2s2_t>>);
	static_assert(std::is_same_v<::llc::slice3s2_t, ::llc::slice<::llc::n3s2_t>>);
	static_assert(std::is_same_v<decltype(::llc::n3s2_t{}.Volume()), ::llc::s2_t>);
	static_assert(std::is_same_v<decltype(::llc::tri3s2_t{}.u2()), ::llc::tri3u2_t>);
	static_assert(std::is_same_v<decltype(::llc::tri3u2_t{}.s2()), ::llc::tri3s2_t>);
	static_assert(false == SGeometryTransforms3<::llc::m3f2_t>::value);
	static_assert(false == SGeometryTransformsPoint2<::llc::m3f2_t>::value);
	static_assert(SGeometryTransformsPoint2<::llc::m3a2f2_t>::value);
	static_assert(false == SGeometryTransforms3<::llc::m3a2f2_t>::value);
	static_assert(SGeometryTransforms3<::llc::m3a3f2_t>::value);
	static_assert(false == SGeometryTransformsPoint2<::llc::m3a3f2_t>::value);
	static_assert(false == std::is_convertible_v<::llc::m3a2f2_t, ::llc::m3a3f2_t>);
	static_assert(false == std::is_convertible_v<::llc::m3a3f2_t, ::llc::m3a2f2_t>);
	static_assert(std::is_same_v<decltype(::llc::m2f2_t{} * ::llc::m2f2_t{}), ::llc::m2f2_t>);
	static_assert(std::is_same_v<decltype(::llc::m3a2f2_t{} * ::llc::m3a2f2_t{}), ::llc::m3a2f2_t>);
	static_assert(std::is_same_v<decltype(::llc::m3a3f2_t{} * ::llc::m3a3f2_t{}), ::llc::m3a3f2_t>);
	static_assert(std::is_same_v<decltype(::llc::m4f2_t{} * ::llc::m4f2_t{}), ::llc::m4f2_t>);

	if_fail_fe(::testGeometryScalar<::llc::uc_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::sc_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::u0_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::u1_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::u2_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::u3_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::s0_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::s1_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::s2_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::s3_t>(errors));
	if_fail_fe(::testGeometryScalar<::llc::f2_t>(errors));
	rtrn ::testGeometryScalar<::llc::f3_t>(errors);
}

sttc ::llc::err_t testGeometryVectorsAndLines(ATestError & errors) {
	cnst ::llc::n3s2_t leftVector = {1, 2, 3};
	cnst ::llc::n3s2_t rightVector = {4, 5, 6};
	cnst ::llc::n3s2_t crossProduct = leftVector.Cross(rightVector);
	cnst ::llc::n3s2_t expectedCross = {-3, 6, -3};
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_VECTOR, leftVector.Dot(rightVector) != 32
		, "n3::Dot, expected:32, actual:%g.", leftVector.Dot(rightVector));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_VECTOR, crossProduct, expectedCross, LLC_CXS("n3::Cross")));

	cnst ::llc::line2s2_t segment2 = {{1, 2}, {4, 8}};
	cnst ::llc::line3s2_t segment3 = {{1, 2, 9}, {4, 8, 10}};
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_LINE, ::llc::rise(segment2) != 6
		, "rise, expected:6, actual:%i.", ::llc::rise(segment2));
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_LINE, ::llc::run(segment2) != 3
		, "run, expected:3, actual:%i.", ::llc::run(segment2));
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_LINE, ::llc::slope(segment2) != 2
		, "slope, expected:2, actual:%i.", ::llc::slope(segment2));
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_LINE, ::llc::orient2d(segment2, ::llc::n2s2_t{4, 2}) != -18
		, "orient2d, expected:-18, actual:%i.", ::llc::orient2d(segment2, ::llc::n2s2_t{4, 2}));
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_LINE, ::llc::orient2d3d(segment3, ::llc::n2s2_t{4, 2}) != -18
		, "orient2d3d, expected:-18, actual:%i.", ::llc::orient2d3d(segment3, ::llc::n2s2_t{4, 2}));
	rtrn 0;
}

sttc ::llc::err_t testGeometryTrianglesAndRegions(ATestError & errors) {
	::llc::tri3s2_t triangle = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	triangle.Translate({1, 2, 3}).Scale({2, 3, 4});
	cnst ::llc::n3s2_t expectedA = {4, 12, 24};
	cnst ::llc::n3s2_t expectedC = {16, 30, 48};
	cnst ::llc::n3s3_t expectedB = {10, 21, 36};
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_TRIANGLE, triangle.A, expectedA, LLC_CXS("tri3::Translate/Scale A")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_TRIANGLE, triangle.s3().B, expectedB, LLC_CXS("tri3::s3 B")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_TRIANGLE, triangle.C, expectedC, LLC_CXS("tri3::Translate/Scale C")));

	cnst ::llc::rects2_t bounds = {2, 3, 12, 23};
	cnst ::llc::rect2s2_t offsetSize = {{2, 3}, {10, 20}};
	cnst ::llc::rect3s2_t volumeBounds = {{1, 2, 3}, {4, 5, 6}};
	cnst ::llc::range2s2_t range = {{1, 2}, {3, 4}};
	cnst ::llc::slice3s2_t slice = {{1, 2, 3}, {4, 5, 6}};
	cnst ::llc::SOrigin<::llc::s2_t> origin = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	cnst ::llc::n2s2_t dimensions = bounds.Dimensions();
	::llc::n2s2_t aligned = {};
	::llc::realignCoord(::llc::n2s2_t{10, 10}, ::llc::n2s2_t{1, 2}, aligned, ::llc::ALIGN_CENTER_TOP);
	cnst ::llc::n2s2_t expectedDimensions = {10, 20};
	cnst ::llc::n2s2_t expectedLimit2 = {12, 23};
	cnst ::llc::n3s2_t expectedLimit3 = {5, 7, 9};
	cnst ::llc::n2s2_t expectedCount = {3, 4};
	cnst ::llc::n3s2_t expectedEnd = {4, 5, 6};
	cnst ::llc::n3s2_t expectedRight = {0, 0, 1};
	cnst ::llc::n2s2_t expectedAligned = {6, 2};
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, dimensions, expectedDimensions, LLC_CXS("rect::Dimensions")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, offsetSize.Limit(), expectedLimit2, LLC_CXS("rect2::Limit")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, volumeBounds.Limit(), expectedLimit3, LLC_CXS("rect3::Limit")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, range.Count, expectedCount, LLC_CXS("range2::Count")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, slice.End, expectedEnd, LLC_CXS("slice3::End")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, origin.Right, expectedRight, LLC_CXS("SOrigin::Right")));
	if_fail_fe(geometryCheck(errors, GEOMETRY_TEST_RESULT_REGION, aligned, expectedAligned, LLC_CXS("realignCoord CENTER_TOP")));
	rtrn 0;
}

sttc ::llc::err_t testGeometryGaugeAndSphere(ATestError & errors) {
	::llc::gaugemaxs2_t maximum = {100, 25};
	::llc::gaugeminmaxs2_t interval = {{-20, 20}, 0};
	maximum.SetWeighted(.75);
	interval.SetWeighted(.25);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_GAUGE, maximum.Value != 75
		, "gaugemax::SetWeighted value, expected:75, actual:%i.", maximum.Value);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_GAUGE, geometryDiffers(maximum.Weight(), .75)
		, "gaugemax::Weight, expected:0.75, actual:%g.", maximum.Weight());
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_GAUGE, interval.Value != -10
		, "gaugeminmax::SetWeighted value, expected:-10, actual:%i.", interval.Value);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_GAUGE, geometryDiffers(interval.Weight(), .25)
		, "gaugeminmax::Weight, expected:0.25, actual:%g.", interval.Weight());

	cnst ::llc::spheref2_t firstSphere = {2, {0, 0, 0}};
	cnst ::llc::spheref2_t touchingSphere = {2, {3, 0, 0}};
	cnst ::llc::spheref2_t separateSphere = {2, {5, 0, 0}};
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_SPHERE, false == ::llc::sphereOverlaps(firstSphere, touchingSphere)
		, "sphereOverlaps touching spheres, expected:true, actual:false.");
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_SPHERE, ::llc::sphereOverlaps(firstSphere, separateSphere)
		, "sphereOverlaps separate spheres, expected:false, actual:true.");
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_SPHERE, geometryDiffers(::llc::sphereSize(firstSphere), 1.3333333333333333 * ::llc::math_pi * 8)
		, "sphereSize, expected:%g, actual:%g.", 1.3333333333333333 * ::llc::math_pi * 8, ::llc::sphereSize(firstSphere));
	rtrn 0;
}

sttc ::llc::err_t testGeometryQuaternion(ATestError & errors) {
	cnst ::llc::quatf2_t identity = {0, 0, 0, 1};
	::llc::quatf2_t quarterTurn = {};
	quarterTurn.CreateFromAxisAngle({0, 0, 1}, ::llc::math_pi_2).Normalize();
	cnst ::llc::n3f2_t rotated = quarterTurn.RotateVector({1, 0, 0});
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(quarterTurn.Length(), 1)
		, "quat::Normalize, expected length:1, actual:%g.", quarterTurn.Length());
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotated.x, 0)
		, "quat::RotateVector quarter turn x:%g, expected:0.", (::llc::f3_t)rotated.x);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotated.y, 1)
		, "quat::RotateVector quarter turn y:%g, expected:1.", (::llc::f3_t)rotated.y);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotated.z, 0)
		, "quat::RotateVector quarter turn z:%g, expected:0.", (::llc::f3_t)rotated.z);

	::llc::quatf2_t midpoint = {};
	midpoint.SLERP(identity, quarterTurn, .5);
	cnst ::llc::n3f2_t rotatedHalf = midpoint.RotateVector({1, 0, 0});
	cnst ::llc::f3_t halfRoot = ::sqrt(.5);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(midpoint.Length(), 1)
		, "quat::SLERP midpoint, expected length:1, actual:%g.", midpoint.Length());
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedHalf.x, (::llc::f2_t)halfRoot)
		, "quat::SLERP midpoint rotation x:%g, expected:%g.", (::llc::f3_t)rotatedHalf.x, halfRoot);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedHalf.y, (::llc::f2_t)halfRoot)
		, "quat::SLERP midpoint rotation y:%g, expected:%g.", (::llc::f3_t)rotatedHalf.y, halfRoot);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedHalf.z, 0)
		, "quat::SLERP midpoint rotation z:%g, expected:0.", (::llc::f3_t)rotatedHalf.z);

	::llc::quatf2_t shortest = {};
	shortest.SLERP(identity, -quarterTurn, .5);
	cnst ::llc::n3f2_t rotatedShortest = shortest.RotateVector({1, 0, 0});
	::llc::quatf2_t unchanged = {};
	unchanged.SLERP(identity, identity, .5);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedShortest.x, (::llc::f2_t)halfRoot)
		, "quat::SLERP shortest path x:%g, expected:%g.", (::llc::f3_t)rotatedShortest.x, halfRoot);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedShortest.y, (::llc::f2_t)halfRoot)
		, "quat::SLERP shortest path y:%g, expected:%g.", (::llc::f3_t)rotatedShortest.y, halfRoot);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, geometryDiffers(rotatedShortest.z, 0)
		, "quat::SLERP shortest path z:%g, expected:0.", (::llc::f3_t)rotatedShortest.z);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, unchanged.x != identity.x
		, "quat::SLERP identical endpoints x:%g, expected:%g.", (::llc::f3_t)unchanged.x, (::llc::f3_t)identity.x);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, unchanged.y != identity.y
		, "quat::SLERP identical endpoints y:%g, expected:%g.", (::llc::f3_t)unchanged.y, (::llc::f3_t)identity.y);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, unchanged.z != identity.z
		, "quat::SLERP identical endpoints z:%g, expected:%g.", (::llc::f3_t)unchanged.z, (::llc::f3_t)identity.z);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_QUATERNION, unchanged.w != identity.w
		, "quat::SLERP identical endpoints w:%g, expected:%g.", (::llc::f3_t)unchanged.w, (::llc::f3_t)identity.w);
	rtrn 0;
}

tplt<::llc::MATRIX_MATH _math, ::llc::MATRIX_LAYOUT _layout>
sttc ::llc::err_t testGeometryMatrixPolicy(ATestError & errors) {
	::llc::m2<::llc::f2_t, _math, _layout> linear2 = {};
	linear2.Rotation(::llc::math_pi_2);
	cnst ::llc::n2f2_t rotated2 = linear2.Transform({1, 0});
	cnst ::llc::n2f2_t restored2 = linear2.TransformInverse(rotated2);

	::llc::m3a2<::llc::f2_t, _math, _layout> affine2 = {};
	affine2.Identity();
	affine2.Scale({2, 3}, false);
	affine2.SetTranslation({1, 2}, false);
	cnst ::llc::n2f2_t transformedPoint2 = affine2.TransformPoint({4, 5});
	cnst ::llc::n2f2_t transformedDirection2 = affine2.TransformDirection({4, 5});
	cnst ::llc::n2f2_t restoredPoint2 = affine2.GetInverse().TransformPoint(transformedPoint2);
	::llc::m3a2<::llc::f2_t, _math, _layout> projective2 = {};
	projective2.Identity();
	projective2.MathElement(2, 2) = 2;
	cnst ::llc::n2f2_t dividedPoint2 = projective2.TransformPoint({4, 6});

	::llc::m3a3<::llc::f2_t, _math, _layout> linear3 = {};
	linear3.RotationZ(::llc::math_pi_2);
	cnst ::llc::n3f2_t rotated3 = linear3.Transform({1, 0, 0});
	cnst ::llc::n3f2_t restored3 = linear3.TransformInverse(rotated3);

	::llc::m4<::llc::f2_t, _math, _layout> affine3 = {};
	affine3.Identity();
	affine3.Scale({2, 3, 4}, false);
	affine3.SetTranslation({1, 2, 3}, false);
	cnst ::llc::n3f2_t transformedPoint3 = affine3.Transform({4, 5, 6});
	cnst ::llc::n3f2_t transformedDirection3 = affine3.TransformDirection({4, 5, 6});
	cnst ::llc::n3f2_t restoredPoint3 = affine3.GetInverse().Transform(transformedPoint3);
	cnst ::llc::n2f2_t expectedPoint2 = {9, 17};
	cnst ::llc::n2f2_t expectedDirection2 = {8, 15};
	cnst ::llc::n2f2_t expectedDividedPoint2 = {2, 3};
	cnst ::llc::n3f2_t expectedPoint3 = {9, 17, 27};
	cnst ::llc::n3f2_t expectedDirection3 = {8, 15, 24};

	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rotated2.x, 0)
		, "m2::Rotation x:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)rotated2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rotated2.y, 1)
		, "m2::Rotation y:%g, expected:1, math:%u, layout:%u."
		, (::llc::f3_t)rotated2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restored2.x, 1)
		, "m2::TransformInverse x:%g, expected:1, math:%u, layout:%u."
		, (::llc::f3_t)restored2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restored2.y, 0)
		, "m2::TransformInverse y:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)restored2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);

	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedPoint2.x != expectedPoint2.x
		, "m3a2::TransformPoint x:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedPoint2.x, (::llc::f3_t)expectedPoint2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedPoint2.y != expectedPoint2.y
		, "m3a2::TransformPoint y:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedPoint2.y, (::llc::f3_t)expectedPoint2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedDirection2.x != expectedDirection2.x
		, "m3a2::TransformDirection x:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedDirection2.x, (::llc::f3_t)expectedDirection2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedDirection2.y != expectedDirection2.y
		, "m3a2::TransformDirection y:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedDirection2.y, (::llc::f3_t)expectedDirection2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restoredPoint2.x, 4)
		, "m3a2::GetInverse/TransformPoint x:%g, expected:4, math:%u, layout:%u."
		, (::llc::f3_t)restoredPoint2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restoredPoint2.y, 5)
		, "m3a2::GetInverse/TransformPoint y:%g, expected:5, math:%u, layout:%u."
		, (::llc::f3_t)restoredPoint2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, dividedPoint2.x != expectedDividedPoint2.x
		, "m3a2::TransformPoint homogeneous divide x:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)dividedPoint2.x, (::llc::f3_t)expectedDividedPoint2.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, dividedPoint2.y != expectedDividedPoint2.y
		, "m3a2::TransformPoint homogeneous divide y:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)dividedPoint2.y, (::llc::f3_t)expectedDividedPoint2.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);

	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rotated3.x, 0)
		, "m3a3::RotationZ x:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)rotated3.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rotated3.y, 1)
		, "m3a3::RotationZ y:%g, expected:1, math:%u, layout:%u."
		, (::llc::f3_t)rotated3.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rotated3.z, 0)
		, "m3a3::RotationZ z:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)rotated3.z, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restored3.x, 1)
		, "m3a3::TransformInverse x:%g, expected:1, math:%u, layout:%u."
		, (::llc::f3_t)restored3.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restored3.y, 0)
		, "m3a3::TransformInverse y:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)restored3.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restored3.z, 0)
		, "m3a3::TransformInverse z:%g, expected:0, math:%u, layout:%u."
		, (::llc::f3_t)restored3.z, (::llc::u2_t)_math, (::llc::u2_t)_layout);

	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedPoint3.x != expectedPoint3.x
		, "m4::Transform x:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedPoint3.x, (::llc::f3_t)expectedPoint3.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedPoint3.y != expectedPoint3.y
		, "m4::Transform y:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedPoint3.y, (::llc::f3_t)expectedPoint3.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedPoint3.z != expectedPoint3.z
		, "m4::Transform z:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedPoint3.z, (::llc::f3_t)expectedPoint3.z, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedDirection3.x != expectedDirection3.x
		, "m4::TransformDirection x:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedDirection3.x, (::llc::f3_t)expectedDirection3.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedDirection3.y != expectedDirection3.y
		, "m4::TransformDirection y:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedDirection3.y, (::llc::f3_t)expectedDirection3.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, transformedDirection3.z != expectedDirection3.z
		, "m4::TransformDirection z:%g, expected:%g, math:%u, layout:%u."
		, (::llc::f3_t)transformedDirection3.z, (::llc::f3_t)expectedDirection3.z, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restoredPoint3.x, 4)
		, "m4::GetInverse/Transform x:%g, expected:4, math:%u, layout:%u."
		, (::llc::f3_t)restoredPoint3.x, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restoredPoint3.y, 5)
		, "m4::GetInverse/Transform y:%g, expected:5, math:%u, layout:%u."
		, (::llc::f3_t)restoredPoint3.y, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(restoredPoint3.z, 6)
		, "m4::GetInverse/Transform z:%g, expected:6, math:%u, layout:%u."
		, (::llc::f3_t)restoredPoint3.z, (::llc::u2_t)_math, (::llc::u2_t)_layout);
	rtrn 0;
}

sttc ::llc::err_t testGeometryMatrixStorage(ATestError & errors) {
	::llc::m2<::llc::f2_t, ::llc::MATRIX_MATH_ROW_VECTOR, ::llc::MATRIX_LAYOUT_ROW_MAJOR> rowVectorRowMajor = {};
	::llc::m2<::llc::f2_t, ::llc::MATRIX_MATH_ROW_VECTOR, ::llc::MATRIX_LAYOUT_COLUMN_MAJOR> rowVectorColumnMajor = {};
	::llc::m2<::llc::f2_t, ::llc::MATRIX_MATH_COLUMN_VECTOR, ::llc::MATRIX_LAYOUT_ROW_MAJOR> columnVectorRowMajor = {};
	::llc::m2<::llc::f2_t, ::llc::MATRIX_MATH_COLUMN_VECTOR, ::llc::MATRIX_LAYOUT_COLUMN_MAJOR> columnVectorColumnMajor = {};
	rowVectorRowMajor.Rotation(::llc::math_pi_2);
	rowVectorColumnMajor.Rotation(::llc::math_pi_2);
	columnVectorRowMajor.Rotation(::llc::math_pi_2);
	columnVectorColumnMajor.Rotation(::llc::math_pi_2);

	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rowVectorRowMajor.Value[1], 1)
		, "m2 row-vector/row-major Value[1], expected:1, actual:%g.", rowVectorRowMajor.Value[1]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rowVectorRowMajor.Value[2], -1)
		, "m2 row-vector/row-major Value[2], expected:-1, actual:%g.", rowVectorRowMajor.Value[2]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rowVectorColumnMajor.Value[1], -1)
		, "m2 row-vector/column-major Value[1], expected:-1, actual:%g.", rowVectorColumnMajor.Value[1]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(rowVectorColumnMajor.Value[2], 1)
		, "m2 row-vector/column-major Value[2], expected:1, actual:%g.", rowVectorColumnMajor.Value[2]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(columnVectorRowMajor.Value[1], -1)
		, "m2 column-vector/row-major Value[1], expected:-1, actual:%g.", columnVectorRowMajor.Value[1]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(columnVectorRowMajor.Value[2], 1)
		, "m2 column-vector/row-major Value[2], expected:1, actual:%g.", columnVectorRowMajor.Value[2]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(columnVectorColumnMajor.Value[1], 1)
		, "m2 column-vector/column-major Value[1], expected:1, actual:%g.", columnVectorColumnMajor.Value[1]);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(columnVectorColumnMajor.Value[2], -1)
		, "m2 column-vector/column-major Value[2], expected:-1, actual:%g.", columnVectorColumnMajor.Value[2]);
	rtrn 0;
}

sttc ::llc::err_t testGeometryMatrixTransposeAndInertia(ATestError & errors) {
	::llc::m4f2_t matrix4Source = {};
	::llc::m3a3f2_t matrix3Source = {};
	for(::llc::u0_t row = 0; row < 4; ++row) {
		for(::llc::u0_t column = 0; column < 4; ++column) {
			matrix4Source(row, column) = (::llc::f2_t)(row * 4 + column + 1);
		}
	}
	for(::llc::u0_t row = 0; row < 3; ++row) {
		for(::llc::u0_t column = 0; column < 3; ++column) {
			matrix3Source(row, column) = (::llc::f2_t)(row * 3 + column + 1);
		}
	}

	::llc::m4f2_t matrix4Transpose = {};
	::llc::m3a3f2_t matrix3Transpose = {};
	matrix4Transpose.Transpose(matrix4Source);
	matrix3Transpose.Transpose(matrix3Source);
	for(::llc::u0_t row = 0; row < 4; ++row) {
		for(::llc::u0_t column = 0; column < 4; ++column) {
			LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, matrix4Transpose(row, column) != matrix4Source(column, row)
				, "m4::Transpose(%u, %u), expected:%g, actual:%g."
				, (::llc::u2_t)row, (::llc::u2_t)column, matrix4Source(column, row), matrix4Transpose(row, column));
		}
	}
	for(::llc::u0_t row = 0; row < 3; ++row) {
		for(::llc::u0_t column = 0; column < 3; ++column) {
			LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, matrix3Transpose(row, column) != matrix3Source(column, row)
				, "m3a3::Transpose(%u, %u), expected:%g, actual:%g."
				, (::llc::u2_t)row, (::llc::u2_t)column, matrix3Source(column, row), matrix3Transpose(row, column));
		}
	}

	::llc::m3a3f2_t blockInertia = {};
	blockInertia.SetBlockAngularMass({1, 2, 3}, 10);
	cnst ::llc::f3_t factorX = blockInertia(0, 0) / (2 * 2 + 3 * 3);
	cnst ::llc::f3_t factorY = blockInertia(1, 1) / (1 * 1 + 3 * 3);
	cnst ::llc::f3_t factorZ = blockInertia(2, 2) / (1 * 1 + 2 * 2);
	cnst ::llc::f3_t expectedFactor = 10.0 / 3;
	for(::llc::u0_t row = 0; row < 3; ++row) {
		for(::llc::u0_t column = 0; column < 3; ++column) {
			if(row != column) {
				LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, 0 != blockInertia(row, column)
					, "m3a3::SetBlockAngularMass(%u, %u), expected:0, actual:%g."
					, (::llc::u2_t)row, (::llc::u2_t)column, blockInertia(row, column));
			}
		}
	}
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(factorX, expectedFactor)
		, "m3a3::SetBlockAngularMass X factor, expected:%g, actual:%g.", expectedFactor, factorX);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(factorY, expectedFactor)
		, "m3a3::SetBlockAngularMass Y factor, expected:%g, actual:%g.", expectedFactor, factorY);
	LLC_TEST_CHECK(errors, GEOMETRY_TEST_RESULT_MATRIX, geometryDiffers(factorZ, expectedFactor)
		, "m3a3::SetBlockAngularMass Z factor, expected:%g, actual:%g.", expectedFactor, factorZ);
	rtrn 0;
}

sttc ::llc::err_t testGeometryMatrix(ATestError & errors) {
	if_fail_fe((::testGeometryMatrixPolicy<::llc::MATRIX_MATH_ROW_VECTOR, ::llc::MATRIX_LAYOUT_ROW_MAJOR>(errors)));
	if_fail_fe((::testGeometryMatrixPolicy<::llc::MATRIX_MATH_ROW_VECTOR, ::llc::MATRIX_LAYOUT_COLUMN_MAJOR>(errors)));
	if_fail_fe((::testGeometryMatrixPolicy<::llc::MATRIX_MATH_COLUMN_VECTOR, ::llc::MATRIX_LAYOUT_ROW_MAJOR>(errors)));
	if_fail_fe((::testGeometryMatrixPolicy<::llc::MATRIX_MATH_COLUMN_VECTOR, ::llc::MATRIX_LAYOUT_COLUMN_MAJOR>(errors)));
	if_fail_fe(::testGeometryMatrixStorage(errors));
	rtrn ::testGeometryMatrixTransposeAndInertia(errors);
}

::llc::err_t testGeometry(ATestError & errors) {
	if_fail_fe(::testGeometryTypeFamily(errors));
	if_fail_fe(::testGeometryVectorsAndLines(errors));
	if_fail_fe(::testGeometryTrianglesAndRegions(errors));
	if_fail_fe(::testGeometryGaugeAndSphere(errors));
	if_fail_fe(::testGeometryQuaternion(errors));
	rtrn ::testGeometryMatrix(errors);
}
