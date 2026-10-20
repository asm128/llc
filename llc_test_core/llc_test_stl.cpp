#include "llc_test_core.h"
#include "llc_stl.h"
#include "llc_file.h"
#include "llc_test_trace.h"
#include "llc_geometry.h"
#include "llc_path.h"

GDEFINE_ENUM_TYPE(STL_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, OK						,  0, "All STL tests passed.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, STORAGE					,  1, "The binary STL test storage could not be prepared.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, MUTABLE_RESULT			,  2, "The mutable STL loader reported the wrong consumed size.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, MUTABLE_HEADER			,  3, "The mutable STL loader returned the wrong header view.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, MUTABLE_TRIANGLE_COUNT	,  4, "The mutable STL loader returned the wrong triangle count or alias.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, MUTABLE_TRIANGLE_VALUE	,  5, "The mutable STL loader returned different triangle data.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, MUTABLE_TRIANGLE_WRITE	,  6, "The mutable STL triangle view did not update its source storage.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, CONST_RESULT				,  7, "The const STL loader reported the wrong consumed size.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, CONST_HEADER				,  8, "The const STL loader returned the wrong header view.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, CONST_TRIANGLE_COUNT		,  9, "The const STL loader returned the wrong triangle count or alias.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, CONST_TRIANGLE_VALUE		, 10, "The const STL loader returned different triangle data.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, TRUNCATED_RESULT			, 11, "The STL loader accepted truncated storage.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, TRUNCATED_HEADER			, 12, "A failed STL load modified the header output.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, TRUNCATED_TRIANGLES		, 13, "A failed STL load modified the triangle output.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, GEOMETRY_BUILD			, 14, "Generated STL geometry has invalid indices or no usable triangles.");
GDEFINE_ENUM_VALUED(STL_TEST_RESULT, GEOMETRY_ROUNDTRIP		, 15, "Generated geometry changed after saving and loading an STL file.");

static_assert(requires(::llc::vu0_t input, ::llc::vcu0_t & header, ::llc::view<::llc::SSTLTriangle> & triangles) { ::llc::stlFileLoad(input, header, triangles); });
static_assert(requires(::llc::vcu0_t input, ::llc::vcu0_t & header, ::llc::view<cnst ::llc::SSTLTriangle> & triangles) { ::llc::stlFileLoad(input, header, triangles); });

sttc ::llc::err_t testSTLGeometry(ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::string folder;
	if_fail_fe(::llc::pathNameCompose(outputFolder, ::llc::get_value_namev(STL_TEST_RESULT_GEOMETRY_ROUNDTRIP), folder));
	if_fail_fe(::llc::pathCreate(folder));
	stxp ::llc::vcst_t filenames[] = {"box.stl", "sphere.stl"};
	for(::llc::u2_t iShape = 0; iShape < ::llc::size(filenames); ++iShape) {
		::llc::SGeometryBuffers geometry;
		if(0 == iShape) {
			::llc::SParamsBox params;
			params.HalfSizes = {20, 15, 10};
			params.Origin = params.HalfSizes;
			if_fail_fe(::llc::geometryBuildBox(geometry, params));
		}
		else {
			::llc::SParamsSphere params;
			params.Radius = 20;
			params.CellCount = {32, 16};
			if_fail_fe(::llc::geometryBuildSphere(geometry, params));
		}
		LLC_TEST_REQUIRE(errors, STL_TEST_RESULT_GEOMETRY_BUILD, geometry.PositionIndices.size() % 3);
		::llc::apod<::llc::SSTLTriangle> expected;
		::llc::u2_t collapsed = 0;
		for(::llc::u2_t iIndex = 0; iIndex < geometry.PositionIndices.size(); iIndex += 3) {
			cnst ::llc::u2_t a = geometry.PositionIndices[iIndex], b = geometry.PositionIndices[iIndex + 1], c = geometry.PositionIndices[iIndex + 2];
			LLC_TEST_REQUIRE(errors, STL_TEST_RESULT_GEOMETRY_BUILD, a >= geometry.Positions.size() || b >= geometry.Positions.size() || c >= geometry.Positions.size());
			::llc::SSTLTriangle triangle = {};
			triangle.Triangle = {geometry.Positions[a], geometry.Positions[b], geometry.Positions[c]};
			triangle.Normal = (triangle.Triangle.B - triangle.Triangle.A).Cross(triangle.Triangle.C - triangle.Triangle.A);
			if(triangle.Normal.LengthSquared() < 1e-8f) {
				++collapsed;
				continue;
			}
			triangle.Normal.Normalize();
			if_fail_fe(expected.push_back(triangle));
		}
		LLC_TEST_REQUIRE(errors, STL_TEST_RESULT_GEOMETRY_BUILD, 0 == expected.size());
		::llc::au0_t serialized;
		if_fail_fe(serialized.resize(84 + expected.byte_count(), 0));
		stxp char header[] = "LLC generated geometry - binary STL";
		memcpy(serialized.begin(), header, szof(header) - 1);
		cnst ::llc::u2_t count = expected.size();
		memcpy(&serialized[80], &count, szof(count));
		memcpy(&serialized[84], expected.begin(), expected.byte_count());
		if_fail_fe(::llc::fileFromMemory(folder, filenames[iShape], serialized));
		::llc::string path;
		if_fail_fe(::llc::pathNameCompose(folder, filenames[iShape], path));
		::llc::SSTLFile loaded;
		cnst ::llc::err_t result = ::llc::stlFileLoad(path, loaded);
		LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_GEOMETRY_ROUNDTRIP, result < 0 || loaded.Triangles.size() != count, "File:%.*s, result:%i, triangles:%u, expected:%u.", LLC_VIEW_ARGS(path), result, loaded.Triangles.size(), count);
		cnst bool different = 0 != memcmp(loaded.Triangles.begin(), expected.begin(), expected.byte_count());
		LLC_TEST_CHECK(errors, STL_TEST_RESULT_GEOMETRY_ROUNDTRIP, different);
		always_printf("STL geometry %.*s: vertices:%u, source triangles:%u, omitted collapsed triangles:%u, saved/loaded triangles:%u/%u, bytes:%u, comparison:%s. Box dimensions:40x30x20; sphere radius:20, cells:32x16. Units are application-defined.", LLC_VIEW_ARGS(filenames[iShape]), geometry.Positions.size(), geometry.PositionIndices.size() / 3, collapsed, count, loaded.Triangles.size(), serialized.size(), different ? "FAIL" : "PASS");
	}
	rtrn 0;
}

::llc::err_t testSTL(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	if_fail_fe(::testSTLGeometry(errors, outputFolder));
	stxp ::llc::u2_t				HEADER_SIZE				= 80;
	::llc::SSTLTriangle			expected[2]				= {};
	expected[0].Normal										= {1, 2, 3};
	expected[0].Triangle.A								= {4, 5, 6};
	expected[0].Attribute								= 0x1234U;
	expected[1].Normal										= {7, 8, 9};
	expected[1].Triangle.C								= {10, 11, 12};
	expected[1].Attribute								= 0x5678U;

	stxp ::llc::u2_t				COUNT_SIZE				= szof(::llc::u2_t);
	cnst ::llc::u2_t				payloadSize				= szof(expected);
	cnst ::llc::u2_t				serializedSize			= HEADER_SIZE + COUNT_SIZE + payloadSize;
	::llc::au0_t					serialized				= {};
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_STORAGE, serialized.resize(serializedSize) != (::llc::err_t)serializedSize , "Size:%u, expected:%u." , serialized.size(), serializedSize );
	for(::llc::u2_t iByte = 0; iByte < HEADER_SIZE; ++iByte)
		serialized[iByte]									= (::llc::u0_t)iByte;
	cnst ::llc::u2_t				triangleCount				= ::llc::size(expected);
	memcpy(&serialized[HEADER_SIZE], &triangleCount, COUNT_SIZE);
	memcpy(&serialized[HEADER_SIZE + COUNT_SIZE], expected, payloadSize);
	if_fail_fe(::llc::fileFromMemory(outputFolder, "input.stl", serialized));
	always_printf("STL fixture input.stl: %u bytes, header:%u, expected triangles:%u. Triangle geometry is synthetic loader data, not a display model.", serialized.size(), HEADER_SIZE, triangleCount);
	::testTraceBytes("Input header", {serialized.begin(), HEADER_SIZE});

	::llc::vcu0_t					mutableHeader				= {};
	::llc::view<::llc::SSTLTriangle>	mutableTriangles			= {};
	cnst ::llc::err_t				mutableResult				= ::llc::stlFileLoad((::llc::vu0_t)serialized, mutableHeader, mutableTriangles);
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_RESULT, mutableResult != (::llc::err_t)(COUNT_SIZE + payloadSize) , "Result:%i, expected:%u." , mutableResult, COUNT_SIZE + payloadSize );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_HEADER, mutableHeader.size() != HEADER_SIZE , "Size:%u, expected:%u." , mutableHeader.size(), HEADER_SIZE );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_MUTABLE_HEADER, mutableHeader.begin() != serialized.begin() , "Begin:%p, expected:%p." , mutableHeader.begin(), serialized.begin() );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_COUNT, mutableTriangles.size() != triangleCount , "Count:%u, expected:%u." , mutableTriangles.size(), triangleCount );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_COUNT, mutableTriangles.cu8().begin() != &serialized[HEADER_SIZE + COUNT_SIZE] , "Begin:%p, expected:%p." , mutableTriangles.begin(), &serialized[HEADER_SIZE + COUNT_SIZE] );
	LLC_TEST_CHECK(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_VALUE, 0 != memcmp(mutableTriangles.begin(), expected, payloadSize));
	for(::llc::u2_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle) {
		always_printf("STL triangle:%u; normal actual:{%g,%g,%g}, expected:{%g,%g,%g}; attribute actual:0x%04X expected:0x%04X.", iTriangle, (double)mutableTriangles[iTriangle].Normal.x, (double)mutableTriangles[iTriangle].Normal.y, (double)mutableTriangles[iTriangle].Normal.z, (double)expected[iTriangle].Normal.x, (double)expected[iTriangle].Normal.y, (double)expected[iTriangle].Normal.z, mutableTriangles[iTriangle].Attribute, expected[iTriangle].Attribute);
		::testTraceBytes("Expected triangle", ::llc::view<cnst ::llc::SSTLTriangle>{&expected[iTriangle], 1}.cu8());
		::testTraceBytes("Actual triangle", ::llc::view<cnst ::llc::SSTLTriangle>{&mutableTriangles[iTriangle], 1}.cu8());
	}
	mutableTriangles[0].Attribute								= 0xA55AU;
	::llc::SSTLTriangle			storedTriangle			= {};
	memcpy(&storedTriangle, &serialized[HEADER_SIZE + COUNT_SIZE], szof(storedTriangle));
	if_fail_fe(::llc::fileFromMemory(outputFolder, "after_attribute_write.stl", serialized));
	always_printf("STL alias write: first attribute changed to A55A; stored:%04X. Snapshot: after_attribute_write.stl.", storedTriangle.Attribute);
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_WRITE, storedTriangle.Attribute != mutableTriangles[0].Attribute , "Stored attribute:0x%04X, expected:0x%04X." , storedTriangle.Attribute, mutableTriangles[0].Attribute );
	expected[0].Attribute									= mutableTriangles[0].Attribute;

	::llc::vcu0_t					constHeader				= {};
	::llc::view<cnst ::llc::SSTLTriangle>	constTriangles	= {};
	cnst ::llc::err_t				constResult				= ::llc::stlFileLoad((::llc::vcu0_t)serialized, constHeader, constTriangles);
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_CONST_RESULT, constResult != (::llc::err_t)(COUNT_SIZE + payloadSize) , "Result:%i, expected:%u." , constResult, COUNT_SIZE + payloadSize );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_CONST_HEADER, constHeader.size() != HEADER_SIZE , "Size:%u, expected:%u." , constHeader.size(), HEADER_SIZE );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_CONST_HEADER, constHeader.begin() != serialized.begin() , "Begin:%p, expected:%p." , constHeader.begin(), serialized.begin() );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_CONST_TRIANGLE_COUNT, constTriangles.size() != triangleCount , "Count:%u, expected:%u." , constTriangles.size(), triangleCount );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_CONST_TRIANGLE_COUNT, constTriangles.cu8().begin() != &serialized[HEADER_SIZE + COUNT_SIZE] , "Begin:%p, expected:%p." , constTriangles.begin(), &serialized[HEADER_SIZE + COUNT_SIZE] );
	LLC_TEST_CHECK(errors, STL_TEST_RESULT_CONST_TRIANGLE_VALUE, 0 != memcmp(constTriangles.begin(), expected, payloadSize));

	::llc::u0_t						headerSentinelStorage[]		= {0xA5U};
	::llc::SSTLTriangle			triangleSentinelStorage[1]	= {};
	for(::llc::u2_t byteCount = 0; byteCount < serialized.size(); ++byteCount) {
		::llc::vcu0_t					headerSentinel				= headerSentinelStorage;
		::llc::view<cnst ::llc::SSTLTriangle>	triangleSentinel		= triangleSentinelStorage;
		cnst ::llc::err_t				result						= ::llc::stlFileLoad(::llc::vcu0_t{serialized.begin(), byteCount}, headerSentinel, triangleSentinel);
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_RESULT, 0 <= result , "Accepted bytes:%u of %u. Result:%i." , byteCount, serialized.size(), result );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_HEADER, headerSentinel.begin() != headerSentinelStorage , "Bytes:%u, begin:%p, expected:%p." , byteCount, headerSentinel.begin(), headerSentinelStorage );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_HEADER, headerSentinel.size() != ::llc::size(headerSentinelStorage) , "Bytes:%u, count:%u, expected:%u." , byteCount, headerSentinel.size(), ::llc::u2_t(::llc::size(headerSentinelStorage)) );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_TRIANGLES, triangleSentinel.begin() != triangleSentinelStorage , "Bytes:%u, begin:%p, expected:%p." , byteCount, triangleSentinel.begin(), triangleSentinelStorage );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_TRIANGLES, triangleSentinel.size() != ::llc::size(triangleSentinelStorage) , "Bytes:%u, count:%u, expected:%u." , byteCount, triangleSentinel.size(), ::llc::u2_t(::llc::size(triangleSentinelStorage)) );
	}
	return 0;
}
