#include "llc_test_core.h"
#include "llc_ptr_pod.h"
#include "llc_ptr_obj.h"

#include <utility>

GDEFINE_ENUM_TYPE(PTR_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, DEFAULT_NCO		, 10, "A default pnco<> contained a reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, DEFAULT_POD		, 11, "A default ppod<> contained a reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, DEFAULT_OBJECT	, 12, "A default pobj<> contained a reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_INSTANCE		, 13, "Mutable ppod<> access returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_REFERENCE	, 14, "Mutable ppod<> access returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_COUNT		, 15, "ppod<> has the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_VALUE		, 16, "ppod<> has the wrong value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_COPY_COUNT	, 17, "Copying ppod<> did not increment its reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_COPY_IDENTITY	, 18, "Moving a copied ppod<> changed its reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_MOVED_FROM	, 19, "Moving ppod<> retained the source reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_MOVE_IDENTITY	, 20, "Moving ppod<> changed its reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_RELEASE_RESULT	, 21, "Releasing ppod<> failed.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_SHARED_REFERENCE, 22, "The shared ppod<> reference disappeared after releasing another owner.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_RELEASE_COUNT	, 23, "Releasing one ppod<> owner left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_RELEASE_VALUE	, 24, "Releasing one ppod<> owner changed the value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_FINAL_RESULT	, 25, "Releasing the last ppod<> owner failed.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_FINAL_REFERENCE, 26, "Releasing the last ppod<> owner retained a reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CREATE_INSTANCE, 27, "ppod<>::create() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CREATE_REFERENCE, 28, "ppod<>::create() returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CREATE_VALUE	, 29, "ppod<>::create() constructed the wrong value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CONST_ACCESS	, 30, "Const ppod<> access changed the instance identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, POD_CREATE_COUNT	, 31, "ppod<>::create() left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, RAW_INSTANCE		, 32, "pobj<>::allocate() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, RAW_REFERENCE	, 33, "pobj<>::allocate() returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, RAW_COUNT		, 34, "pobj<>::allocate() left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, RAW_VALUE		, 35, "pobj<>::allocate() did not preserve the written value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_INSTANCE	, 36, "Mutable pobj<> access returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REFERENCE, 37, "Mutable pobj<> access returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_CONSTRUCTED, 38, "Mutable pobj<> access constructed the wrong number of objects.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_NOT_DESTROYED, 39, "Mutable pobj<> access destroyed the object prematurely.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_DEFAULT_VALUE, 40, "Mutable pobj<> access constructed the wrong default value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_ACCESS_IDENTITY, 41, "Repeated pobj<> access changed the instance identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_COPY_COUNT, 42, "Copying pobj<> left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_COPY_IDENTITY, 43, "Copying pobj<> changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_MOVED_FROM, 44, "Moving pobj<> retained the source reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_MOVE_IDENTITY, 45, "Moving pobj<> changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_SHARED_ALIVE, 46, "Releasing one pobj<> owner destroyed the shared object.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_SHARED_REFERENCE, 47, "The shared pobj<> reference disappeared after releasing another owner.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_SHARED_COUNT, 48, "Releasing one pobj<> owner left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_FINAL_DESTRUCTION, 49, "Releasing the last pobj<> owner did not destroy exactly one object.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_CREATE_INSTANCE, 50, "pobj<>::create() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_INSTANCE, 51, "Replacing pobj<> returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_REFERENCE, 52, "Replacing pobj<> lost its reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_VALUE, 53, "Replacing pobj<> constructed the wrong value.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_CONSTRUCTIONS, 54, "Replacing pobj<> left the wrong construction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_DESTRUCTIONS, 55, "Replacing pobj<> left the wrong destruction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_REPLACE_COUNT, 56, "Replacing pobj<> left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_LIFETIME_CONSTRUCTIONS, 57, "pobj<> lifecycle had the wrong construction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, OBJECT_LIFETIME_DESTRUCTIONS, 58, "pobj<> lifecycle had the wrong destruction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_SOURCE_INSTANCE, 59, "NCO source creation returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_SOURCE_REFERENCE, 60, "NCO source creation returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_COPY_COUNT, 61, "NCO copying left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_COPY_IDENTITY, 62, "NCO copy changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_ASSIGNED_IDENTITY, 63, "NCO assignment changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_MOVED_FROM, 64, "NCO move retained the source reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_MOVE_ASSIGNED_FROM, 65, "NCO move assignment retained the source reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_MOVE_IDENTITY, 66, "NCO move changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_MOVE_ASSIGNED_IDENTITY, 67, "NCO move assignment changed the reference identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_MOVE_COUNT, 68, "NCO move changed the reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_OWNER_EQUAL_MOVED, 69, "The NCO owner and moved pointer compare unequal.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_OWNER_EQUAL_ASSIGNED, 70, "The NCO owner and move-assigned pointer compare unequal.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_INSTANCE_IDENTITY, 71, "NCO moved access changed the instance identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_SHARED_ALIVE, 72, "Releasing shared NCO owners destroyed the instance prematurely.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_SHARED_REFERENCE, 73, "Releasing shared NCO owners lost the remaining reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_SHARED_COUNT, 74, "Releasing shared NCO owners left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_FINAL_CONSTRUCTIONS, 75, "NCO lifecycle had the wrong construction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, NCO_FINAL_DESTRUCTIONS, 76, "NCO lifecycle had the wrong destruction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_INSTANCE, 77, "ref_create() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_REFERENCE, 78, "ref_create() returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_AS_RESULT, 79, "pnco<>::as() returned the wrong instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_AS_OUTPUT, 80, "pnco<>::as() set the wrong output instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_AS_COUNT, 81, "pnco<>::as() changed the reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_REPLACEMENT_INSTANCE, 82, "Replacement ref_create() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_REPLACEMENT_REFERENCE, 83, "Replacement ref_create() returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_SET_REFERENCE, 84, "set_ref() lost the replacement reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_SET_INSTANCE, 85, "set_ref() produced the wrong instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_SET_COUNT, 86, "set_ref() left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_SET_CONSTRUCTIONS, 87, "set_ref() left the wrong construction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_SET_DESTRUCTIONS, 88, "set_ref() left the wrong destruction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_ADOPT_INSTANCE, 89, "Adopted ref_create() returned no instance.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_ADOPT_REFERENCE, 90, "Adopted ref_create() returned no reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_ADOPT_OWNER, 91, "NCO adoption lost the reference.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_ADOPT_IDENTITY, 92, "NCO adoption changed the instance identity.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_ADOPT_COUNT, 93, "NCO adoption left the wrong reference count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_FINAL_CONSTRUCTIONS, 94, "Raw reference lifecycle had the wrong construction count.");
GDEFINE_ENUM_VALUED(PTR_TEST_RESULT, BOUNDARY_FINAL_DESTRUCTIONS, 95, "Raw reference lifecycle had the wrong destruction count.");

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
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_DEFAULT_NCO, nco.get_ref(), "Default pnco reference:%p.", nco.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_DEFAULT_POD, pod.get_ref(), "Default ppod reference:%p.", pod.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_DEFAULT_OBJECT, obj.get_ref(), "Default pobj reference:%p.", obj.get_ref());
	rtrn 0;
}

sttc ::llc::err_t testPointerPOD(ATestError & errors) {
	::llc::ppod<SPtrPodValue>	pointer;
	SPtrPodValue					* instance			= pointer.operator->();
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_INSTANCE, 0 == instance, "Lazy POD instance:%p.", instance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_REFERENCE, 0 == pointer.get_ref(), "Lazy POD reference:%p.", pointer.get_ref());
	instance->Value = 0x12345678U;
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_COUNT, pointer.get_ref()->References != 1 , "Lazy POD references:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_VALUE, pointer->Value != 0x12345678U , "Lazy POD value:%" LLC_FMT_U2 ".", pointer->Value);

	::llc::ppod<SPtrPodValue>	copy				= pointer;
	::llc::ppod<SPtrPodValue>	moved				= ::std::move(copy);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_COPY_COUNT, pointer.get_ref()->References != 2 , "POD copied references:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_COPY_IDENTITY, moved.get_ref() != pointer.get_ref() , "POD copied reference:%p, source:%p.", moved.get_ref(), pointer.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_MOVED_FROM, copy.get_ref() , "POD moved-from reference:%p.", copy.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_MOVE_IDENTITY, moved.get_ref() != pointer.get_ref() , "POD moved reference:%p, source:%p.", moved.get_ref(), pointer.get_ref());

	cnst ::llc::err_t			firstRelease		= pointer.clear();
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_RELEASE_RESULT, ::llc::failed(firstRelease), "POD release result:%i.", firstRelease);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_SHARED_REFERENCE, 0 == moved.get_ref(), "Remaining POD reference:%p.", moved.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_RELEASE_COUNT, moved.get_ref()->References != 1 , "Remaining POD references:%" LLC_FMT_S2 ".", (::llc::s2_t)moved.get_ref()->References);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_RELEASE_VALUE, moved->Value != 0x12345678U , "Remaining POD value:%" LLC_FMT_U2 ".", moved->Value);
	cnst ::llc::err_t			finalRelease		= moved.clear();
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_FINAL_RESULT, ::llc::failed(finalRelease), "POD final release result:%i.", finalRelease);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_FINAL_REFERENCE, moved.get_ref(), "POD reference after final release:%p.", moved.get_ref());

	instance = pointer.create(0x89ABCDEFU);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_CREATE_INSTANCE, 0 == instance, "Created POD instance:%p.", instance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_POD_CREATE_REFERENCE, 0 == pointer.get_ref(), "Created POD reference:%p.", pointer.get_ref());
	cnst ::llc::ppod<SPtrPodValue> & constPointer = pointer;
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_CREATE_VALUE, instance->Value != 0x89ABCDEFU , "Created POD value:%" LLC_FMT_U2 ".", instance->Value);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_CONST_ACCESS, constPointer.operator->() != instance , "Const POD instance:%p, mutable:%p.", constPointer.operator->(), instance);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_POD_CREATE_COUNT, pointer.get_ref()->References != 1 , "Created POD references:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);
	if_fail_fe(pointer.clear());

	::llc::pobj<SPtrPodValue>	rawObject;
	SPtrPodValue					* rawInstance			= rawObject.allocate();
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_RAW_INSTANCE, 0 == rawInstance, "Raw object instance:%p.", rawInstance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_RAW_REFERENCE, 0 == rawObject.get_ref(), "Raw object reference:%p.", rawObject.get_ref());
	rawInstance->Value = 0xA5A5A5A5U;
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_RAW_COUNT, rawObject.get_ref()->References != 1 , "Raw object references:%" LLC_FMT_S2 ".", (::llc::s2_t)rawObject.get_ref()->References);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_RAW_VALUE, rawInstance->Value != 0xA5A5A5A5U , "Raw object value:%" LLC_FMT_U2 ".", rawInstance->Value);
	if_fail_fe(rawObject.clear());
	rtrn 0;
}

sttc ::llc::err_t testPointerObject(ATestError & errors) {
	SPtrObject::reset();
	::llc::pobj<SPtrObject>	pointer;
	SPtrObject					* instance			= pointer.operator->();
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_INSTANCE, 0 == instance, "Lazy object instance:%p.", instance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_REFERENCE, 0 == pointer.get_ref(), "Lazy object reference:%p.", pointer.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_CONSTRUCTED, SPtrObject::Constructions != 1 , "Lazy object constructions:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_NOT_DESTROYED, SPtrObject::Destructions , "Lazy object destructions:%u.", SPtrObject::Destructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_DEFAULT_VALUE, instance->Value != 0xC0DEU , "Lazy object value:%" LLC_FMT_U2 ".", instance->Value);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_ACCESS_IDENTITY, pointer.operator->() != instance , "Repeated object instance:%p, first:%p.", pointer.operator->(), instance);

	::llc::pobj<SPtrObject>	copy				= pointer;
	::llc::pobj<SPtrObject>	moved				= ::std::move(copy);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_COPY_COUNT, pointer.get_ref()->References != 2 , "Object copied references:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_COPY_IDENTITY, moved.get_ref() != pointer.get_ref() , "Object copied reference:%p, source:%p.", moved.get_ref(), pointer.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_MOVED_FROM, copy.get_ref(), "Object moved-from reference:%p.", copy.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_MOVE_IDENTITY, moved.get_ref() != pointer.get_ref() , "Object moved reference:%p, source:%p.", moved.get_ref(), pointer.get_ref());

	if_fail_fe(pointer.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_SHARED_ALIVE, SPtrObject::Destructions , "Object destructions after shared release:%u.", SPtrObject::Destructions);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_SHARED_REFERENCE, 0 == moved.get_ref() , "Object reference after shared release:%p.", moved.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_SHARED_COUNT, moved.get_ref()->References != 1 , "Object references after shared release:%" LLC_FMT_S2 ".", (::llc::s2_t)moved.get_ref()->References);
	if_fail_fe(moved.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_FINAL_DESTRUCTION, SPtrObject::Destructions != 1 , "Object final release mismatch. constructions:%u, destructions:%u." , SPtrObject::Constructions, SPtrObject::Destructions );

	instance = pointer.create(0x1234U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_CREATE_INSTANCE, 0 == instance , "Object creation failed. reference:%p." , pointer.get_ref() );
	SPtrObject * replacement = pointer.create(0x5678U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_INSTANCE, 0 == replacement , "Object replacement failed. reference:%p." , pointer.get_ref() );
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_REFERENCE, 0 == pointer.get_ref() , "Object replacement reference:%p.", pointer.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_VALUE, replacement->Value != 0x5678U , "Replacement value:%" LLC_FMT_U2 ".", replacement->Value);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_CONSTRUCTIONS, SPtrObject::Constructions != 3 , "Object constructions after replacement:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_DESTRUCTIONS, SPtrObject::Destructions != 2 , "Object destructions after replacement:%u.", SPtrObject::Destructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_REPLACE_COUNT, pointer.get_ref()->References != 1 , "Object references after replacement:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);
	if_fail_fe(pointer.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_LIFETIME_CONSTRUCTIONS, SPtrObject::Constructions != 3 , "Object lifecycle constructions:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_OBJECT_LIFETIME_DESTRUCTIONS, SPtrObject::Destructions != 3 , "Object lifecycle destructions:%u.", SPtrObject::Destructions);
	rtrn 0;
}

sttc ::llc::err_t testPointerNCO(ATestError & errors) {
	SPtrObject::reset();
	::llc::pobj<SPtrObject>	owner;
	SPtrObject					* instance			= owner.create(0x77U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_SOURCE_INSTANCE, 0 == instance, "NCO source instance:%p.", instance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_SOURCE_REFERENCE, 0 == owner.get_ref(), "NCO source reference:%p.", owner.get_ref());

	::llc::pnco<SPtrObject>	shared				= owner;
	::llc::pnco<SPtrObject>	assigned;
	assigned = shared;
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_COPY_COUNT, owner.get_ref()->References != 3 , "NCO copied references:%" LLC_FMT_S2 ".", (::llc::s2_t)owner.get_ref()->References);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_COPY_IDENTITY, shared.get_ref() != owner.get_ref() , "NCO copied reference:%p, owner:%p.", shared.get_ref(), owner.get_ref());
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_ASSIGNED_IDENTITY, assigned.get_ref() != owner.get_ref() , "NCO assigned reference:%p, owner:%p.", assigned.get_ref(), owner.get_ref());

	::llc::pnco<SPtrObject>	moved				= ::std::move(shared);
	::llc::pnco<SPtrObject>	moveAssigned;
	moveAssigned = ::std::move(assigned);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_MOVED_FROM, shared.get_ref(), "NCO moved-from reference:%p.", shared.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_MOVE_ASSIGNED_FROM, assigned.get_ref(), "NCO move-assigned source:%p.", assigned.get_ref());
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_MOVE_IDENTITY, moved.get_ref() != owner.get_ref() , "NCO moved reference:%p, owner:%p.", moved.get_ref(), owner.get_ref());
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_MOVE_ASSIGNED_IDENTITY, moveAssigned.get_ref() != owner.get_ref() , "NCO move-assigned reference:%p, owner:%p.", moveAssigned.get_ref(), owner.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_MOVE_COUNT, owner.get_ref()->References != 3 , "NCO references after move:%" LLC_FMT_S2 ".", (::llc::s2_t)owner.get_ref()->References);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_OWNER_EQUAL_MOVED, owner != moved , "NCO owner:%p, moved:%p.", owner.get_ref(), moved.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_OWNER_EQUAL_ASSIGNED, owner != moveAssigned , "NCO owner:%p, move-assigned:%p.", owner.get_ref(), moveAssigned.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_INSTANCE_IDENTITY, moved.operator->() != instance , "NCO moved instance:%p, expected:%p.", moved.operator->(), instance);

	if_fail_fe(owner.clear());
	if_fail_fe(moved.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_SHARED_ALIVE, SPtrObject::Destructions , "NCO destructions after shared release:%u.", SPtrObject::Destructions);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_NCO_SHARED_REFERENCE, 0 == moveAssigned.get_ref() , "NCO remaining reference:%p.", moveAssigned.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_SHARED_COUNT, moveAssigned.get_ref()->References != 1 , "NCO remaining references:%" LLC_FMT_S2 ".", (::llc::s2_t)moveAssigned.get_ref()->References);
	if_fail_fe(moveAssigned.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_FINAL_CONSTRUCTIONS, SPtrObject::Constructions != 1 , "NCO lifecycle constructions:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_NCO_FINAL_DESTRUCTIONS, SPtrObject::Destructions != 1 , "NCO lifecycle destructions:%u.", SPtrObject::Destructions);
	rtrn 0;
}

sttc ::llc::err_t testPointerReferenceBoundary(ATestError & errors) {
	SPtrObject::reset();
	::llc::pnco<SPtrObject>	pointer;
	SPtrObject					* instance			= ::llc::ref_create(&pointer, 0x11U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_INSTANCE, 0 == instance, "Created boundary instance:%p.", instance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_REFERENCE, 0 == pointer.get_ref(), "Created boundary reference:%p.", pointer.get_ref());
	SPtrObject					* typedInstance		= 0;
	SPtrObject					* asResult			= pointer.as(&typedInstance);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_AS_RESULT, asResult != instance , "Typed result:%p, expected:%p.", asResult, instance);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_AS_OUTPUT, typedInstance != instance , "Typed output:%p, expected:%p.", typedInstance, instance);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_AS_COUNT, pointer.get_ref()->References != 1 , "Typed boundary references:%" LLC_FMT_S2 ".", (::llc::s2_t)pointer.get_ref()->References);

	::llc::gref<SPtrObject>	* replacementRef		= 0;
	SPtrObject					* replacement			= ::llc::ref_create(&replacementRef, 0x22U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_REPLACEMENT_INSTANCE, 0 == replacement , "Replacement instance:%p.", replacement);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_REPLACEMENT_REFERENCE, 0 == replacementRef , "Replacement reference:%p.", replacementRef);
	pointer.set_ref(replacementRef);
	replacementRef = 0;
	SPtrObject					* currentInstance		= pointer.get_ref() ? pointer.operator->() : 0;
	cnst ::llc::s2_t			currentReferences	= pointer.get_ref() ? (::llc::s2_t)pointer.get_ref()->References : -1;
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_SET_REFERENCE, 0 == pointer.get_ref() , "set_ref() reference:%p.", pointer.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_SET_INSTANCE, currentInstance != replacement , "set_ref() instance:%p, expected:%p.", currentInstance, replacement);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_SET_COUNT, currentReferences != 1 , "set_ref() references:%" LLC_FMT_S2 ".", currentReferences);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_SET_CONSTRUCTIONS, SPtrObject::Constructions != 2 , "set_ref() constructions:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_SET_DESTRUCTIONS, SPtrObject::Destructions != 1 , "set_ref() destructions:%u.", SPtrObject::Destructions);

	::llc::gref<SPtrObject>	* adoptedRef			= 0;
	SPtrObject					* adoptedInstance		= ::llc::ref_create(&adoptedRef, 0x33U);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_ADOPT_INSTANCE, 0 == adoptedInstance , "Adopted instance:%p.", adoptedInstance);
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_ADOPT_REFERENCE, 0 == adoptedRef , "Adopted reference:%p.", adoptedRef);
	::llc::pnco<SPtrObject>	adopted				= adoptedRef;
	adoptedRef = 0;
	SPtrObject					* adoptedCurrent		= adopted.get_ref() ? adopted.operator->() : 0;
	cnst ::llc::s2_t			adoptedReferences	= adopted.get_ref() ? (::llc::s2_t)adopted.get_ref()->References : -1;
	LLC_TEST_REQUIREF(errors, PTR_TEST_RESULT_BOUNDARY_ADOPT_OWNER, 0 == adopted.get_ref() , "Adopted owner reference:%p.", adopted.get_ref());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_ADOPT_IDENTITY, adoptedCurrent != adoptedInstance , "Adopted current instance:%p, expected:%p.", adoptedCurrent, adoptedInstance);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_ADOPT_COUNT, adoptedReferences != 1 , "Adopted references:%" LLC_FMT_S2 ".", adoptedReferences);

	if_fail_fe(pointer.clear());
	if_fail_fe(adopted.clear());
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_FINAL_CONSTRUCTIONS, SPtrObject::Constructions != 3 , "Boundary lifecycle constructions:%u.", SPtrObject::Constructions);
	LLC_TEST_CHECKF(errors, PTR_TEST_RESULT_BOUNDARY_FINAL_DESTRUCTIONS, SPtrObject::Destructions != 3 , "Boundary lifecycle destructions:%u.", SPtrObject::Destructions);
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
