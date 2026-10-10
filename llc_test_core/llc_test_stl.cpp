#include "llc_test_core.h"
#include "llc_stl.h"

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

static_assert(requires(::llc::vu0_t input, ::llc::vcu0_t & header, ::llc::view<::llc::SSTLTriangle> & triangles) { ::llc::stlFileLoad(input, header, triangles); });
static_assert(requires(::llc::vcu0_t input, ::llc::vcu0_t & header, ::llc::view<cnst ::llc::SSTLTriangle> & triangles) { ::llc::stlFileLoad(input, header, triangles); });

::llc::err_t testSTL(ATestError & errors) {
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

	::llc::vcu0_t					mutableHeader				= {};
	::llc::view<::llc::SSTLTriangle>	mutableTriangles			= {};
	cnst ::llc::err_t				mutableResult				= ::llc::stlFileLoad((::llc::vu0_t)serialized, mutableHeader, mutableTriangles);
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_RESULT, mutableResult != (::llc::err_t)(COUNT_SIZE + payloadSize) , "Result:%i, expected:%u." , mutableResult, COUNT_SIZE + payloadSize );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_HEADER, mutableHeader.size() != HEADER_SIZE , "Size:%u, expected:%u." , mutableHeader.size(), HEADER_SIZE );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_MUTABLE_HEADER, mutableHeader.begin() != serialized.begin() , "Begin:%p, expected:%p." , mutableHeader.begin(), serialized.begin() );
	LLC_TEST_REQUIREF(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_COUNT, mutableTriangles.size() != triangleCount , "Count:%u, expected:%u." , mutableTriangles.size(), triangleCount );
	LLC_TEST_CHECKF(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_COUNT, mutableTriangles.cu8().begin() != &serialized[HEADER_SIZE + COUNT_SIZE] , "Begin:%p, expected:%p." , mutableTriangles.begin(), &serialized[HEADER_SIZE + COUNT_SIZE] );
	LLC_TEST_CHECK(errors, STL_TEST_RESULT_MUTABLE_TRIANGLE_VALUE, 0 != memcmp(mutableTriangles.begin(), expected, payloadSize));
	mutableTriangles[0].Attribute								= 0xA55AU;
	::llc::SSTLTriangle			storedTriangle			= {};
	memcpy(&storedTriangle, &serialized[HEADER_SIZE + COUNT_SIZE], szof(storedTriangle));
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
		::llc::setupLogCallbacks(0, 0);
		cnst ::llc::err_t				result						= ::llc::stlFileLoad(::llc::vcu0_t{serialized.begin(), byteCount}, headerSentinel, triangleSentinel);
		::llc::setupDefaultLogCallbacks();
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_RESULT, 0 <= result , "Accepted bytes:%u of %u. Result:%i." , byteCount, serialized.size(), result );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_HEADER, headerSentinel.begin() != headerSentinelStorage , "Bytes:%u, begin:%p, expected:%p." , byteCount, headerSentinel.begin(), headerSentinelStorage );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_HEADER, headerSentinel.size() != ::llc::size(headerSentinelStorage) , "Bytes:%u, count:%u, expected:%u." , byteCount, headerSentinel.size(), ::llc::u2_t(::llc::size(headerSentinelStorage)) );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_TRIANGLES, triangleSentinel.begin() != triangleSentinelStorage , "Bytes:%u, begin:%p, expected:%p." , byteCount, triangleSentinel.begin(), triangleSentinelStorage );
		LLC_TEST_CHECKF(errors, STL_TEST_RESULT_TRUNCATED_TRIANGLES, triangleSentinel.size() != ::llc::size(triangleSentinelStorage) , "Bytes:%u, count:%u, expected:%u." , byteCount, triangleSentinel.size(), ::llc::u2_t(::llc::size(triangleSentinelStorage)) );
	}
	return 0;
}
