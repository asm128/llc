#include "llc_collision.h"

#include "llc_test_core.h"

GDEFINE_ENUM_TYPE(COLLISION_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, COORDINATE_INTEGER			,  0, "collisionCoordinateCast() rejected an exactly representable integer coordinate.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, COORDINATE_FRACTION			,  1, "collisionCoordinateCast() accepted a fractional integer coordinate or changed its output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, COORDINATE_RANGE				,  2, "collisionCoordinateCast() accepted an out-of-range coordinate or changed its output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_CROSS_RESULT				,  3, "line_line_intersect() did not report a unique crossing.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_CROSS_POINT				,  4, "line_line_intersect() produced the wrong crossing point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_REVERSED_RESULT			,  5, "Reversing both lines changed the unique-intersection result.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_REVERSED_POINT			,  6, "Reversing both lines changed their intersection point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_PARALLEL_RESULT			,  7, "Parallel lines reported a unique intersection.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_PARALLEL_PRESERVED		,  8, "Parallel lines changed the caller's intersection output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_COINCIDENT_RESULT		,  9, "Coincident lines reported a unique intersection.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_COINCIDENT_PRESERVED	, 10, "Coincident lines changed the caller's intersection output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_INTEGER_RESULT			, 11, "An exactly representable integer-line intersection was rejected.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_INTEGER_POINT			, 12, "An integer-line intersection produced the wrong point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_FRACTION_RESULT			, 13, "A fractional intersection was accepted by an integer output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_FRACTION_PRESERVED		, 14, "A rejected fractional integer intersection changed its output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_INTERIOR				, 15, "point_in_segment() rejected an interior collinear point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_ENDPOINT				, 16, "point_in_segment() rejected a segment endpoint.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_REVERSED				, 17, "Reversing a segment changed point membership.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_OFF_LINE				, 18, "point_in_segment() accepted a non-collinear point inside the bounding box.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_BEYOND					, 19, "point_in_segment() accepted a collinear point beyond an endpoint.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_DEGENERATE_HIT			, 20, "A zero-length segment rejected its only point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, POINT_DEGENERATE_MISS		, 21, "A zero-length segment accepted another point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_SEGMENT_RESULT			, 22, "line_segment_intersect() did not report a crossing inside the segment.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_SEGMENT_POINT			, 23, "line_segment_intersect() produced the wrong crossing point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_SEGMENT_MISS_RESULT		, 24, "line_segment_intersect() accepted a crossing outside the segment.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, LINE_SEGMENT_MISS_PRESERVED	, 25, "A line-segment miss changed the caller's output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_CROSS_RESULT			, 26, "segment_segment_intersect() did not report an interior crossing.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_CROSS_POINT			, 27, "segment_segment_intersect() produced the wrong interior crossing.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_ENDPOINT_RESULT		, 28, "segment_segment_intersect() rejected a shared endpoint.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_ENDPOINT_POINT		, 29, "segment_segment_intersect() produced the wrong shared endpoint.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_MISS_RESULT			, 30, "segment_segment_intersect() accepted an intersection outside one segment.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_MISS_PRESERVED		, 31, "A segment-segment miss changed the caller's output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_COINCIDENT_RESULT		, 32, "Overlapping collinear segments reported one unique intersection.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, SEGMENT_COINCIDENT_PRESERVED	, 33, "Overlapping collinear segments changed the caller's output.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TOWARD_RESULT				, 34, "intersectRaySphere() missed a ray aimed at the sphere.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TOWARD_PARAMETER			, 35, "intersectRaySphere() produced the wrong nearest parameter.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TOWARD_POINT				, 36, "intersectRaySphere() produced the wrong nearest point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_NON_UNIT_RESULT			, 37, "intersectRaySphere() rejected a non-unit direction.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_NON_UNIT_PARAMETER		, 38, "A non-unit ray direction produced the wrong parameter scale.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_NON_UNIT_POINT			, 39, "A non-unit ray direction changed the intersection point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_AWAY_RESULT				, 40, "A ray pointing away from the sphere reported an intersection.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_AWAY_PRESERVED			, 41, "A ray-sphere miss changed the caller's outputs.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TANGENT_RESULT			, 42, "intersectRaySphere() missed a tangent ray.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TANGENT_PARAMETER			, 43, "A tangent ray produced the wrong parameter.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_TANGENT_POINT				, 44, "A tangent ray produced the wrong point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_INSIDE_RESULT				, 45, "A ray starting inside the sphere did not report its origin.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_INSIDE_PARAMETER			, 46, "A ray starting inside the sphere did not report t == 0.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_INSIDE_POINT				, 47, "A ray starting inside the sphere changed its intersection point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_STATIONARY_INSIDE_RESULT	, 48, "A stationary point inside the sphere was not reported as contained.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_STATIONARY_INSIDE_POINT	, 49, "A stationary point inside the sphere changed its intersection point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_STATIONARY_OUTSIDE_RESULT	, 50, "A stationary point outside the sphere reported an intersection.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_STATIONARY_PRESERVED		, 51, "A stationary point outside the sphere changed the caller's outputs.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_NEGATIVE_RADIUS_RESULT	, 52, "intersectRaySphere() accepted a negative sphere radius.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, RAY_NEGATIVE_RADIUS_PRESERVED	, 53, "A rejected negative-radius sphere changed the caller's outputs.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_CENTER_RESULT				, 54, "The center/half-size AABB builder rejected valid dimensions.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_CENTER_LEFT				, 55, "The center/half-size AABB builder produced the wrong left edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_CENTER_TOP				, 56, "The center/half-size AABB builder produced the wrong top edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_CENTER_RIGHT				, 57, "The center/half-size AABB builder produced the wrong right edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_CENTER_BOTTOM				, 58, "The center/half-size AABB builder produced the wrong bottom edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_RECT_RESULT				, 59, "The rectangle AABB builder rejected valid dimensions.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_RECT_LEFT				, 60, "The rectangle AABB builder produced the wrong left edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_RECT_TOP					, 61, "The rectangle AABB builder produced the wrong top edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_RECT_RIGHT				, 62, "The rectangle AABB builder produced the wrong right edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_RECT_BOTTOM				, 63, "The rectangle AABB builder produced the wrong bottom edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_NEGATIVE_HALF_RESULT		, 64, "The center/half-size AABB builder accepted a negative half-size.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_NEGATIVE_HALF_PRESERVED	, 65, "A rejected negative half-size changed an output edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_NEGATIVE_SIZE_RESULT		, 66, "The rectangle AABB builder accepted a negative size.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_NEGATIVE_SIZE_PRESERVED	, 67, "A rejected negative rectangle size changed an output edge.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_ZERO_RESULT				, 68, "The AABB builder rejected a zero-area box.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, AABB_ZERO_EDGE				, 69, "A zero-area AABB did not collapse every edge to its center.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, CONSUMER_LINE_F2_RESULT		, 70, "The f2_t line-intersection contract used by Gontrill failed.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, CONSUMER_LINE_F2_POINT		, 71, "The f2_t line-intersection contract used by Gontrill produced the wrong point.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, CONSUMER_RAY_F2_RESULT		, 72, "The f2_t ray-sphere contract used by Galaxy Hell failed.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, CONSUMER_RAY_F2_PARAMETER	, 73, "The f2_t ray-sphere contract used by Galaxy Hell produced the wrong parameter.");
GDEFINE_ENUM_VALUED(COLLISION_TEST_RESULT, CONSUMER_RAY_F2_POINT		, 74, "The f2_t ray-sphere contract used by Galaxy Hell produced the wrong point.");

sttc bool collisionDiffers(::llc::f3_t actual, ::llc::f3_t expected) { rtrn false == (::llc::abs(actual - expected) <= ::llc::math_epsilon); }

tplt<tpnm T>
sttc ::llc::err_t collisionCheckPoint2(::llc::ATestError& errors, COLLISION_TEST_RESULT result, cnst ::llc::n2<T>& actual, cnst ::llc::n2<T>& expected, cnst char* pointName) {
	LLC_TEST_CHECKF(errors, result, collisionDiffers(actual.x, expected.x), "%s.x:%f, expected:%f.", pointName, (::llc::f3_t)actual.x, (::llc::f3_t)expected.x);
	LLC_TEST_CHECKF(errors, result, collisionDiffers(actual.y, expected.y), "%s.y:%f, expected:%f.", pointName, (::llc::f3_t)actual.y, (::llc::f3_t)expected.y);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t collisionCheckPoint3(::llc::ATestError& errors, COLLISION_TEST_RESULT result, cnst ::llc::n3<T>& actual, cnst ::llc::n3<T>& expected, cnst char* pointName) {
	LLC_TEST_CHECKF(errors, result, collisionDiffers(actual.x, expected.x), "%s.x:%f, expected:%f.", pointName, (::llc::f3_t)actual.x, (::llc::f3_t)expected.x);
	LLC_TEST_CHECKF(errors, result, collisionDiffers(actual.y, expected.y), "%s.y:%f, expected:%f.", pointName, (::llc::f3_t)actual.y, (::llc::f3_t)expected.y);
	LLC_TEST_CHECKF(errors, result, collisionDiffers(actual.z, expected.z), "%s.z:%f, expected:%f.", pointName, (::llc::f3_t)actual.z, (::llc::f3_t)expected.z);
	rtrn 0;
}

tplt<tpnm T>
sttc ::llc::err_t collisionCheckLine(::llc::ATestError& errors, COLLISION_TEST_RESULT result, cnst ::llc::line2<T>& actual, cnst ::llc::line2<T>& expected, cnst char* lineName) {
	if_fail_fe(collisionCheckPoint2(errors, result, actual.A, expected.A, lineName));
	if_fail_fe(collisionCheckPoint2(errors, result, actual.B, expected.B, lineName));
	rtrn 0;
}

sttc ::llc::err_t testCollisionCoordinates(::llc::ATestError& errors) {
	::llc::s1_t integerOutput = -1;
	bool castResult = ::llc::collisionCoordinateCast(7.0, integerOutput);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_INTEGER, false == castResult, "Cast result:%u, expected:1.", (::llc::u2_t)castResult);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_INTEGER, integerOutput != 7, "Coordinate:%i, expected:7.", integerOutput);
	integerOutput = 11;
	castResult = ::llc::collisionCoordinateCast(7.5, integerOutput);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_FRACTION, castResult, "Cast result:%u, expected:0.", (::llc::u2_t)castResult);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_FRACTION, integerOutput != 11, "Coordinate:%i, expected preserved:11.", integerOutput);
	::llc::u0_t byteOutput = 13;
	castResult = ::llc::collisionCoordinateCast(256.0, byteOutput);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_RANGE, castResult, "Cast result:%u, expected:0.", (::llc::u2_t)castResult);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_COORDINATE_RANGE, byteOutput != 13, "Coordinate:%u, expected preserved:13.", (::llc::u2_t)byteOutput);
	rtrn 0;
}

sttc ::llc::err_t testCollisionLines(::llc::ATestError& errors) {
	cnst ::llc::line2f3_t diagonal = {{0, 0}, {4, 4}};
	cnst ::llc::line2f3_t opposite = {{0, 4}, {4, 0}};
	::llc::n2f3_t intersection = {91, 92};
	::llc::err_t result = ::llc::line_line_intersect(diagonal, opposite, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_CROSS_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_CROSS_POINT, intersection, ::llc::n2f3_t{2, 2}, "intersection"));
	intersection = {91, 92};
	result = ::llc::line_line_intersect(::llc::line2f3_t{{4, 4}, {0, 0}}, ::llc::line2f3_t{{4, 0}, {0, 4}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_REVERSED_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_REVERSED_POINT, intersection, ::llc::n2f3_t{2, 2}, "intersection"));
	cnst ::llc::n2f3_t sentinel = {91, 92};
	intersection = sentinel;
	result = ::llc::line_line_intersect(diagonal, ::llc::line2f3_t{{0, 1}, {4, 5}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_PARALLEL_RESULT, result != 0, "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_PARALLEL_PRESERVED, intersection, sentinel, "intersection"));
	intersection = sentinel;
	result = ::llc::line_line_intersect(diagonal, ::llc::line2f3_t{{1, 1}, {3, 3}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_COINCIDENT_RESULT, result != 0, "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_COINCIDENT_PRESERVED, intersection, sentinel, "intersection"));
	::llc::n2s2_t integerIntersection = {91, 92};
	result = ::llc::line_line_intersect(::llc::line2s2_t{{0, 0}, {4, 4}}, ::llc::line2s2_t{{0, 4}, {4, 0}}, integerIntersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_INTEGER_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_INTEGER_POINT, integerIntersection, ::llc::n2s2_t{2, 2}, "intersection"));
	cnst ::llc::n2s2_t integerSentinel = {91, 92};
	integerIntersection = integerSentinel;
	result = ::llc::line_line_intersect(::llc::line2s2_t{{0, 0}, {2, 1}}, ::llc::line2s2_t{{1, -1}, {1, 2}}, integerIntersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_FRACTION_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_FRACTION_PRESERVED, integerIntersection, integerSentinel, "intersection"));
	rtrn 0;
}

sttc ::llc::err_t testCollisionSegments(::llc::ATestError& errors) {
	cnst ::llc::line2f3_t segment = {{0, 0}, {4, 4}};
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_INTERIOR, 1 != ::llc::point_in_segment(segment, ::llc::n2f3_t{2, 2}), "%s", "Interior point rejected.");
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_ENDPOINT, 1 != ::llc::point_in_segment(segment, segment.B), "%s", "Endpoint rejected.");
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_REVERSED, 1 != ::llc::point_in_segment(::llc::line2f3_t{segment.B, segment.A}, ::llc::n2f3_t{2, 2}), "%s", "Reversed segment rejected its interior point.");
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_OFF_LINE, 0 != ::llc::point_in_segment(segment, ::llc::n2f3_t{2, 3}), "%s", "Non-collinear point accepted.");
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_BEYOND, 0 != ::llc::point_in_segment(segment, ::llc::n2f3_t{5, 5}), "%s", "Point beyond endpoint accepted.");
	cnst ::llc::line2f3_t pointSegment = {{3, 3}, {3, 3}};
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_DEGENERATE_HIT, 1 != ::llc::point_in_segment(pointSegment, pointSegment.A), "%s", "Zero-length segment rejected its point.");
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_POINT_DEGENERATE_MISS, 0 != ::llc::point_in_segment(pointSegment, ::llc::n2f3_t{3, 4}), "%s", "Zero-length segment accepted another point.");
	::llc::n2f3_t intersection = {91, 92};
	::llc::err_t result = ::llc::line_segment_intersect(::llc::line2f3_t{{-10, 2}, {10, 2}}, ::llc::line2f3_t{{3, 0}, {3, 4}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_SEGMENT_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_SEGMENT_POINT, intersection, ::llc::n2f3_t{3, 2}, "intersection"));
	cnst ::llc::n2f3_t sentinel = {91, 92};
	intersection = sentinel;
	result = ::llc::line_segment_intersect(::llc::line2f3_t{{-10, 2}, {10, 2}}, ::llc::line2f3_t{{3, 4}, {3, 6}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_LINE_SEGMENT_MISS_RESULT, result != 0, "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_LINE_SEGMENT_MISS_PRESERVED, intersection, sentinel, "intersection"));
	intersection = sentinel;
	result = ::llc::segment_segment_intersect(segment, ::llc::line2f3_t{{0, 4}, {4, 0}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_SEGMENT_CROSS_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_SEGMENT_CROSS_POINT, intersection, ::llc::n2f3_t{2, 2}, "intersection"));
	intersection = sentinel;
	result = ::llc::segment_segment_intersect(::llc::line2f3_t{{0, 0}, {2, 0}}, ::llc::line2f3_t{{2, 0}, {2, 3}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_SEGMENT_ENDPOINT_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_SEGMENT_ENDPOINT_POINT, intersection, ::llc::n2f3_t{2, 0}, "intersection"));
	intersection = sentinel;
	result = ::llc::segment_segment_intersect(::llc::line2f3_t{{0, 0}, {1, 0}}, ::llc::line2f3_t{{2, -1}, {2, 1}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_SEGMENT_MISS_RESULT, result != 0, "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_SEGMENT_MISS_PRESERVED, intersection, sentinel, "intersection"));
	intersection = sentinel;
	result = ::llc::segment_segment_intersect(::llc::line2f3_t{{0, 0}, {4, 0}}, ::llc::line2f3_t{{2, 0}, {6, 0}}, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_SEGMENT_COINCIDENT_RESULT, result != 0, "Result:%i, expected:0 because no unique point exists.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_SEGMENT_COINCIDENT_PRESERVED, intersection, sentinel, "intersection"));
	rtrn 0;
}

sttc ::llc::err_t testCollisionRaySphere(::llc::ATestError& errors) {
	cnst ::llc::spheref3_t sphere = {1, {0, 0, 0}};
	::llc::f2_t parameter = 91;
	::llc::n3f3_t intersection = {91, 92, 93};
	::llc::err_t result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 0, 0}, ::llc::n3f3_t{1, 0, 0}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_TOWARD_RESULT, result != 1, "Result:%i, expected:1.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_TOWARD_PARAMETER, collisionDiffers(parameter, 2), "Parameter:%f, expected:2.", (::llc::f3_t)parameter);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_TOWARD_POINT, intersection, ::llc::n3f3_t{-1, 0, 0}, "intersection"));
	parameter = 91;
	intersection = {91, 92, 93};
	result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 0, 0}, ::llc::n3f3_t{2, 0, 0}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_NON_UNIT_RESULT, result != 1, "Result:%i, expected:1.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_NON_UNIT_PARAMETER, collisionDiffers(parameter, 1), "Parameter:%f, expected:1.", (::llc::f3_t)parameter);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_NON_UNIT_POINT, intersection, ::llc::n3f3_t{-1, 0, 0}, "intersection"));
	cnst ::llc::f2_t parameterSentinel = 91;
	cnst ::llc::n3f3_t pointSentinel = {91, 92, 93};
	parameter = parameterSentinel;
	intersection = pointSentinel;
	result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 0, 0}, ::llc::n3f3_t{-1, 0, 0}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_AWAY_RESULT, result != 0, "Result:%i, expected:0.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_AWAY_PRESERVED, parameter != parameterSentinel, "Parameter:%f, expected preserved:%f.", (::llc::f3_t)parameter, (::llc::f3_t)parameterSentinel);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_AWAY_PRESERVED, intersection, pointSentinel, "intersection"));
	parameter = parameterSentinel;
	intersection = pointSentinel;
	result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 1, 0}, ::llc::n3f3_t{1, 0, 0}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_TANGENT_RESULT, result != 1, "Result:%i, expected:1.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_TANGENT_PARAMETER, collisionDiffers(parameter, 3), "Parameter:%f, expected:3.", (::llc::f3_t)parameter);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_TANGENT_POINT, intersection, ::llc::n3f3_t{0, 1, 0}, "intersection"));
	cnst ::llc::n3f3_t inside = {0.25, 0, 0};
	parameter = parameterSentinel;
	intersection = pointSentinel;
	result = ::llc::intersectRaySphere(inside, ::llc::n3f3_t{1, 0, 0}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_INSIDE_RESULT, result != 1, "Result:%i, expected:1.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_INSIDE_PARAMETER, collisionDiffers(parameter, 0), "Parameter:%f, expected:0.", (::llc::f3_t)parameter);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_INSIDE_POINT, intersection, inside, "intersection"));
	parameter = parameterSentinel;
	intersection = pointSentinel;
	result = ::llc::intersectRaySphere(inside, ::llc::n3f3_t{}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_STATIONARY_INSIDE_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_STATIONARY_INSIDE_POINT, intersection, inside, "intersection"));
	parameter = parameterSentinel;
	intersection = pointSentinel;
	result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 0, 0}, ::llc::n3f3_t{}, sphere, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_STATIONARY_OUTSIDE_RESULT, result != 0, "Result:%i, expected:0.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_STATIONARY_PRESERVED, parameter != parameterSentinel, "Parameter:%f, expected preserved:%f.", (::llc::f3_t)parameter, (::llc::f3_t)parameterSentinel);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_STATIONARY_PRESERVED, intersection, pointSentinel, "intersection"));
	parameter = parameterSentinel;
	intersection = pointSentinel;
	cnst ::llc::spheref3_t negativeRadius = {-1, {0, 0, 0}};
	result = ::llc::intersectRaySphere(::llc::n3f3_t{-3, 0, 0}, ::llc::n3f3_t{1, 0, 0}, negativeRadius, parameter, intersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_NEGATIVE_RADIUS_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_RAY_NEGATIVE_RADIUS_PRESERVED, parameter != parameterSentinel, "Parameter:%f, expected preserved:%f.", (::llc::f3_t)parameter, (::llc::f3_t)parameterSentinel);
	if_fail_fe(collisionCheckPoint3(errors, COLLISION_TEST_RESULT_RAY_NEGATIVE_RADIUS_PRESERVED, intersection, pointSentinel, "intersection"));
	rtrn 0;
}

sttc ::llc::err_t testCollisionAABB(::llc::ATestError& errors) {
	::llc::line2f3_t left = {}, top = {}, right = {}, bottom = {};
	::llc::err_t result = ::llc::buildAABBSegments(::llc::n2f3_t{5, 7}, 2.0, left, top, right, bottom);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_AABB_CENTER_RESULT, ::llc::failed(result), "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_CENTER_LEFT, left, ::llc::line2f3_t{{3, 5}, {3, 9}}, "left"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_CENTER_TOP, top, ::llc::line2f3_t{{3, 9}, {7, 9}}, "top"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_CENTER_RIGHT, right, ::llc::line2f3_t{{7, 9}, {7, 5}}, "right"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_CENTER_BOTTOM, bottom, ::llc::line2f3_t{{7, 5}, {3, 5}}, "bottom"));
	result = ::llc::buildAABBSegments(::llc::rect2f3_t{{3, 5}, {4, 4}}, left, top, right, bottom);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_AABB_RECT_RESULT, ::llc::failed(result), "Result:%i, expected:0.", result);
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_RECT_LEFT, left, ::llc::line2f3_t{{3, 5}, {3, 9}}, "left"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_RECT_TOP, top, ::llc::line2f3_t{{3, 9}, {7, 9}}, "top"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_RECT_RIGHT, right, ::llc::line2f3_t{{7, 9}, {7, 5}}, "right"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_RECT_BOTTOM, bottom, ::llc::line2f3_t{{7, 5}, {3, 5}}, "bottom"));
	cnst ::llc::line2f3_t sentinel = {{91, 92}, {93, 94}};
	left = top = right = bottom = sentinel;
	result = ::llc::buildAABBSegments(::llc::n2f3_t{5, 7}, -2.0, left, top, right, bottom);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_HALF_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_HALF_PRESERVED, left, sentinel, "left"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_HALF_PRESERVED, top, sentinel, "top"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_HALF_PRESERVED, right, sentinel, "right"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_HALF_PRESERVED, bottom, sentinel, "bottom"));
	left = top = right = bottom = sentinel;
	result = ::llc::buildAABBSegments(::llc::rect2f3_t{{3, 5}, {-4, 4}}, left, top, right, bottom);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_SIZE_RESULT, false == ::llc::failed(result), "Result:%i, expected failure.", result);
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_SIZE_PRESERVED, left, sentinel, "left"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_SIZE_PRESERVED, top, sentinel, "top"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_SIZE_PRESERVED, right, sentinel, "right"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_NEGATIVE_SIZE_PRESERVED, bottom, sentinel, "bottom"));
	result = ::llc::buildAABBSegments(::llc::n2f3_t{5, 7}, 0.0, left, top, right, bottom);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_AABB_ZERO_RESULT, ::llc::failed(result), "Result:%i, expected:0.", result);
	cnst ::llc::line2f3_t collapsed = {{5, 7}, {5, 7}};
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_ZERO_EDGE, left, collapsed, "left"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_ZERO_EDGE, top, collapsed, "top"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_ZERO_EDGE, right, collapsed, "right"));
	if_fail_fe(collisionCheckLine(errors, COLLISION_TEST_RESULT_AABB_ZERO_EDGE, bottom, collapsed, "bottom"));
	rtrn 0;
}

sttc ::llc::err_t testCollisionConsumerTypes(::llc::ATestError& errors) {
	::llc::n2f2_t lineIntersection = {};
	::llc::err_t result = ::llc::line_line_intersect(::llc::line2f2_t{{0, 0}, {4, 4}}, ::llc::line2f2_t{{0, 4}, {4, 0}}, lineIntersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_CONSUMER_LINE_F2_RESULT, result != 1, "Result:%i, expected:1.", result);
	if_fail_fe(collisionCheckPoint2(errors, COLLISION_TEST_RESULT_CONSUMER_LINE_F2_POINT, lineIntersection, ::llc::n2f2_t{2, 2}, "intersection"));
	::llc::f2_t parameter = {};
	::llc::n3f2_t sphereIntersection = {};
	result = ::llc::intersectRaySphere(::llc::n3f2_t{-3, 0, 0}, ::llc::n3f2_t{1, 0, 0}, ::llc::spheref2_t{1, {0, 0, 0}}, parameter, sphereIntersection);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_CONSUMER_RAY_F2_RESULT, result != 1, "Result:%i, expected:1.", result);
	LLC_TEST_CHECKF(errors, COLLISION_TEST_RESULT_CONSUMER_RAY_F2_PARAMETER, collisionDiffers(parameter, 2), "Parameter:%f, expected:2.", (::llc::f3_t)parameter);
	rtrn collisionCheckPoint3(errors, COLLISION_TEST_RESULT_CONSUMER_RAY_F2_POINT, sphereIntersection, ::llc::n3f2_t{-1, 0, 0}, "intersection");
}

::llc::err_t testCollision(::llc::ATestError& errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(testCollisionCoordinates(errors));
	if_fail_fe(testCollisionLines(errors));
	if_fail_fe(testCollisionSegments(errors));
	if_fail_fe(testCollisionRaySphere(errors));
	if_fail_fe(testCollisionAABB(errors));
	rtrn testCollisionConsumerTypes(errors);
}
