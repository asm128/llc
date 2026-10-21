#include "llc_test_engine_fixture.h"
#include "llc_file.h"
#include "llc_path.h"

GDEFINE_ENUM_TYPE(ENGINE_SERIAL_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, SAVE             ,  1, "Saving a valid engine manager must succeed.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, PREFIX           ,  2, "Save must append without changing earlier output bytes.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, LOAD             ,  3, "Loading a valid engine manager must succeed.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, CURSOR           ,  4, "Load must consume exactly its own bytes, leaving the next field untouched.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, BYTES            ,  5, "Save/load/save must preserve the serialized manager representation.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, RESOURCE_COUNT   ,  6, "Resource manager load must replace the destination with matching names and element counts.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, RESOURCE_VALUE   ,  7, "Resource payload fields and names must survive loading and outlive the input storage.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, OWNERSHIP        ,  8, "Loaded resource payloads must own independent writable storage.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, TRUNCATED        ,  9, "Resource manager load must reject a truncated serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, REJECTED_CURSOR  , 10, "Rejected resource manager load must preserve the caller's input cursor.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, REJECTED_STATE   , 11, "Rejected resource manager load must preserve existing names, owners and payloads.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, INVALID_SAVE     , 12, "Null resource entries must fail saving without modifying output.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, ENTITY_IDS       , 13, "Entity node/body/parent IDs and names must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, CHILDREN         , 14, "Entity child order and sparse IDs must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, NODE_DATA        , 15, "Render node IDs, flags and both transform arrays must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, LIGHTS           , 16, "Directional, point and spot light payloads and per-node references must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, CAMERAS          , 17, "All camera payloads must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, BODY_DATA        , 18, "Body frames, flags, forces, masses, centers and transform caches must survive serialization.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, BODY_VOLUMES     , 19, "Body bounding volumes must retain their count and half-sizes through save/load.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, BODY_CONTINUATION, 20, "Loaded bodies must continue the same simulation from the saved state.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, ENGINE_LINKS     , 21, "Loaded engine entities must still reference the same valid body/node/mesh/skin/surface graph.");
GDEFINE_ENUM_VALUED(ENGINE_SERIAL_RESULT, RUNTIME_STATE    , 22, "Graphics loading must preserve caller-owned runtime shader/font registrations.");

stxp ::llc::u0_t ENGINE_SAVE_PREFIX[] = {0x31, 0x72, 0xB4};
stxp ::llc::u0_t ENGINE_SAVE_SUFFIX[] = {0xE8, 0x5D, 0x0A, 0xC3};

tplt<tpnm T>
sttc ::llc::err_t engineRoundTrip(ATestError & errors, cnst T & source, T & loaded) {
	::llc::au0_t bytes;
	llc_necs(bytes.append(ENGINE_SAVE_PREFIX));
	cnst ::llc::err_t saveResult = source.Save(bytes);
	LLC_TEST_REQUIREF(errors, ENGINE_SERIAL_RESULT_SAVE, saveResult < 0, "Result:%i.", saveResult);
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_PREFIX, bytes.size() < ::llc::size(ENGINE_SAVE_PREFIX));
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_PREFIX, memcmp(bytes.begin(), ENGINE_SAVE_PREFIX, sizeof(ENGINE_SAVE_PREFIX)));
	cnst ::llc::u2_t size = bytes.size() - ::llc::size(ENGINE_SAVE_PREFIX);
	llc_necs(bytes.append(ENGINE_SAVE_SUFFIX));
	::llc::vcu0_t input = {bytes.begin() + ::llc::size(ENGINE_SAVE_PREFIX), size + ::llc::size(ENGINE_SAVE_SUFFIX)};
	cnst ::llc::err_t loadResult = loaded.Load(input);
	LLC_TEST_REQUIREF(errors, ENGINE_SERIAL_RESULT_LOAD, loadResult < 0, "Result:%i, saved bytes:%u.", loadResult, size);
	LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_CURSOR, input.size() != ::llc::size(ENGINE_SAVE_SUFFIX) || input.begin() != bytes.begin() + ::llc::size(ENGINE_SAVE_PREFIX) + size, "Remaining:%u, expected:%u.", input.size(), ::llc::size(ENGINE_SAVE_SUFFIX));
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_CURSOR, input != ::llc::vcu0_t{ENGINE_SAVE_SUFFIX});
	::llc::au0_t roundtrip;
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_SAVE, loaded.Save(roundtrip) < 0);
	LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_BYTES, (::llc::vcu0_t{roundtrip} != ::llc::vcu0_t{bytes.begin() + ::llc::size(ENGINE_SAVE_PREFIX), size}), "Saved bytes:%u, loaded bytes:%u.", size, roundtrip.size());
	// Invalidate the stream storage before callers inspect names and owned payloads.
	llc_necs(bytes.fill(0xDD));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t engineArrayEqual(ATestError & errors, ENGINE_SERIAL_RESULT result, ::llc::view<cnst T> expected, ::llc::view<cnst T> actual) {
	LLC_TEST_CHECKF(errors, result, expected.size() != actual.size(), "Expected count:%u, actual:%u, element bytes:%u.", expected.size(), actual.size(), (::llc::u2_t)sizeof(T));
	for(::llc::u2_t i = 0; i < ::llc::min(expected.size(), actual.size()); ++i)
		LLC_TEST_CHECKF(errors, result, memcmp(&expected[i], &actual[i], sizeof(T)), "Element:%u, element bytes:%u.", i, (::llc::u2_t)sizeof(T));
	return 0;
}

tplt<tpnm T>
sttc ::llc::err_t engineArrayEqual(ATestError & errors, ENGINE_SERIAL_RESULT result, cnst ::llc::apod<T> & expected, cnst ::llc::apod<T> & actual) {
	return engineArrayEqual<T>(errors, result, ::llc::view<cnst T>{expected}, ::llc::view<cnst T>{actual});
}

tplt<tpnm T>
sttc ::llc::err_t engineNestedArraysEqual(ATestError & errors, ENGINE_SERIAL_RESULT result, cnst ::llc::apobj<::llc::apod<T>> & expected, cnst ::llc::apobj<::llc::apod<T>> & actual) {
	LLC_TEST_CHECKF(errors, result, expected.size() != actual.size(), "Expected buckets:%u, actual:%u.", expected.size(), actual.size());
	for(::llc::u2_t i = 0; i < ::llc::min(expected.size(), actual.size()); ++i) {
		cnst ::llc::view<cnst T> a = expected[i] ? ::llc::view<cnst T>{*expected[i]} : ::llc::view<cnst T>{};
		cnst ::llc::view<cnst T> b = actual[i] ? ::llc::view<cnst T>{*actual[i]} : ::llc::view<cnst T>{};
		llc_necs(engineArrayEqual<T>(errors, result, a, b));
	}
	return 0;
}

sttc ::llc::err_t enginePayloadEqual(ATestError & errors, cnst ::llc::SRenderBuffer & a, cnst ::llc::SRenderBuffer & b) {
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Desc.Format != b.Desc.Format || a.Desc.Usage != b.Desc.Usage || a.Desc.PitchStride != b.Desc.PitchStride || a.Desc.PitchRow != b.Desc.PitchRow || a.Desc.Frequency != b.Desc.Frequency);
	return engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Data, b.Data);
}
sttc ::llc::err_t enginePayloadEqual(ATestError & errors, cnst ::llc::SSurface & a, cnst ::llc::SSurface & b) {
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Desc.ColorType != b.Desc.ColorType || a.Desc.BitDepth != b.Desc.BitDepth || a.Desc.Dimensions != b.Desc.Dimensions || a.Desc.MethodCompression != b.Desc.MethodCompression || a.Desc.MethodFilter != b.Desc.MethodFilter || a.Desc.MethodInterlace != b.Desc.MethodInterlace);
	return engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Data, b.Data);
}
sttc ::llc::err_t enginePayloadEqual(ATestError & errors, cnst ::llc::SGeometryMesh & a, cnst ::llc::SGeometryMesh & b) {
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Desc.Type != b.Desc.Type || a.Desc.Mode != b.Desc.Mode || a.Desc.NormalMode != b.Desc.NormalMode);
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.GeometryBuffers, b.GeometryBuffers));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.ConstantBuffers, b.ConstantBuffers));
	return engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.GeometrySlices, b.GeometrySlices);
}
sttc ::llc::err_t enginePayloadEqual(ATestError & errors, cnst ::llc::SSkin & a, cnst ::llc::SSkin & b) {
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, memcmp(&a.Material, &b.Material, sizeof(a.Material)));
	return engineArrayEqual(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Textures, b.Textures);
}

tplt<tpnm TManager>
sttc ::llc::err_t engineResourcesEqual(ATestError & errors, cnst TManager & a, cnst TManager & b) {
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_RESOURCE_COUNT, a.size() != b.size() || a.Elements.size() != b.Elements.size() || b.size() != b.Elements.size());
	for(::llc::u2_t i = 0; i < a.size(); ++i) {
		LLC_TEST_REQUIREF(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, !a[i] || !b[i], "Element:%u.", i);
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_RESOURCE_VALUE, a.Names[i] != b.Names[i], "Element:%u, expected:'%.*s', actual:'%.*s'.", i, LLC_VIEW_ARGS(a.Names[i]), LLC_VIEW_ARGS(b.Names[i]));
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_OWNERSHIP, &*a[i] == &*b[i], "Element:%u.", i);
		llc_necs(enginePayloadEqual(errors, *a[i], *b[i]));
	}
	return 0;
}

tplt<tpnm TManager>
sttc ::llc::err_t testResourcePersistence(ATestError & errors) {
	TManager empty, source, loaded;
	llc_necs(::engine_test::resources(loaded));
	llc_necs(engineRoundTrip(errors, empty, loaded));
	llc_necs(engineResourcesEqual(errors, empty, loaded));
	llc_necs(::engine_test::resources(source));
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineResourcesEqual(errors, source, loaded));
	// Load again into populated storage: counts must replace rather than append.
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineResourcesEqual(errors, source, loaded));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_RESOURCE_COUNT, loaded.size() != 3 || loaded.Elements.size() != 3 || !loaded[0U]);
	::llc::au0_t original;
	llc_necs(source.Save(original));
	llc_necs(::engine_test::fill(*loaded[0U], 9));
	::llc::au0_t unchanged;
	llc_necs(source.Save(unchanged));
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_OWNERSHIP, ::llc::vcu0_t{unchanged} != ::llc::vcu0_t{original});
	::llc::au0_t sentinel;
	llc_necs(loaded.Save(sentinel));
	cnst auto * owner = &*loaded[0U];
	cnst auto * names = loaded.Names.begin();
	cnst auto * elements = loaded.Elements.begin();
	cnst ::llc::u2_t cuts[] = {0, 1, 3, 4, 5, original.size() / 2, original.size() - 1};
	for(::llc::u2_c cut : cuts) {
		::llc::vcu0_t input = {original.begin(), cut};
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_TRUNCATED, loaded.Load(input) >= 0, "Truncated length:%u, complete:%u.", cut, original.size());
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_REJECTED_CURSOR, input.begin() != original.begin() || input.size() != cut, "Truncated length:%u, remaining:%u.", cut, input.size());
		LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_REJECTED_STATE, loaded.size() != 3 || loaded.Elements.size() != 3 || !loaded[0U]);
		LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_REJECTED_STATE, &*loaded[0U] != owner || loaded.Names.begin() != names || loaded.Elements.begin() != elements);
		::llc::au0_t after;
		llc_necs(loaded.Save(after));
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_REJECTED_STATE, ::llc::vcu0_t{after} != ::llc::vcu0_t{sentinel}, "Truncated length:%u.", cut);
	}
	TManager invalid;
	llc_necs(invalid.Create("unallocated"));
	::llc::au0_t output;
	llc_necs(output.append(ENGINE_SAVE_PREFIX));
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_INVALID_SAVE, invalid.Save(output) >= 0);
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_INVALID_SAVE, ::llc::vcu0_t{output} != ::llc::vcu0_t{ENGINE_SAVE_PREFIX});
	return 0;
}

sttc ::llc::err_t engineEntitiesEqual(ATestError & errors, cnst ::llc::SVirtualEntityManager & a, cnst ::llc::SVirtualEntityManager & b) {
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_ENTITY_IDS, a.Entities, b.Entities));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_ENTITY_IDS, a.size() != b.size() || b.Entities.size() != b.size() || b.Children.size() != b.size());
	for(::llc::u2_t i = 0; i < a.size(); ++i) {
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_ENTITY_IDS, a.Names[i] != b.Names[i], "Entity:%u, expected:'%.*s', actual:'%.*s'.", i, LLC_VIEW_ARGS(a.Names[i]), LLC_VIEW_ARGS(b.Names[i]));
		llc_necs(engineArrayEqual<::llc::eid_t>(errors, ENGINE_SERIAL_RESULT_CHILDREN, a.GetChildren(i), b.GetChildren(i)));
	}
	return 0;
}

sttc ::llc::err_t engineNodesEqual(ATestError & errors, cnst ::llc::SRenderNodeManager & a, cnst ::llc::SRenderNodeManager & b) {
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_NODE_DATA, a.RenderNodes, b.RenderNodes));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_NODE_DATA, a.Flags, b.Flags));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_NODE_DATA, a.Transforms, b.Transforms));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_NODE_DATA, a.BaseTransforms, b.BaseTransforms));
	llc_necs(engineNestedArraysEqual(errors, ENGINE_SERIAL_RESULT_LIGHTS, a.Lights, b.Lights));
	llc_necs(engineNestedArraysEqual(errors, ENGINE_SERIAL_RESULT_CAMERAS, a.Cameras, b.Cameras));
	llc_necs(engineNestedArraysEqual(errors, ENGINE_SERIAL_RESULT_LIGHTS, a.LightsDirectional, b.LightsDirectional));
	llc_necs(engineNestedArraysEqual(errors, ENGINE_SERIAL_RESULT_LIGHTS, a.LightsPoint, b.LightsPoint));
	return engineNestedArraysEqual(errors, ENGINE_SERIAL_RESULT_LIGHTS, a.LightsSpot, b.LightsSpot);
}

sttc ::llc::err_t engineBodiesEqual(ATestError & errors, cnst ::llc::SRigidBodyIntegrator & a, cnst ::llc::SRigidBodyIntegrator & b) {
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.Frames, b.Frames));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.Flags, b.Flags));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.Forces, b.Forces));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.Masses, b.Masses));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.Centers, b.Centers));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_DATA, a.TransformsLocal, b.TransformsLocal));
	return engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_VOLUMES, a.BoundingVolumes, b.BoundingVolumes);
}

sttc ::llc::err_t testEntityPersistence(ATestError & errors) {
	::llc::SVirtualEntityManager empty, source, loaded;
	llc_necs(::engine_test::entities(loaded));
	llc_necs(engineRoundTrip(errors, empty, loaded));
	llc_necs(engineEntitiesEqual(errors, empty, loaded));
	llc_necs(::engine_test::entities(source));
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineEntitiesEqual(errors, source, loaded));
	llc_necs(engineRoundTrip(errors, source, loaded));
	return engineEntitiesEqual(errors, source, loaded);
}

sttc ::llc::err_t testNodePersistence(ATestError & errors) {
	::llc::SRenderNodeManager empty, source, loaded;
	llc_necs(::engine_test::nodes(loaded));
	llc_necs(engineRoundTrip(errors, empty, loaded));
	llc_necs(engineNodesEqual(errors, empty, loaded));
	llc_necs(::engine_test::nodes(source));
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineNodesEqual(errors, source, loaded));
	llc_necs(engineRoundTrip(errors, source, loaded));
	return engineNodesEqual(errors, source, loaded);
}

sttc ::llc::err_t testBodyPersistence(ATestError & errors) {
	::llc::SRigidBodyIntegrator empty, source, loaded;
	llc_necs(::engine_test::bodies(loaded));
	llc_necs(engineRoundTrip(errors, empty, loaded));
	llc_necs(engineBodiesEqual(errors, empty, loaded));
	// Fresh destination reveals omitted fields that stale destination data could conceal.
	llc_necs(loaded.Clear());
	llc_necs(::engine_test::bodies(source));
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineBodiesEqual(errors, source, loaded));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_BODY_DATA, loaded.Centers.size() != 3 || loaded.Frames.size() != 3 || loaded.Flags.size() != 3 || loaded.Forces.size() != 3 || loaded.Masses.size() != 3 || loaded.TransformsLocal.size() != 3);
	// Integrate uses the six saved arrays; do not call volume consumers if that array is missing.
	llc_necs(source.Integrate(.125));
	llc_necs(loaded.Integrate(.125));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_CONTINUATION, source.Centers, loaded.Centers));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_CONTINUATION, source.Forces, loaded.Forces));
	llc_necs(engineArrayEqual(errors, ENGINE_SERIAL_RESULT_BODY_CONTINUATION, source.Frames, loaded.Frames));
	::llc::SRigidBodyIntegrator reused;
	llc_necs(::engine_test::bodies(reused));
	reused.BoundingVolumes[0].HalfSizes = {91, 92, 93};
	llc_necs(engineRoundTrip(errors, source, reused));
	return engineBodiesEqual(errors, source, reused);
}

sttc ::llc::err_t engineGraphicsEqual(ATestError & errors, cnst ::llc::SEngineGraphics & a, cnst ::llc::SEngineGraphics & b) {
	llc_necs(engineResourcesEqual(errors, a.Buffers, b.Buffers));
	llc_necs(engineResourcesEqual(errors, a.Surfaces, b.Surfaces));
	llc_necs(engineResourcesEqual(errors, a.Meshes, b.Meshes));
	return engineResourcesEqual(errors, a.Skins, b.Skins);
}

sttc ::llc::err_t testGraphicsPersistence(ATestError & errors) {
	::llc::SEngineGraphics source, loaded;
	llc_necs(::engine_test::resources(source.Buffers));
	llc_necs(::engine_test::resources(source.Surfaces));
	llc_necs(::engine_test::resources(source.Meshes));
	llc_necs(::engine_test::resources(source.Skins));
	llc_necs(loaded.Shaders.Create("runtime"));
	if_null_fe(loaded.Shaders[0U].create());
	llc_necs(loaded.Fonts.Fonts.push_back({}));
	if_null_fe(loaded.Fonts.Fonts[0].create());
	loaded.Fonts.Fonts[0]->CharSize = {8, 16};
	cnst auto * shader = &*loaded.Shaders[0U];
	cnst auto * font = &*loaded.Fonts.Fonts[0];
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineGraphicsEqual(errors, source, loaded));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_RUNTIME_STATE, loaded.Shaders.size() != 1 || loaded.Fonts.Fonts.size() != 1 || !loaded.Shaders[0U] || !loaded.Fonts.Fonts[0]);
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_RUNTIME_STATE, &*loaded.Shaders[0U] != shader || &*loaded.Fonts.Fonts[0] != font || loaded.Shaders.Names[0] != ::llc::vcst_t{"runtime"});
	return 0;
}

sttc ::llc::err_t testCompleteEnginePersistence(ATestError & errors, ::llc::vcst_t outputFolder) {
	::llc::SEngine empty, source, loaded;
	llc_necs(engineRoundTrip(errors, empty, loaded));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, !loaded.Scene || !loaded.Scene->Graphics);
	LLC_TEST_CHECK(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, loaded.Entities.size() || loaded.Scene->RenderNodes.size() || loaded.Integrator.Centers.size());
	llc_necs(::engine_test::engine(source));
	llc_necs(engineRoundTrip(errors, source, loaded));
	llc_necs(engineEntitiesEqual(errors, source.Entities, loaded.Entities));
	llc_necs(engineNodesEqual(errors, source.Scene->RenderNodes, loaded.Scene->RenderNodes));
	llc_necs(engineBodiesEqual(errors, source.Integrator, loaded.Integrator));
	llc_necs(engineGraphicsEqual(errors, *source.Scene->Graphics, *loaded.Scene->Graphics));
	LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, loaded.Entities.size() != 4 || loaded.Scene->RenderNodes.size() != 4 || loaded.Integrator.Centers.size() != 3);
	for(::llc::u2_t i = 0; i < 3; ++i) {
		cnst auto & entity = loaded.Entities[i];
		LLC_TEST_REQUIREF(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, entity.RenderNode >= loaded.Scene->RenderNodes.size() || entity.RigidBody >= loaded.Integrator.Centers.size(), "Entity:%u.", i);
		cnst auto & node = loaded.Scene->RenderNodes[entity.RenderNode];
		LLC_TEST_REQUIREF(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, node.Mesh >= loaded.Scene->Graphics->Meshes.size() || node.Skin >= loaded.Scene->Graphics->Skins.size(), "Entity:%u, mesh:%u, skin:%u.", i, node.Mesh, node.Skin);
		cnst auto & skin = loaded.Scene->Graphics->Skins[node.Skin];
		LLC_TEST_REQUIRE(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, !skin || skin->Textures.size() != 1);
		LLC_TEST_CHECKF(errors, ENGINE_SERIAL_RESULT_ENGINE_LINKS, skin->Textures[0] >= loaded.Scene->Graphics->Surfaces.size(), "Entity:%u, texture:%u.", i, skin->Textures[0]);
	}
	::llc::au0_t saved, restored;
	llc_necs(source.Save(saved));
	llc_necs(loaded.Save(restored));
	::llc::string path;
	llc_necs(::llc::pathNameCompose(outputFolder, "engine-save.bin", path));
	llc_necs(::llc::fileFromMemory(path, saved));
	path.clear();
	llc_necs(::llc::pathNameCompose(outputFolder, "engine-loaded-save.bin", path));
	llc_necs(::llc::fileFromMemory(path, restored));
	return 0;
}

::llc::err_t testEngineSerialization(ATestError & errors, ::llc::vcst_t outputFolder) {
	always_printf("Engine resource serialization: buffers."); llc_necs(testResourcePersistence<::llc::SRenderBufferManager>(errors));
	always_printf("Engine resource serialization: surfaces."); llc_necs(testResourcePersistence<::llc::SSurfaceManager>(errors));
	always_printf("Engine resource serialization: meshes."); llc_necs(testResourcePersistence<::llc::SMeshManager>(errors));
	always_printf("Engine resource serialization: skins."); llc_necs(testResourcePersistence<::llc::SSkinManager>(errors));
	llc_necs(testEntityPersistence(errors));
	llc_necs(testNodePersistence(errors));
	llc_necs(testBodyPersistence(errors));
	llc_necs(testGraphicsPersistence(errors));
	return testCompleteEnginePersistence(errors, outputFolder);
}
