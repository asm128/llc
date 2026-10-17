#include "llc_camera.h"

#include "llc_test_core.h"

#include <type_traits>

GDEFINE_ENUM_TYPE(CAMERA_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POINTS_SIZE			,  0, "SCameraPoints does not contain exactly two n3f2_t values.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, VECTORS_SIZE		,  1, "SCameraVectors does not contain exactly three n3f2_t values.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RANGE_SIZE			,  2, "SCameraRange does not contain one minmaxf2_t and two f3_t values.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POINTS_AGGREGATE	,  3, "SCameraPoints is no longer an aggregate value record.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, VECTORS_AGGREGATE	,  4, "SCameraVectors is no longer an aggregate value record.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RANGE_AGGREGATE		,  5, "SCameraRange is no longer an aggregate value record.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_X_DEFAULT	,  6, "SCameraPoints::Position.x has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_Y_DEFAULT	,  7, "SCameraPoints::Position.y has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_Z_DEFAULT	,  8, "SCameraPoints::Position.z has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_X_DEFAULT	,  9, "SCameraPoints::Target.x has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_Y_DEFAULT	, 10, "SCameraPoints::Target.y has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_Z_DEFAULT	, 11, "SCameraPoints::Target.z has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_X_DEFAULT		, 12, "SCameraVectors::Front.x has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_Y_DEFAULT		, 13, "SCameraVectors::Front.y has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_Z_DEFAULT		, 14, "SCameraVectors::Front.z has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_X_DEFAULT		, 15, "SCameraVectors::Up.x has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_Y_DEFAULT		, 16, "SCameraVectors::Up.y has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_Z_DEFAULT		, 17, "SCameraVectors::Up.z has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_X_DEFAULT		, 18, "SCameraVectors::Right.x has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_Y_DEFAULT		, 19, "SCameraVectors::Right.y has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_Z_DEFAULT		, 20, "SCameraVectors::Right.z has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, NEAR_DEFAULT		, 21, "SCameraRange::NearFar.Min has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FAR_DEFAULT			, 22, "SCameraRange::NearFar.Max has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, ANGLE_DEFAULT		, 23, "SCameraRange::Angle has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, ZOOM_DEFAULT		, 24, "SCameraRange::Zoom has the wrong default.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_X_AGGREGATE, 25, "Aggregate initialization changed SCameraPoints::Position.x.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_Y_AGGREGATE, 26, "Aggregate initialization changed SCameraPoints::Position.y.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, POSITION_Z_AGGREGATE, 27, "Aggregate initialization changed SCameraPoints::Position.z.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_X_AGGREGATE	, 28, "Aggregate initialization changed SCameraPoints::Target.x.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_Y_AGGREGATE	, 29, "Aggregate initialization changed SCameraPoints::Target.y.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, TARGET_Z_AGGREGATE	, 30, "Aggregate initialization changed SCameraPoints::Target.z.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_X_AGGREGATE	, 31, "Aggregate initialization changed SCameraVectors::Front.x.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_Y_AGGREGATE	, 32, "Aggregate initialization changed SCameraVectors::Front.y.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FRONT_Z_AGGREGATE	, 33, "Aggregate initialization changed SCameraVectors::Front.z.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_X_AGGREGATE		, 34, "Aggregate initialization changed SCameraVectors::Up.x.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_Y_AGGREGATE		, 35, "Aggregate initialization changed SCameraVectors::Up.y.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, UP_Z_AGGREGATE		, 36, "Aggregate initialization changed SCameraVectors::Up.z.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_X_AGGREGATE	, 37, "Aggregate initialization changed SCameraVectors::Right.x.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_Y_AGGREGATE	, 38, "Aggregate initialization changed SCameraVectors::Right.y.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, RIGHT_Z_AGGREGATE	, 39, "Aggregate initialization changed SCameraVectors::Right.z.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, NEAR_AGGREGATE		, 40, "Aggregate initialization changed SCameraRange::NearFar.Min.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, FAR_AGGREGATE		, 41, "Aggregate initialization changed SCameraRange::NearFar.Max.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, ANGLE_AGGREGATE		, 42, "Aggregate initialization changed SCameraRange::Angle.");
GDEFINE_ENUM_VALUED(CAMERA_TEST_RESULT, ZOOM_AGGREGATE		, 43, "Aggregate initialization changed SCameraRange::Zoom.");

sttc ::llc::err_t testCameraDefaults(ATestError & errors) {
	cnst ::llc::SCameraPoints points = {};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_X_DEFAULT, points.Position.x != -0.45f, "Position.x:%g, expected:-0.45.", points.Position.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_Y_DEFAULT, points.Position.y !=  0.05f, "Position.y:%g, expected:0.05.", points.Position.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_Z_DEFAULT, points.Position.z != -0.125f, "Position.z:%g, expected:-0.125.", points.Position.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_X_DEFAULT, points.Target.x != 0, "Target.x:%g, expected:0.", points.Target.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_Y_DEFAULT, points.Target.y != 0, "Target.y:%g, expected:0.", points.Target.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_Z_DEFAULT, points.Target.z != 0, "Target.z:%g, expected:0.", points.Target.z);

	cnst ::llc::SCameraVectors vectors = {};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_X_DEFAULT, vectors.Front.x != 1, "Front.x:%g, expected:1.", vectors.Front.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_Y_DEFAULT, vectors.Front.y != 0, "Front.y:%g, expected:0.", vectors.Front.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_Z_DEFAULT, vectors.Front.z != 0, "Front.z:%g, expected:0.", vectors.Front.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_X_DEFAULT, vectors.Up.x != 0, "Up.x:%g, expected:0.", vectors.Up.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_Y_DEFAULT, vectors.Up.y != 1, "Up.y:%g, expected:1.", vectors.Up.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_Z_DEFAULT, vectors.Up.z != 0, "Up.z:%g, expected:0.", vectors.Up.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_X_DEFAULT, vectors.Right.x != 0, "Right.x:%g, expected:0.", vectors.Right.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_Y_DEFAULT, vectors.Right.y != 0, "Right.y:%g, expected:0.", vectors.Right.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_Z_DEFAULT, vectors.Right.z != 1, "Right.z:%g, expected:1.", vectors.Right.z);

	cnst ::llc::SCameraRange range = {};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_NEAR_DEFAULT, range.NearFar.Min != 0.0001f, "Near:%g, expected:0.0001.", range.NearFar.Min);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FAR_DEFAULT, range.NearFar.Max != 100.0f, "Far:%g, expected:100.", range.NearFar.Max);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_ANGLE_DEFAULT, range.Angle != ::llc::math_pi * 0.25, "Angle:%g, expected:%g.", range.Angle, ::llc::math_pi * 0.25);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_ZOOM_DEFAULT, range.Zoom != 1.0, "Zoom:%g, expected:1.", range.Zoom);
	rtrn 0;
}

sttc ::llc::err_t testCameraAggregates(ATestError & errors) {
	cnst ::llc::SCameraPoints points = {{1, 2, 3}, {4, 5, 6}};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_X_AGGREGATE, points.Position.x != 1, "Position.x:%g, expected:1.", points.Position.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_Y_AGGREGATE, points.Position.y != 2, "Position.y:%g, expected:2.", points.Position.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POSITION_Z_AGGREGATE, points.Position.z != 3, "Position.z:%g, expected:3.", points.Position.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_X_AGGREGATE, points.Target.x != 4, "Target.x:%g, expected:4.", points.Target.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_Y_AGGREGATE, points.Target.y != 5, "Target.y:%g, expected:5.", points.Target.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_TARGET_Z_AGGREGATE, points.Target.z != 6, "Target.z:%g, expected:6.", points.Target.z);

	cnst ::llc::SCameraVectors vectors = {{7, 8, 9}, {10, 11, 12}, {13, 14, 15}};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_X_AGGREGATE, vectors.Front.x != 7, "Front.x:%g, expected:7.", vectors.Front.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_Y_AGGREGATE, vectors.Front.y != 8, "Front.y:%g, expected:8.", vectors.Front.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FRONT_Z_AGGREGATE, vectors.Front.z != 9, "Front.z:%g, expected:9.", vectors.Front.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_X_AGGREGATE, vectors.Up.x != 10, "Up.x:%g, expected:10.", vectors.Up.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_Y_AGGREGATE, vectors.Up.y != 11, "Up.y:%g, expected:11.", vectors.Up.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_UP_Z_AGGREGATE, vectors.Up.z != 12, "Up.z:%g, expected:12.", vectors.Up.z);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_X_AGGREGATE, vectors.Right.x != 13, "Right.x:%g, expected:13.", vectors.Right.x);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_Y_AGGREGATE, vectors.Right.y != 14, "Right.y:%g, expected:14.", vectors.Right.y);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RIGHT_Z_AGGREGATE, vectors.Right.z != 15, "Right.z:%g, expected:15.", vectors.Right.z);

	cnst ::llc::SCameraRange range = {{16, 17}, 18, 19};
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_NEAR_AGGREGATE, range.NearFar.Min != 16, "Near:%g, expected:16.", range.NearFar.Min);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_FAR_AGGREGATE, range.NearFar.Max != 17, "Far:%g, expected:17.", range.NearFar.Max);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_ANGLE_AGGREGATE, range.Angle != 18, "Angle:%g, expected:18.", range.Angle);
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_ZOOM_AGGREGATE, range.Zoom != 19, "Zoom:%g, expected:19.", range.Zoom);
	rtrn 0;
}

::llc::err_t testCamera(ATestError & errors) {
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_POINTS_SIZE, sizeof(::llc::SCameraPoints) != sizeof(::llc::n3f2_t) * 2, "SCameraPoints size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::SCameraPoints), (::llc::u2_t)(sizeof(::llc::n3f2_t) * 2));
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_VECTORS_SIZE, sizeof(::llc::SCameraVectors) != sizeof(::llc::n3f2_t) * 3, "SCameraVectors size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::SCameraVectors), (::llc::u2_t)(sizeof(::llc::n3f2_t) * 3));
	LLC_TEST_CHECKF(errors, CAMERA_TEST_RESULT_RANGE_SIZE, sizeof(::llc::SCameraRange) != sizeof(::llc::minmaxf2_t) + sizeof(::llc::f3_t) * 2, "SCameraRange size:%u, expected:%u.", (::llc::u2_t)sizeof(::llc::SCameraRange), (::llc::u2_t)(sizeof(::llc::minmaxf2_t) + sizeof(::llc::f3_t) * 2));
	LLC_TEST_CHECK(errors, CAMERA_TEST_RESULT_POINTS_AGGREGATE, false == std::is_aggregate_v<::llc::SCameraPoints>);
	LLC_TEST_CHECK(errors, CAMERA_TEST_RESULT_VECTORS_AGGREGATE, false == std::is_aggregate_v<::llc::SCameraVectors>);
	LLC_TEST_CHECK(errors, CAMERA_TEST_RESULT_RANGE_AGGREGATE, false == std::is_aggregate_v<::llc::SCameraRange>);
	if_fail_fe(::testCameraDefaults(errors));
	rtrn ::testCameraAggregates(errors);
}
