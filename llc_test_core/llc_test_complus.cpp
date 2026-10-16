#include "llc_test_core.h"
#include "llc_complus.h"

#include <utility>

GDEFINE_ENUM_TYPE(COMPLUS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_RELEASE_RESULT			,  0, "safe_release() returned the wrong remaining reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_RELEASE_POINTER		,  1, "safe_release() retained the released pointer.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_RELEASE_COUNT			,  2, "safe_release() left the wrong object reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_RELEASE_EMPTY_STORAGE	,  3, "safe_release() mishandled an empty storage slot.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_ADDREF_RESULT			,  4, "safe_addref() returned the wrong acquired reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_ADDREF_POINTER		,  5, "safe_addref() stored the wrong pointer.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_ADDREF_COUNT			,  6, "safe_addref() left the wrong object reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SAFE_ADDREF_EMPTY_STORAGE	,  7, "safe_addref() mishandled an empty storage slot.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, DEFAULT_POINTER			,  8, "A default pcom<> retained a pointer.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ADOPT_IDENTITY				,  9, "Raw construction did not adopt the supplied interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ADOPT_COUNT				, 10, "Raw construction changed the adopted reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ADOPT_RELEASE				, 11, "Destruction did not release the adopted reference.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, COPY_IDENTITY				, 12, "Copy construction changed the interface identity.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, COPY_COUNT				, 13, "Copy construction did not acquire one reference.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, MOVE_SOURCE				, 14, "Move construction retained the source interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, MOVE_IDENTITY				, 15, "Move construction changed the interface identity.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, MOVE_COUNT				, 16, "Move construction changed the reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SELF_MOVE_IDENTITY		, 17, "Self-move assignment lost the interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SELF_MOVE_COUNT			, 18, "Self-move assignment changed the reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, FIRST_CLEAR_RESULT		, 19, "Clearing one shared owner returned the wrong count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, FIRST_CLEAR_POINTER		, 20, "Clearing one shared owner retained its pointer.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, FINAL_CLEAR_RESULT		, 21, "Clearing the final owner returned the wrong count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, FINAL_CLEAR_POINTER		, 22, "Clearing the final owner retained its pointer.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, COPY_ASSIGN_IDENTITY		, 23, "Copy assignment stored the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, COPY_ASSIGN_ACQUIRE		, 24, "Copy assignment left the acquired interface with the wrong count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, COPY_ASSIGN_RELEASE		, 25, "Copy assignment did not release the replaced interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SELF_COPY_IDENTITY		, 26, "Self-copy assignment changed the interface identity.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, SELF_COPY_COUNT			, 27, "Self-copy assignment changed the reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, OUTPUT_PRESERVED			, 28, "COM address access changed the previous reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, OUTPUT_ADDRESS			, 29, "COM address access returned the wrong interface slot.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, OUTPUT_RELEASE			, 34, "Explicit clear did not release the previous interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, OUTPUT_EMPTY				, 35, "Explicit clear retained the previous interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, OUTPUT_IDENTITY			, 36, "COM output access stored the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_RAW_RESULT			, 37, "Raw QueryInterface returned failure for IUnknown.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_RAW_IDENTITY		, 38, "Raw QueryInterface stored the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_RAW_ACQUIRE		, 39, "Raw QueryInterface left the source with the wrong count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_RAW_RELEASE		, 40, "Raw QueryInterface did not release the replaced interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_PTR_RESULT			, 41, "pcom QueryInterface returned failure for IUnknown.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_PTR_IDENTITY		, 42, "pcom QueryInterface stored the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_PTR_ACQUIRE		, 43, "pcom QueryInterface left the source with the wrong count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_PTR_RELEASE		, 44, "pcom QueryInterface did not release the replaced interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_SOURCE_IDENTITY		, 45, "The query setup lost its source interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_SOURCE_COUNT		, 46, "The query setup changed the source reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_SOURCE_AFTER_MISSING	, 47, "An unsupported query changed the source reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_NULL_RESULT			, 48, "A null source returned a nonzero result.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_NULL_EMPTY			, 49, "A null source retained the destination interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_MISSING_RESULT		, 50, "An unsupported interface reported successful QueryInterface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_MISSING_EMPTY		, 51, "An unsupported query retained the destination interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_MISSING_COUNT		, 52, "An unsupported query did not release the replaced destination.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_APPEND_RESULT		, 53, "The COM-pointer array rejected an element.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_SIZE				, 54, "The COM-pointer array has the wrong size.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_IDENTITY			, 55, "The COM-pointer array stored the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_COUNT				, 56, "The COM-pointer array did not acquire its own reference.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_VIEW_SIZE			, 57, "The COM-pointer view has the wrong size.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_VIEW_IDENTITY		, 58, "The COM-pointer view exposes the wrong interface.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_OWNER_RELEASE		, 59, "Releasing the original owner changed the array reference count.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, ARRAY_FINAL_RELEASE		, 60, "Clearing the COM-pointer array retained a reference.");
GDEFINE_ENUM_VALUED(COMPLUS_TEST_RESULT, QUERY_NULL_RELEASE			, 61, "A null source did not release the replaced destination.");

#ifdef LLC_WINDOWS

struct __declspec(uuid("D1F1944D-3460-43E0-9692-D41D9614653D")) ICOMTestMissing : IUnknown {};

stct SCOMTestUnknown final : IUnknown {
	ULONG References = 1;
	ULONG AddCalls = 0;
	ULONG ReleaseCalls = 0;
	ULONG QueryCalls = 0;

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, void** output) override {
		++QueryCalls;
		if(0 == output)
			rtrn E_POINTER;
		*output = {};
		if(interfaceId != __uuidof(IUnknown))
			rtrn E_NOINTERFACE;
		*output = static_cast<IUnknown*>(this);
		AddRef();
		rtrn S_OK;
	}
	ULONG STDMETHODCALLTYPE AddRef() override { ++AddCalls; rtrn ++References; }
	ULONG STDMETHODCALLTYPE Release() override { ++ReleaseCalls; rtrn --References; }
};

stct SCOMTestMissing final : ICOMTestMissing {
	ULONG References = 1;

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, void** output) override {
		if(0 == output)
			rtrn E_POINTER;
		*output = {};
		if(interfaceId != __uuidof(IUnknown) && interfaceId != __uuidof(ICOMTestMissing))
			rtrn E_NOINTERFACE;
		*output = static_cast<ICOMTestMissing*>(this);
		AddRef();
		rtrn S_OK;
	}
	ULONG STDMETHODCALLTYPE AddRef() override { rtrn ++References; }
	ULONG STDMETHODCALLTYPE Release() override { rtrn --References; }
};

sttc ::llc::err_t testCOMSafeFunctions(ATestError& errors) {
	SCOMTestUnknown released;
	IUnknown* pointer = &released;
	::llc::u2_c releaseResult = ::llc::safe_release(&pointer);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_RELEASE_RESULT, releaseResult != 0, "Remaining references:%u, expected:0.", releaseResult);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_RELEASE_POINTER, pointer, "Released pointer:%p, expected:null.", pointer);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_RELEASE_COUNT, released.References != 0, "References:%lu, expected:0.", released.References);
	IUnknown* emptyStorage = 0;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_RELEASE_EMPTY_STORAGE, ::llc::safe_release(&emptyStorage) != 0, "%s", "Empty storage returned a reference count.");

	SCOMTestUnknown acquired;
	pointer = {};
	::llc::u2_c addResult = ::llc::safe_addref(&pointer, static_cast<IUnknown*>(&acquired));
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_ADDREF_RESULT, addResult != 2, "Acquired references:%u, expected:2.", addResult);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_ADDREF_POINTER, pointer != &acquired, "Stored pointer:%p, expected:%p.", pointer, &acquired);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_ADDREF_COUNT, acquired.References != 2, "References:%lu, expected:2.", acquired.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SAFE_ADDREF_EMPTY_STORAGE, ::llc::safe_addref(&emptyStorage, (IUnknown*)0) != 0, "%s", "Empty storage returned a reference count.");
	::llc::safe_release(&pointer);
	acquired.Release();
	rtrn 0;
}

sttc ::llc::err_t testCOMOwnership(ATestError& errors) {
	cnst ::llc::pcom<IUnknown> empty;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_DEFAULT_POINTER, empty.get_ref(), "Default pointer:%p, expected:null.", empty.get_ref());

	SCOMTestUnknown adopted;
	{
		::llc::pcom<IUnknown> owner = &adopted;
		LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ADOPT_IDENTITY, owner.get() != &adopted, "Adopted pointer:%p, expected:%p.", owner.get(), &adopted);
		LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ADOPT_COUNT, adopted.References != 1, "References:%lu, expected:1.", adopted.References);
	}
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ADOPT_RELEASE, adopted.References != 0, "References:%lu, expected:0.", adopted.References);

	SCOMTestUnknown shared;
	::llc::pcom<IUnknown> owner = &shared;
	::llc::pcom<IUnknown> copy = owner;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_COPY_IDENTITY, copy.get() != owner.get(), "Copy pointer:%p, owner:%p.", copy.get(), owner.get());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_COPY_COUNT, shared.References != 2, "References:%lu, expected:2.", shared.References);
	::llc::pcom<IUnknown> moved = ::std::move(copy);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_MOVE_SOURCE, copy.get(), "Moved-from pointer:%p, expected:null.", copy.get());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_MOVE_IDENTITY, moved.get() != owner.get(), "Moved pointer:%p, owner:%p.", moved.get(), owner.get());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_MOVE_COUNT, shared.References != 2, "References:%lu, expected:2.", shared.References);
	moved = ::std::move(moved);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SELF_MOVE_IDENTITY, moved.get() != &shared, "Self-moved pointer:%p, expected:%p.", moved.get(), &shared);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SELF_MOVE_COUNT, shared.References != 2, "References:%lu, expected:2.", shared.References);
	::llc::u2_c firstClear = owner.clear();
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_FIRST_CLEAR_RESULT, firstClear != 1, "Remaining references:%u, expected:1.", firstClear);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_FIRST_CLEAR_POINTER, owner.get(), "Cleared pointer:%p, expected:null.", owner.get());
	::llc::u2_c finalClear = moved.clear();
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_FINAL_CLEAR_RESULT, finalClear != 0, "Remaining references:%u, expected:0.", finalClear);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_FINAL_CLEAR_POINTER, moved.get(), "Cleared pointer:%p, expected:null.", moved.get());
	rtrn 0;
}

sttc ::llc::err_t testCOMReplacement(ATestError& errors) {
	SCOMTestUnknown source, replaced;
	::llc::pcom<IUnknown> sourceOwner = &source;
	::llc::pcom<IUnknown> destination = &replaced;
	destination = sourceOwner;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_COPY_ASSIGN_IDENTITY, destination.get() != &source, "Assigned pointer:%p, expected:%p.", destination.get(), &source);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_COPY_ASSIGN_ACQUIRE, source.References != 2, "Source references:%lu, expected:2.", source.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_COPY_ASSIGN_RELEASE, replaced.References != 0, "Replaced references:%lu, expected:0.", replaced.References);
	destination = destination;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SELF_COPY_IDENTITY, destination.get() != &source, "Self-assigned pointer:%p, expected:%p.", destination.get(), &source);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_SELF_COPY_COUNT, source.References != 2, "Source references:%lu, expected:2.", source.References);
	destination.clear();
	sourceOwner.clear();

	SCOMTestUnknown outputPrevious, outputNext;
	::llc::pcom<IUnknown> outputOwner = &outputPrevious;
	IUnknown** output = &outputOwner;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_OUTPUT_PRESERVED, outputPrevious.References != 1, "Previous references:%lu, expected:1.", outputPrevious.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_OUTPUT_ADDRESS, *output != &outputPrevious, "Output slot:%p, expected:%p.", *output, &outputPrevious);
	outputOwner.clear();
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_OUTPUT_RELEASE, outputPrevious.References != 0, "Previous references:%lu, expected:0.", outputPrevious.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_OUTPUT_EMPTY, outputOwner.get(), "Output owner:%p, expected:null.", outputOwner.get());
	*output = &outputNext;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_OUTPUT_IDENTITY, outputOwner.get() != &outputNext, "Output pointer:%p, expected:%p.", outputOwner.get(), &outputNext);
	outputOwner.clear();
	rtrn 0;
}

sttc ::llc::err_t testCOMQuery(ATestError& errors) {
	SCOMTestUnknown source, rawReplaced;
	::llc::pcom<IUnknown> sourceOwner = &source;
	IUnknown* rawOutput = &rawReplaced;
	::llc::err_t result = sourceOwner.as(&rawOutput);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_RAW_RESULT, ::llc::failed(result), "Query result:0x%X.", (::llc::u2_t)result);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_RAW_IDENTITY, rawOutput != &source, "Query pointer:%p, expected:%p.", rawOutput, &source);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_RAW_ACQUIRE, source.References != 2, "Source references:%lu, expected:2.", source.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_RAW_RELEASE, rawReplaced.References != 0, "Replaced references:%lu, expected:0.", rawReplaced.References);
	rawOutput->Release();

	SCOMTestUnknown pointerReplaced;
	::llc::pcom<IUnknown> pointerOutput = &pointerReplaced;
	result = sourceOwner.as(pointerOutput);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_PTR_RESULT, ::llc::failed(result), "Query result:0x%X.", (::llc::u2_t)result);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_PTR_IDENTITY, pointerOutput.get() != &source, "Query pointer:%p, expected:%p.", pointerOutput.get(), &source);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_PTR_ACQUIRE, source.References != 2, "Source references:%lu, expected:2.", source.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_PTR_RELEASE, pointerReplaced.References != 0, "Replaced references:%lu, expected:0.", pointerReplaced.References);
	pointerOutput.clear();

	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_SOURCE_IDENTITY, sourceOwner.get() != &source, "Source pointer:%p, expected:%p.", sourceOwner.get(), &source);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_SOURCE_COUNT, source.References != 1, "Source references:%lu, expected:1.", source.References);

	SCOMTestUnknown nullPreserved;
	::llc::pcom<IUnknown> nullSource;
	::llc::pcom<IUnknown> nullOutput = &nullPreserved;
	result = nullSource.as(nullOutput);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_NULL_RESULT, result != 0, "Query result:0x%X, expected:0.", (::llc::u2_t)result);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_NULL_EMPTY, nullOutput.get(), "Destination pointer:%p, expected:null.", nullOutput.get());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_NULL_RELEASE, nullPreserved.References != 0, "Replaced references:%lu, expected:0.", nullPreserved.References);

	SCOMTestMissing missing;
	::llc::pcom<ICOMTestMissing> missingOutput = &missing;
	result = sourceOwner.as(missingOutput);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_MISSING_RESULT, false == ::llc::failed(result), "Query result:0x%X, expected failure.", (::llc::u2_t)result);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_MISSING_EMPTY, missingOutput.get(), "Destination pointer:%p, expected:null.", missingOutput.get());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_MISSING_COUNT, missing.References != 0, "Missing-interface references:%lu, expected:0.", missing.References);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_QUERY_SOURCE_AFTER_MISSING, source.References != 1, "Source references:%lu, expected:1.", source.References);
	missingOutput.clear();
	nullOutput.clear();
	sourceOwner.clear();
	rtrn 0;
}

sttc ::llc::err_t testCOMArray(ATestError& errors) {
	SCOMTestUnknown element;
	::llc::pcom<IUnknown> owner = &element;
	::llc::acom<IUnknown> array;
	::llc::err_t result = array.push_back(owner);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_APPEND_RESULT, ::llc::failed(result), "Append result:%i.", result);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_SIZE, array.size() != 1, "Array size:%u, expected:1.", array.size());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_IDENTITY, array[0].get() != &element, "Array pointer:%p, expected:%p.", array[0].get(), &element);
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_COUNT, element.References != 2, "Element references:%lu, expected:2.", element.References);
	::llc::vcom<IUnknown> view = array;
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_VIEW_SIZE, view.size() != 1, "View size:%u, expected:1.", view.size());
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_VIEW_IDENTITY, view[0].get() != &element, "View pointer:%p, expected:%p.", view[0].get(), &element);
	owner.clear();
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_OWNER_RELEASE, element.References != 1, "Element references:%lu, expected:1.", element.References);
	array.clear();
	LLC_TEST_CHECKF(errors, COMPLUS_TEST_RESULT_ARRAY_FINAL_RELEASE, element.References != 0, "Element references:%lu, expected:0.", element.References);
	rtrn 0;
}

::llc::err_t testCOMPlus(ATestError& errors) {
	if_fail_fe(testCOMSafeFunctions(errors));
	if_fail_fe(testCOMOwnership(errors));
	if_fail_fe(testCOMReplacement(errors));
	if_fail_fe(testCOMQuery(errors));
	rtrn testCOMArray(errors);
}

#else

::llc::err_t testCOMPlus(ATestError&) { rtrn 0; }

#endif
