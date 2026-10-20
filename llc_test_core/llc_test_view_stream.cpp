#include "llc_test_core.h"
#include "llc_view_stream.h"

GDEFINE_ENUM_TYPE(VIEW_STREAM_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_CONST_ACCESS		,  0, "A byte stream over const storage could not read POD values.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_MUTABLE_ACCESS	,  1, "A byte stream over mutable storage could not write POD values.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_CONST_ACCESS		,  2, "A byte stream over const storage admitted a POD write.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_SCALAR_RESULT		,  3, "Scalar write_pod() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_SCALAR_CURSOR		,  4, "Scalar write_pod() advanced the cursor incorrectly.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_SCALAR_REMAINING	,  5, "Scalar write_pod() reported the wrong remaining byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_SCALAR_VALUE		,  6, "Scalar write_pod() stored different bytes.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_SCALAR_RESULT		,  7, "Scalar read_pod() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_SCALAR_CURSOR		,  8, "Scalar read_pod() advanced the cursor incorrectly.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_SCALAR_REMAINING	,  9, "Scalar read_pod() reported the wrong remaining byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_SCALAR_VALUE		, 10, "Scalar read_pod() restored different bytes.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_ARRAY_RESULT		, 11, "Array write_pod() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_ARRAY_CURSOR		, 12, "Array write_pod() advanced the cursor incorrectly.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_ARRAY_REMAINING	, 13, "Array write_pod() reported the wrong remaining byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, WRITE_ARRAY_VALUE		, 14, "Array write_pod() stored different bytes.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_ARRAY_RESULT		, 15, "Array read_pod() reported the wrong byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_ARRAY_CURSOR		, 16, "Array read_pod() advanced the cursor incorrectly.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_ARRAY_REMAINING	, 17, "Array read_pod() reported the wrong remaining byte count.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, READ_ARRAY_VALUE		, 18, "Array read_pod() restored different bytes.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, EMPTY_RESULT			, 19, "An empty POD operation did not report zero bytes.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, EMPTY_CURSOR			, 20, "An empty POD operation changed the cursor.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_READ_RESULT	, 21, "read_pod() accepted a truncated source.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_READ_OUTPUT	, 22, "read_pod() modified its output after rejecting a truncated source.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_READ_CURSOR	, 23, "read_pod() moved its cursor after rejecting a truncated source.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_WRITE_RESULT	, 24, "write_pod() accepted a truncated destination.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_WRITE_OUTPUT	, 25, "write_pod() modified storage after rejecting a truncated destination.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, TRUNCATED_WRITE_CURSOR	, 26, "write_pod() moved its cursor after rejecting a truncated destination.");
GDEFINE_ENUM_VALUED(VIEW_STREAM_TEST_RESULT, INVALID_CURSOR_REMAINING, 27, "remaining() underflowed after an invalid cursor position.");

tplt<tpnm TStream, tpnm T>
concept canViewStreamRead = requires(TStream & stream, T & output) { stream.read_pod(output); };

tplt<tpnm TStream, tpnm T>
concept canViewStreamWrite = requires(TStream & stream, cnst T & input) { stream.write_pod(input); };

tplt<tpnm T>
sttc ::llc::err_t testViewStreamType(ATestError & errors) {
	cnst T scalarSource = T(0x35);
	::llc::u0_t scalarStorage[szof(T)] = {};
	::llc::view_stream<::llc::u0_t> scalarWriter{scalarStorage};
	cnst ::llc::err_t scalarWriteResult = scalarWriter.write_pod(scalarSource);
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_SCALAR_RESULT, scalarWriteResult != szof(T) , "%u-bit scalar write result:%i, expected:%u." , bcof(T), scalarWriteResult, ::llc::u2_t(szof(T)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_SCALAR_CURSOR, scalarWriter.CursorPosition != szof(T) , "%u-bit scalar write cursor:%u, expected:%u." , bcof(T), scalarWriter.CursorPosition, ::llc::u2_t(szof(T)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_SCALAR_REMAINING, scalarWriter.remaining() , "%u-bit scalar write remaining:%u, expected:0." , bcof(T), scalarWriter.remaining() );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_SCALAR_VALUE, 0 != memcmp(scalarStorage, &scalarSource, szof(T)) , "%u-bit scalar write changed the representation." , bcof(T) );

	::llc::view_stream<cnst ::llc::u0_t> scalarReader{scalarStorage};
	T scalarOutput = {};
	cnst ::llc::err_t scalarReadResult = scalarReader.read_pod(scalarOutput);
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_SCALAR_RESULT, scalarReadResult != szof(T) , "%u-bit scalar read result:%i, expected:%u." , bcof(T), scalarReadResult, ::llc::u2_t(szof(T)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_SCALAR_CURSOR, scalarReader.CursorPosition != szof(T) , "%u-bit scalar read cursor:%u, expected:%u." , bcof(T), scalarReader.CursorPosition, ::llc::u2_t(szof(T)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_SCALAR_REMAINING, scalarReader.remaining() , "%u-bit scalar read remaining:%u, expected:0." , bcof(T), scalarReader.remaining() );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_SCALAR_VALUE, 0 != memcmp(&scalarOutput, &scalarSource, szof(T)) , "%u-bit scalar read changed the representation." , bcof(T) );

	cnst T arraySource[3] = {T(0x11), T(0x23), T(0x47)};
	::llc::u0_t arrayStorage[szof(arraySource)] = {};
	::llc::view_stream<::llc::u0_t> arrayWriter{arrayStorage};
	cnst ::llc::err_t arrayWriteResult = arrayWriter.write_pod(arraySource);
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_ARRAY_RESULT, arrayWriteResult != szof(arraySource) , "%u-bit array write result:%i, expected:%u." , bcof(T), arrayWriteResult, ::llc::u2_t(szof(arraySource)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_ARRAY_CURSOR, arrayWriter.CursorPosition != szof(arraySource) , "%u-bit array write cursor:%u, expected:%u." , bcof(T), arrayWriter.CursorPosition, ::llc::u2_t(szof(arraySource)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_ARRAY_REMAINING, arrayWriter.remaining() , "%u-bit array write remaining:%u, expected:0." , bcof(T), arrayWriter.remaining() );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_WRITE_ARRAY_VALUE, 0 != memcmp(arrayStorage, arraySource, szof(arraySource)) , "%u-bit array write changed the representation." , bcof(T) );

	::llc::view_stream<cnst ::llc::u0_t> arrayReader{arrayStorage};
	T arrayOutput[::llc::size(arraySource)] = {};
	cnst ::llc::err_t arrayReadResult = arrayReader.read_pod(arrayOutput);
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_ARRAY_RESULT, arrayReadResult != szof(arraySource) , "%u-bit array read result:%i, expected:%u." , bcof(T), arrayReadResult, ::llc::u2_t(szof(arraySource)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_ARRAY_CURSOR, arrayReader.CursorPosition != szof(arraySource) , "%u-bit array read cursor:%u, expected:%u." , bcof(T), arrayReader.CursorPosition, ::llc::u2_t(szof(arraySource)) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_ARRAY_REMAINING, arrayReader.remaining() , "%u-bit array read remaining:%u, expected:0." , bcof(T), arrayReader.remaining() );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_READ_ARRAY_VALUE, 0 != memcmp(arrayOutput, arraySource, szof(arraySource)) , "%u-bit array read changed the representation." , bcof(T) );

	cnst ::llc::u2_t scalarWriteEnd = scalarWriter.CursorPosition;
	cnst ::llc::err_t emptyWriteResult = scalarWriter.write_pod(::llc::view<cnst T>{});
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_EMPTY_RESULT, emptyWriteResult , "%u-bit empty write result:%i, expected:0." , bcof(T), emptyWriteResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_EMPTY_CURSOR, scalarWriter.CursorPosition != scalarWriteEnd , "%u-bit empty write cursor:%u, expected:%u." , bcof(T), scalarWriter.CursorPosition, scalarWriteEnd );
	cnst ::llc::u2_t scalarReadEnd = scalarReader.CursorPosition;
	cnst ::llc::err_t emptyReadResult = scalarReader.read_pod(::llc::view<T>{});
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_EMPTY_RESULT, emptyReadResult , "%u-bit empty read result:%i, expected:0." , bcof(T), emptyReadResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_EMPTY_CURSOR, scalarReader.CursorPosition != scalarReadEnd , "%u-bit empty read cursor:%u, expected:%u." , bcof(T), scalarReader.CursorPosition, scalarReadEnd );

	T truncatedScalarOutput = T(0x67);
	cnst T originalScalarOutput = truncatedScalarOutput;
	::llc::view_stream<cnst ::llc::u0_t> truncatedScalarReader{::llc::u2_t(szof(T) - 1), scalarStorage};
	T truncatedArrayOutput[::llc::size(arraySource)] = {T(0x71), T(0x72), T(0x73)};
	T originalArrayOutput[::llc::size(arraySource)] = {};
	memcpy(originalArrayOutput, truncatedArrayOutput, szof(originalArrayOutput));
	::llc::view_stream<cnst ::llc::u0_t> truncatedArrayReader{::llc::u2_t(szof(arraySource) - 1), arrayStorage};
	::llc::u0_t truncatedScalarStorage[szof(T)] = {};
	memset(truncatedScalarStorage, 0xA5, szof(truncatedScalarStorage));
	::llc::u0_t originalScalarStorage[szof(T)] = {};
	memcpy(originalScalarStorage, truncatedScalarStorage, szof(originalScalarStorage));
	::llc::view_stream<::llc::u0_t> truncatedScalarWriter{::llc::u2_t(szof(T) - 1), truncatedScalarStorage};
	::llc::u0_t truncatedArrayStorage[szof(arraySource)] = {};
	memset(truncatedArrayStorage, 0x5A, szof(truncatedArrayStorage));
	::llc::u0_t originalArrayStorage[szof(arraySource)] = {};
	memcpy(originalArrayStorage, truncatedArrayStorage, szof(originalArrayStorage));
	::llc::view_stream<::llc::u0_t> truncatedArrayWriter{::llc::u2_t(szof(arraySource) - 1), truncatedArrayStorage};
	cnst ::llc::err_t truncatedScalarReadResult = truncatedScalarReader.read_pod(truncatedScalarOutput);
	cnst ::llc::err_t truncatedArrayReadResult = truncatedArrayReader.read_pod(truncatedArrayOutput);
	cnst ::llc::err_t truncatedScalarWriteResult = truncatedScalarWriter.write_pod(scalarSource);
	cnst ::llc::err_t truncatedArrayWriteResult = truncatedArrayWriter.write_pod(arraySource);
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_RESULT, 0 <= truncatedScalarReadResult , "%u-bit scalar read accepted %u/%u bytes. result:%i." , bcof(T), ::llc::u2_t(szof(T) - 1), ::llc::u2_t(szof(T)), truncatedScalarReadResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_OUTPUT, 0 != memcmp(&truncatedScalarOutput, &originalScalarOutput, szof(T)) , "%u-bit truncated scalar read modified output." , bcof(T) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_CURSOR, truncatedScalarReader.CursorPosition , "%u-bit truncated scalar read cursor:%u, expected:0." , bcof(T), truncatedScalarReader.CursorPosition );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_RESULT, 0 <= truncatedArrayReadResult , "%u-bit array read accepted %u/%u bytes. result:%i." , bcof(T), ::llc::u2_t(szof(arraySource) - 1), ::llc::u2_t(szof(arraySource)), truncatedArrayReadResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_OUTPUT, 0 != memcmp(truncatedArrayOutput, originalArrayOutput, szof(originalArrayOutput)) , "%u-bit truncated array read modified output." , bcof(T) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_READ_CURSOR, truncatedArrayReader.CursorPosition , "%u-bit truncated array read cursor:%u, expected:0." , bcof(T), truncatedArrayReader.CursorPosition );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_RESULT, 0 <= truncatedScalarWriteResult , "%u-bit scalar write accepted %u/%u bytes. result:%i." , bcof(T), ::llc::u2_t(szof(T) - 1), ::llc::u2_t(szof(T)), truncatedScalarWriteResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_OUTPUT, 0 != memcmp(truncatedScalarStorage, originalScalarStorage, szof(originalScalarStorage)) , "%u-bit truncated scalar write modified storage." , bcof(T) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_CURSOR, truncatedScalarWriter.CursorPosition , "%u-bit truncated scalar write cursor:%u, expected:0." , bcof(T), truncatedScalarWriter.CursorPosition );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_RESULT, 0 <= truncatedArrayWriteResult , "%u-bit array write accepted %u/%u bytes. result:%i." , bcof(T), ::llc::u2_t(szof(arraySource) - 1), ::llc::u2_t(szof(arraySource)), truncatedArrayWriteResult );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_OUTPUT, 0 != memcmp(truncatedArrayStorage, originalArrayStorage, szof(originalArrayStorage)) , "%u-bit truncated array write modified storage." , bcof(T) );
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_TRUNCATED_WRITE_CURSOR, truncatedArrayWriter.CursorPosition , "%u-bit truncated array write cursor:%u, expected:0." , bcof(T), truncatedArrayWriter.CursorPosition );

	::llc::view_stream<cnst ::llc::u0_t> invalidCursor{scalarStorage};
	invalidCursor.CursorPosition = invalidCursor.size() + 1;
	LLC_TEST_CHECKF(errors, VIEW_STREAM_TEST_RESULT_INVALID_CURSOR_REMAINING, invalidCursor.remaining() , "%u-bit invalid cursor remaining:%u, expected:0." , bcof(T), invalidCursor.remaining() );
	rtrn 0;
}

::llc::err_t testViewStream(ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	LLC_TEST_CHECK(errors, VIEW_STREAM_TEST_RESULT_READ_CONST_ACCESS, (false == canViewStreamRead<::llc::view_stream<cnst ::llc::u0_t>, ::llc::u2_t>));
	LLC_TEST_CHECK(errors, VIEW_STREAM_TEST_RESULT_WRITE_MUTABLE_ACCESS, (false == canViewStreamWrite<::llc::view_stream<::llc::u0_t>, ::llc::u2_t>));
	LLC_TEST_CHECK(errors, VIEW_STREAM_TEST_RESULT_WRITE_CONST_ACCESS, (canViewStreamWrite<::llc::view_stream<cnst ::llc::u0_t>, ::llc::u2_t>));
	if_fail_fe(::testViewStreamType<::llc::u0_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::u1_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::u2_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::u3_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::s0_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::s1_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::s2_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::s3_t>(errors));
	if_fail_fe(::testViewStreamType<::llc::f2_t>(errors));
	rtrn ::testViewStreamType<::llc::f3_t>(errors);
}
