#include "llc_test_core.h"
#include "llc_ptr_pod.h"
#include "llc_ptr_obj.h"

#include <utility>

GDEFINE_ENUM_TYPE(PTR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, DEFAULT_STATE		, 0, "A default pointer contained a reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_LAZY_ALLOCATE	, 1, "ppod<> mutable access did not allocate one writable instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CREATE			, 2, "ppod<>::create() did not construct the requested POD value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_LAZY_CREATE	, 3, "pobj<> mutable access did not construct exactly one default object.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_CREATE		, 4, "pobj<>::create() did not construct or replace the requested object.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, COPY_ACQUIRE		, 5, "Pointer copy did not acquire the shared reference exactly once.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, MOVE_TRANSFER		, 6, "Pointer move did not transfer the reference without changing its count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, SHARED_RELEASE		, 7, "Releasing a shared pointer destroyed the instance too early or retained the wrong count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, FINAL_RELEASE		, 8, "Releasing the final pointer did not destroy the instance exactly once.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, REFERENCE_BOUNDARY	, 9, "The raw reference boundary did not preserve ownership, identity or typed access.");

// Raw pointers in this suite are the instance and control-block boundaries exposed by ppod<>, pobj<>, pnco<> and ref_*(); their null, identity and output behavior is what these tests verify.

stct SPtrPodValue {
	::llc::u2_t	Value;
};

stct SPtrObject {
	sttc ::llc::u2_t	Constructions;
	sttc ::llc::u2_t	Destructions;
	::llc::u2_t			Value;

	SPtrObject(::llc::u2_c value = 0xC0DEU) : Value(value) { ++Constructions; }
	virtual ~SPtrObject() { ++Destructions; }

	sttc void reset() { Constructions = Destructions = 0; }
};

::llc::u2_t SPtrObject::Constructions = 0;
::llc::u2_t SPtrObject::Destructions = 0;

sttc ::llc::err_t testPointerDefaultState(ATestError & errors) {
	cnst ::llc::pnco<SPtrObject>	nco;
	cnst ::llc::ppod<SPtrPodValue>	pod;
	cnst ::llc::pobj<SPtrObject>	obj;
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_DEFAULT_STATE, nco.get_ref() || pod.get_ref() || obj.get_ref()
		, "Default reference mismatch. pnco:%p, ppod:%p, pobj:%p."
		, nco.get_ref(), pod.get_ref(), obj.get_ref()
		);
	rtrn 0;
}

sttc ::llc::err_t testPointerPOD(ATestError & errors) {
	::llc::ppod<SPtrPodValue>	pointer;
	SPtrPodValue					* instance			= pointer.operator->();
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_POD_LAZY_ALLOCATE, 0 == instance || 0 == pointer.get_ref()
		, "Lazy POD allocation failed. instance:%p, reference:%p."
		, instance, pointer.get_ref()
		);
	instance->Value = 0x12345678U;
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_POD_LAZY_ALLOCATE, pointer.get_ref()->References != 1 || pointer->Value != 0x12345678U
		, "Lazy POD state mismatch. references:%" LLC_FMT_S2 ", value:%" LLC_FMT_U2 "."
		, (::llc::s2_t)pointer.get_ref()->References, pointer->Value
		);

	::llc::ppod<SPtrPodValue>	copy				= pointer;
	::llc::ppod<SPtrPodValue>	moved				= ::std::move(copy);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_COPY_ACQUIRE, pointer.get_ref()->References != 2 || moved.get_ref() != pointer.get_ref()
		, "POD copy mismatch. references:%" LLC_FMT_S2 ", source:%p, copy:%p."
		, (::llc::s2_t)pointer.get_ref()->References, pointer.get_ref(), moved.get_ref()
		);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_MOVE_TRANSFER, copy.get_ref() || moved.get_ref() != pointer.get_ref()
		, "POD move mismatch. moved-from:%p, source:%p, moved:%p."
		, copy.get_ref(), pointer.get_ref(), moved.get_ref()
		);

	cnst ::llc::err_t			firstRelease		= pointer.clear();
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_SHARED_RELEASE, ::llc::failed(firstRelease) || moved.get_ref()->References != 1 || moved->Value != 0x12345678U
		, "POD shared release mismatch. result:%i, references:%" LLC_FMT_S2 ", value:%" LLC_FMT_U2 "."
		, firstRelease, (::llc::s2_t)moved.get_ref()->References, moved->Value
		);
	cnst ::llc::err_t			finalRelease		= moved.clear();
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_FINAL_RELEASE, ::llc::failed(finalRelease) || moved.get_ref()
		, "POD final release mismatch. result:%i, reference:%p."
		, finalRelease, moved.get_ref()
		);

	instance = pointer.create(0x89ABCDEFU);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_POD_CREATE, 0 == instance || 0 == pointer.get_ref()
		, "POD creation failed. instance:%p, reference:%p."
		, instance, pointer.get_ref()
		);
	cnst ::llc::ppod<SPtrPodValue> & constPointer = pointer;
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_POD_CREATE, instance->Value != 0x89ABCDEFU || constPointer.operator->() != instance || pointer.get_ref()->References != 1
		, "POD creation mismatch. value:%" LLC_FMT_U2 ", mutable:%p, const:%p, references:%" LLC_FMT_S2 "."
		, instance->Value, instance, constPointer.operator->(), (::llc::s2_t)pointer.get_ref()->References
		);
	if_fail_fe(pointer.clear());

	::llc::pobj<SPtrPodValue>	rawObject;
	SPtrPodValue					* rawInstance			= rawObject.allocate();
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_POD_LAZY_ALLOCATE, 0 == rawInstance || 0 == rawObject.get_ref()
		, "pobj<> raw allocation failed. instance:%p, reference:%p."
		, rawInstance, rawObject.get_ref()
		);
	rawInstance->Value = 0xA5A5A5A5U;
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_POD_LAZY_ALLOCATE, rawObject.get_ref()->References != 1 || rawInstance->Value != 0xA5A5A5A5U
		, "pobj<> raw allocation mismatch. references:%" LLC_FMT_S2 ", value:%" LLC_FMT_U2 "."
		, (::llc::s2_t)rawObject.get_ref()->References, rawInstance->Value
		);
	if_fail_fe(rawObject.clear());
	rtrn 0;
}

sttc ::llc::err_t testPointerObject(ATestError & errors) {
	SPtrObject::reset();
	::llc::pobj<SPtrObject>	pointer;
	SPtrObject					* instance			= pointer.operator->();
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_OBJECT_LAZY_CREATE, 0 == instance || 0 == pointer.get_ref()
		, "Lazy object creation failed. instance:%p, reference:%p."
		, instance, pointer.get_ref()
		);
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_OBJECT_LAZY_CREATE, SPtrObject::Constructions != 1 || SPtrObject::Destructions || instance->Value != 0xC0DEU || pointer.operator->() != instance
		, "Lazy object state mismatch. constructions:%u, destructions:%u, value:%" LLC_FMT_U2 ", first:%p, second:%p."
		, SPtrObject::Constructions, SPtrObject::Destructions, instance->Value, instance, pointer.operator->()
		);

	::llc::pobj<SPtrObject>	copy				= pointer;
	::llc::pobj<SPtrObject>	moved				= ::std::move(copy);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_COPY_ACQUIRE, pointer.get_ref()->References != 2 || moved.get_ref() != pointer.get_ref()
		, "Object copy mismatch. references:%" LLC_FMT_S2 ", source:%p, copy:%p."
		, (::llc::s2_t)pointer.get_ref()->References, pointer.get_ref(), moved.get_ref()
		);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_MOVE_TRANSFER, copy.get_ref() || moved.get_ref() != pointer.get_ref()
		, "Object move mismatch. moved-from:%p, source:%p, moved:%p."
		, copy.get_ref(), pointer.get_ref(), moved.get_ref()
		);

	if_fail_fe(pointer.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_SHARED_RELEASE, SPtrObject::Destructions || moved.get_ref()->References != 1
		, "Object shared release mismatch. destructions:%u, references:%" LLC_FMT_S2 "."
		, SPtrObject::Destructions, (::llc::s2_t)moved.get_ref()->References
		);
	if_fail_fe(moved.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_FINAL_RELEASE, SPtrObject::Destructions != 1
		, "Object final release mismatch. constructions:%u, destructions:%u."
		, SPtrObject::Constructions, SPtrObject::Destructions
		);

	instance = pointer.create(0x1234U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_OBJECT_CREATE, 0 == instance
		, "Object creation failed. reference:%p."
		, pointer.get_ref()
		);
	SPtrObject * replacement = pointer.create(0x5678U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_OBJECT_CREATE, 0 == replacement
		, "Object replacement failed. reference:%p."
		, pointer.get_ref()
		);
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_OBJECT_CREATE, replacement->Value != 0x5678U || SPtrObject::Constructions != 3 || SPtrObject::Destructions != 2 || pointer.get_ref()->References != 1
		, "Object replacement mismatch. value:%" LLC_FMT_U2 ", constructions:%u, destructions:%u, references:%" LLC_FMT_S2 "."
		, replacement->Value, SPtrObject::Constructions, SPtrObject::Destructions, (::llc::s2_t)pointer.get_ref()->References
		);
	if_fail_fe(pointer.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_FINAL_RELEASE, SPtrObject::Constructions != 3 || SPtrObject::Destructions != 3
		, "Object lifecycle mismatch. constructions:%u, destructions:%u."
		, SPtrObject::Constructions, SPtrObject::Destructions
		);
	rtrn 0;
}

sttc ::llc::err_t testPointerNCO(ATestError & errors) {
	SPtrObject::reset();
	::llc::pobj<SPtrObject>	owner;
	SPtrObject					* instance			= owner.create(0x77U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_OBJECT_CREATE, 0 == instance || 0 == owner.get_ref()
		, "NCO source creation failed. instance:%p, reference:%p."
		, instance, owner.get_ref()
		);

	::llc::pnco<SPtrObject>	shared				= owner;
	::llc::pnco<SPtrObject>	assigned;
	assigned = shared;
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_COPY_ACQUIRE, owner.get_ref()->References != 3 || shared.get_ref() != owner.get_ref() || assigned.get_ref() != owner.get_ref()
		, "NCO copy mismatch. references:%" LLC_FMT_S2 ", owner:%p, copy:%p, assigned:%p."
		, (::llc::s2_t)owner.get_ref()->References, owner.get_ref(), shared.get_ref(), assigned.get_ref()
		);

	::llc::pnco<SPtrObject>	moved				= ::std::move(shared);
	::llc::pnco<SPtrObject>	moveAssigned;
	moveAssigned = ::std::move(assigned);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_MOVE_TRANSFER
		, shared.get_ref() || assigned.get_ref() || moved.get_ref() != owner.get_ref() || moveAssigned.get_ref() != owner.get_ref() || owner.get_ref()->References != 3
		, "NCO move mismatch. references:%" LLC_FMT_S2 ", moved-from:%p/%p, owner:%p, moved:%p/%p."
		, (::llc::s2_t)owner.get_ref()->References, shared.get_ref(), assigned.get_ref(), owner.get_ref(), moved.get_ref(), moveAssigned.get_ref()
		);
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY, owner != moved || owner != moveAssigned || moved.operator->() != instance
		, "NCO identity mismatch. owner:%p, moved:%p, assigned:%p, instance:%p."
		, owner.get_ref(), moved.get_ref(), moveAssigned.get_ref(), instance
		);

	if_fail_fe(owner.clear());
	if_fail_fe(moved.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_SHARED_RELEASE, SPtrObject::Destructions || 0 == moveAssigned.get_ref() || moveAssigned.get_ref()->References != 1
		, "NCO shared release mismatch. destructions:%u, reference:%p, references:%" LLC_FMT_S2 "."
		, SPtrObject::Destructions, moveAssigned.get_ref(), moveAssigned.get_ref() ? (::llc::s2_t)moveAssigned.get_ref()->References : -1
		);
	if_fail_fe(moveAssigned.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_FINAL_RELEASE, SPtrObject::Constructions != 1 || SPtrObject::Destructions != 1
		, "NCO final lifecycle mismatch. constructions:%u, destructions:%u."
		, SPtrObject::Constructions, SPtrObject::Destructions
		);
	rtrn 0;
}

sttc ::llc::err_t testPointerReferenceBoundary(ATestError & errors) {
	SPtrObject::reset();
	::llc::pnco<SPtrObject>	pointer;
	SPtrObject					* instance			= ::llc::ref_create(&pointer, 0x11U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY, 0 == instance || 0 == pointer.get_ref()
		, "operator& reference creation failed. instance:%p, reference:%p."
		, instance, pointer.get_ref()
		);
	SPtrObject					* typedInstance		= 0;
	SPtrObject					* asResult			= pointer.as(&typedInstance);
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY
		, asResult != instance || typedInstance != instance || pointer.get_ref()->References != 1
		, "Typed pointer boundary mismatch. result:%p, typed:%p, expected:%p, references:%" LLC_FMT_S2 "."
		, asResult, typedInstance, instance, (::llc::s2_t)pointer.get_ref()->References
		);

	::llc::gref<SPtrObject>	* replacementRef		= 0;
	SPtrObject					* replacement			= ::llc::ref_create(&replacementRef, 0x22U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY, 0 == replacement || 0 == replacementRef
		, "Replacement reference creation failed. instance:%p, reference:%p."
		, replacement, replacementRef
		);
	pointer.set_ref(replacementRef);
	replacementRef = 0;
	SPtrObject					* currentInstance		= pointer.get_ref() ? pointer.operator->() : 0;
	cnst ::llc::s2_t			currentReferences	= pointer.get_ref() ? (::llc::s2_t)pointer.get_ref()->References : -1;
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY
		, 0 == pointer.get_ref() || currentInstance != replacement || currentReferences != 1 || SPtrObject::Constructions != 2 || SPtrObject::Destructions != 1
		, "set_ref() ownership mismatch. instance:%p, expected:%p, references:%" LLC_FMT_S2 ", constructions:%u, destructions:%u."
		, currentInstance, replacement, currentReferences, SPtrObject::Constructions, SPtrObject::Destructions
		);

	::llc::gref<SPtrObject>	* adoptedRef			= 0;
	SPtrObject					* adoptedInstance		= ::llc::ref_create(&adoptedRef, 0x33U);
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY, 0 == adoptedInstance || 0 == adoptedRef
		, "Adopted reference creation failed. instance:%p, reference:%p."
		, adoptedInstance, adoptedRef
		);
	::llc::pnco<SPtrObject>	adopted				= adoptedRef;
	adoptedRef = 0;
	SPtrObject					* adoptedCurrent		= adopted.get_ref() ? adopted.operator->() : 0;
	cnst ::llc::s2_t			adoptedReferences	= adopted.get_ref() ? (::llc::s2_t)adopted.get_ref()->References : -1;
	LLC_TEST_REQUIRE(errors, PTR_TEST_RESULT_REFERENCE_BOUNDARY, 0 == adopted.get_ref() || adoptedCurrent != adoptedInstance || adoptedReferences != 1
		, "Reference adoption mismatch. instance:%p, expected:%p, references:%" LLC_FMT_S2 "."
		, adoptedCurrent, adoptedInstance, adoptedReferences
		);

	if_fail_fe(pointer.clear());
	if_fail_fe(adopted.clear());
	LLC_TEST_CHECK(errors, PTR_TEST_RESULT_FINAL_RELEASE, SPtrObject::Constructions != 3 || SPtrObject::Destructions != 3
		, "Reference-boundary lifecycle mismatch. constructions:%u, destructions:%u."
		, SPtrObject::Constructions, SPtrObject::Destructions
		);
	rtrn 0;
}

::llc::err_t testPointers(ATestError & errors) {
	if_fail_fe(testPointerDefaultState(errors));
	if_fail_fe(testPointerPOD(errors));
	if_fail_fe(testPointerObject(errors));
	if_fail_fe(testPointerNCO(errors));
	if_fail_fe(testPointerReferenceBoundary(errors));
	rtrn 0;
}
