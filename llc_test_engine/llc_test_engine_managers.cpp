#include "llc_test_engine_fixture.h"

GDEFINE_ENUM_TYPE(ENGINE_MANAGER_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, CREATE          ,  1, "Manager creation must return the next index and keep parallel arrays aligned.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, DEFAULTS        ,  2, "New manager entries must have safe default IDs and transforms.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, CLONE_VALUE     ,  3, "Cloning must preserve the source payload and name.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, CLONE_OWNERSHIP ,  4, "Cloned resource payloads and body values must be independently writable.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, DELETE_PAIR     ,  5, "Unordered deletion must move every parallel field with the last entry.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, INVALID_INDEX   ,  6, "Resource managers must reject out-of-range clone and delete without mutation.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, PARENT          ,  7, "Parenting, reparenting and detaching must preserve both directions of the hierarchy.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, CLEAR           ,  8, "Clearing a manager must empty every owned array and permit reuse.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, NODE_REFERENCES ,  9, "Node cloning must retain mesh references and the existing shared light/camera ownership.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, BUFFER_FORMAT   , 10, "Geometry buffers must select the correct index width, format and usage at width boundaries.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, SHADER          , 11, "Shader manager cloning must retain a callable with independent replaceable ownership.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, BUILD_GRAPH     , 12, "Engine builders must create valid entity/body/node/resource links and reuse matching geometry.");
GDEFINE_ENUM_VALUED(ENGINE_MANAGER_RESULT, CLONE_GRAPH     , 13, "Engine cloning must reproduce child relationships and requested resource sharing.");

tplt<tpnm TManager>
sttc ::llc::err_t testEngineResourceManager(::llc::ATestError & errors) {
	TManager manager;
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CREATE, manager.size() || manager.Elements.size());
	llc_necs(::engine_test::resources(manager));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.size() != 3 || manager.Elements.size() != 3);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CREATE, manager.find("second") != 1);
	::llc::au0_t original;
	llc_necs(manager[0U]->Save(original));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, manager.Clone(0) != 3);
	::llc::au0_t clone;
	llc_necs(manager[3]->Save(clone));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, manager.Names[3] != manager.Names[0] || ::llc::vcu0_t{clone} != ::llc::vcu0_t{original});
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_OWNERSHIP, &*manager[0U] == &*manager[3]);
	llc_necs(::engine_test::fill(*manager[3], 7));
	::llc::au0_t unchanged;
	llc_necs(manager[0U]->Save(unchanged));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_OWNERSHIP, ::llc::vcu0_t{unchanged} != ::llc::vcu0_t{original});
	::llc::au0_t last;
	llc_necs(manager[3]->Save(last));
	llc_necs(manager.Delete(1));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.size() != 3 || manager.Elements.size() != 3);
	::llc::au0_t moved;
	llc_necs(manager[1]->Save(moved));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.Names[1] != manager.Names[0] || ::llc::vcu0_t{moved} != ::llc::vcu0_t{last});
	::llc::au0_t before, after;
	llc_necs(manager.Save(before));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_INVALID_INDEX, manager.Clone(manager.size()) >= 0);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_INVALID_INDEX, manager.Delete(manager.size()) >= 0);
	llc_necs(manager.Save(after));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_INVALID_INDEX, ::llc::vcu0_t{before} != ::llc::vcu0_t{after});
	return 0;
}

sttc ::llc::err_t testEngineEntityManager(::llc::ATestError & errors) {
	::llc::SVirtualEntityManager manager;
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Create() != 0);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DEFAULTS, manager[0].Parent != ::llc::EID_INVALID || manager[0].RenderNode != (::llc::u2_t)-1 || manager[0].RigidBody != (::llc::u2_t)-1 || manager.GetChildCount(0));
	manager.clear();
	llc_necs(::engine_test::entities(manager));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.size() != 4 || manager.Entities.size() != 4 || manager.Children.size() != 4);
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetChildCount(0) != 1 || manager.GetChildCount(2) != 1);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetIndexChild(0, 0) != 2 || manager.GetIndexChild(2, 0) != 3 || manager[3].Parent != 2);
	llc_necs(manager.SetParent(2, 1));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetChildCount(0) || manager.GetChildCount(1) != 1);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetIndexChild(1, 0) != 2 || manager[2].Parent != 1);
	llc_necs(manager.SetParent(2, 1));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetChildCount(1) != 1);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager.SetParent(2, 40) >= 0);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager[2].Parent != 1 || manager.GetChildCount(1) != 1);
	llc_necs(manager.SetParent(2, ::llc::EID_INVALID));
	llc_necs(manager.SetParent(3, ::llc::EID_INVALID));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_PARENT, manager.GetChildCount(1) || manager.GetChildCount(2) || manager[2].Parent != ::llc::EID_INVALID);
	// Delete isolated entries: hierarchy-ID repair is not provided by this low-level API.
	llc_necs(manager.Delete(1));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.size() != 3 || manager.Entities.size() != 3 || manager.Children.size() != 3);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.Names[1] != ::llc::vcst_t{"grandchild"} || manager[1].RenderNode != 13 || manager[1].RigidBody != 23 || manager.GetChildCount(1));
	manager.clear();
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLEAR, manager.size() || manager.Entities.size() || manager.Children.size());
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLEAR, manager.Create() != 0);
	return 0;
}

sttc ::llc::err_t testEngineNodeManager(::llc::ATestError & errors) {
	::llc::SRenderNodeManager manager;
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Create() != 0);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DEFAULTS, manager[0].Mesh != (::llc::u2_t)-1 || manager[0].Skin != (::llc::u2_t)-1 || manager[0].Shader != (::llc::u2_t)-1 || manager.Lights[0] || manager.Cameras[0]);
	llc_necs(manager.Delete(0));
	llc_necs(::engine_test::nodes(manager));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, manager.Clone(0) != 3);
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.size() != 4 || manager.Flags.size() != 4 || manager.Transforms.size() != 4 || manager.BaseTransforms.size() != 4 || manager.Lights.size() != 4 || manager.Cameras.size() != 4);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager[0], &manager[3], sizeof(::llc::SRenderNode)) || memcmp(&manager.Transforms[0], &manager.Transforms[3], sizeof(::llc::SRenderNodeConstants)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Flags[0], &manager.Flags[3], sizeof(::llc::SRenderNodeFlags)) || memcmp(&manager.BaseTransforms[0], &manager.BaseTransforms[3], sizeof(::llc::SRenderNodeConstants)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_NODE_REFERENCES, &*manager.Lights[0] != &*manager.Lights[3] || &*manager.Cameras[0] != &*manager.Cameras[3]);
	manager.Transforms[3].NodeSize.x = 42;
	manager.Flags[3].NoDraw = true;
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_OWNERSHIP, manager.Transforms[0].NodeSize.x == 42 || manager.Flags[0].NoDraw);
	llc_necs(manager.Delete(1));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.size() != 3 || manager.Flags.size() != 3 || manager.Transforms.size() != 3 || manager.BaseTransforms.size() != 3 || manager.Lights.size() != 3 || manager.Cameras.size() != 3);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager[1].Mesh != manager[0].Mesh || manager.Transforms[1].NodeSize.x != 42 || !manager.Flags[1].NoDraw || &*manager.Lights[1] != &*manager.Lights[0] || &*manager.Cameras[1] != &*manager.Cameras[0]);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.BaseTransforms[0], &manager.BaseTransforms[1], sizeof(::llc::SRenderNodeConstants)));
	return 0;
}

sttc ::llc::err_t testEngineBodyManager(::llc::ATestError & errors) {
	::llc::SRigidBodyIntegrator manager;
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Create(0) != 0 || manager.Centers.size());
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Create(2) != 0);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DEFAULTS, (manager.Centers[1].Orientation != ::llc::quatf2_t{0, 0, 0, 1} || manager.BoundingVolumes[1].HalfSizes != ::llc::n3f2_t{.5f, .5f, .5f}));
	llc_necs(manager.Clear());
	llc_necs(::engine_test::bodies(manager));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, manager.Clone(0) != 3);
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Centers.size() != 4 || manager.Frames.size() != 4 || manager.Flags.size() != 4 || manager.Forces.size() != 4 || manager.Masses.size() != 4 || manager.BoundingVolumes.size() != 4 || manager.TransformsLocal.size() != 4);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Centers[0], &manager.Centers[3], sizeof(::llc::SBodyCenter)) || manager.BoundingVolumes[0].HalfSizes != manager.BoundingVolumes[3].HalfSizes);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Frames[0], &manager.Frames[3], sizeof(::llc::SBodyFrame)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Flags[0], &manager.Flags[3], sizeof(::llc::SBodyFlags)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Forces[0], &manager.Forces[3], sizeof(::llc::SBodyForces)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.Masses[0], &manager.Masses[3], sizeof(::llc::SBodyMass)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, memcmp(&manager.TransformsLocal[0], &manager.TransformsLocal[3], sizeof(::llc::m4f2_t)));
	manager.Centers[3].Position.x = 42;
	manager.BoundingVolumes[3].HalfSizes.x = 43;
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_OWNERSHIP, manager.Centers[0].Position.x == 42 || manager.BoundingVolumes[0].HalfSizes.x == 43);
	llc_necs(manager.Delete(1));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.Centers.size() != 3 || manager.Frames.size() != 3 || manager.Flags.size() != 3 || manager.Forces.size() != 3 || manager.Masses.size() != 3 || manager.BoundingVolumes.size() != 3 || manager.TransformsLocal.size() != 3);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.Centers[1].Position.x != 42 || manager.BoundingVolumes[1].HalfSizes.x != 43);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.Frames[0], &manager.Frames[1], sizeof(::llc::SBodyFrame)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.Flags[0], &manager.Flags[1], sizeof(::llc::SBodyFlags)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.Forces[0], &manager.Forces[1], sizeof(::llc::SBodyForces)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.Masses[0], &manager.Masses[1], sizeof(::llc::SBodyMass)));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, memcmp(&manager.TransformsLocal[0], &manager.TransformsLocal[1], sizeof(::llc::m4f2_t)));
	llc_necs(manager.Clear());
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLEAR, manager.Centers.size() || manager.Frames.size() || manager.Flags.size() || manager.Forces.size() || manager.Masses.size() || manager.BoundingVolumes.size() || manager.TransformsLocal.size());
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLEAR, manager.Create() != 0);
	return 0;
}

sttc ::llc::err_t testEngineBufferFormats(::llc::ATestError & errors) {
	cnst ::llc::u2_t counts[] = {0, 1, 255, 256, 65535, 65536};
	cnst ::llc::u2_t widths[] = {1, 1, 1, 2, 2, 4};
	::llc::pobj<::llc::SRenderBuffer> indices, positions, normals, uv;
	for(::llc::u2_t i = 0; i < ::llc::size(counts); ++i) {
		llc_necs(::llc::createBuffers(counts[i], indices, positions, normals, uv));
		LLC_TEST_REQUIREF(errors, ENGINE_MANAGER_RESULT_BUFFER_FORMAT, !indices || !positions || !normals || !uv, "Vertex count:%u.", counts[i]);
		LLC_TEST_CHECKF(errors, ENGINE_MANAGER_RESULT_BUFFER_FORMAT, indices->Desc.Format.TotalBytes() != widths[i] || indices->Desc.Usage != ::llc::BUFFER_USAGE_Index, "Vertex count:%u, index bytes:%u, expected:%u.", counts[i], (::llc::u2_t)indices->Desc.Format.TotalBytes(), widths[i]);
		LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_BUFFER_FORMAT, positions->Desc.Format != ::llc::DATA_TYPE_FLOAT32_3 || normals->Desc.Format != ::llc::DATA_TYPE_FLOAT32_3 || uv->Desc.Format != ::llc::DATA_TYPE_FLOAT32_2);
		LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_BUFFER_FORMAT, positions->Desc.Usage != ::llc::BUFFER_USAGE_Position || normals->Desc.Usage != ::llc::BUFFER_USAGE_Normal || uv->Desc.Usage != ::llc::BUFFER_USAGE_UV);
	}
	return 0;
}

sttc ::llc::err_t testEngineShaderManager(::llc::ATestError & errors) {
	::llc::SShaderManager manager;
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CREATE, manager.Create("shader") != 0);
	if_null_fe(manager[0U].create([](cnst ::llc::SEngineSceneConstants &, cnst ::llc::SPSIn &, ::llc::bgra & output) -> ::llc::err_t { output = {1, 2, 3, 255}; return 0; }));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_VALUE, manager.Clone(0) != 1);
	::llc::SRenderNodeConstants constants = {};
	::llc::SPSIn input = {constants};
	::llc::bgra source = {}, clone = {};
	llc_necs((*manager[0U])({}, input, source));
	llc_necs((*manager[1])({}, input, clone));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_SHADER, source != clone || manager.Names[0] != manager.Names[1]);
	if_null_fe(manager[1].create([](cnst ::llc::SEngineSceneConstants &, cnst ::llc::SPSIn &, ::llc::bgra & output) -> ::llc::err_t { output = {4, 5, 6, 255}; return 0; }));
	llc_necs((*manager[0U])({}, input, source));
	llc_necs((*manager[1])({}, input, clone));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_SHADER, source.b != 1 || source.g != 2 || source.r != 3 || clone.b != 4 || clone.g != 5 || clone.r != 6);
	llc_necs(manager.Delete(0));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_DELETE_PAIR, manager.size() != 1 || manager.Elements.size() != 1);
	llc_necs((*manager[0U])({}, input, clone));
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_SHADER, clone.b != 4 || manager.Names[0] != ::llc::vcst_t{"shader"});
	return 0;
}

sttc ::llc::err_t testEngineGraph(::llc::ATestError & errors) {
	::llc::SEngine engine;
	llc_necs(::engine_test::engine(engine));
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_BUILD_GRAPH, engine.Entities.size() != 4 || engine.Integrator.Centers.size() != 3 || engine.Scene->RenderNodes.size() != 4 || engine.Scene->Graphics->Meshes.size() != 1);
	for(::llc::u2_t i = 0; i < 3; ++i) {
		LLC_TEST_CHECKF(errors, ENGINE_MANAGER_RESULT_BUILD_GRAPH, engine.GetRigidBody(i) != (::llc::err_t)i || engine.GetRenderNode(i) != (::llc::err_t)i || engine.Scene->RenderNodes[i].Mesh != 0 || engine.Scene->RenderNodes[i].Skin != i, "Entity:%u.", i);
	}
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, engine.Clone(0, true, true, false) != 4);
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, engine.Entities.size() != 6 || engine.GetChildCount(4) != 1);
	cnst ::llc::eid_t child = engine.GetChildren(4)[0];
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, child >= engine.Entities.size());
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, child == 2 || engine.GetParentIndex(child) != 4 || engine.GetParentIndex(2) != 0 || engine.GetChildCount(0) != 1);
	cnst auto & sourceNode = engine.Scene->RenderNodes[engine.GetRenderNode(0)];
	cnst auto & clonedNode = engine.Scene->RenderNodes[engine.GetRenderNode(4)];
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, clonedNode.Skin >= engine.Scene->Graphics->Skins.size());
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, sourceNode.Mesh != clonedNode.Mesh || sourceNode.Skin == clonedNode.Skin || engine.GetRigidBody(0) == engine.GetRigidBody(4));
	cnst auto & skins = engine.Scene->Graphics->Skins;
	LLC_TEST_REQUIRE(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, !skins[sourceNode.Skin] || !skins[clonedNode.Skin] || skins[sourceNode.Skin]->Textures.size() != 1 || skins[clonedNode.Skin]->Textures.size() != 1);
	LLC_TEST_CHECK(errors, ENGINE_MANAGER_RESULT_CLONE_GRAPH, skins[sourceNode.Skin]->Textures[0] == skins[clonedNode.Skin]->Textures[0]);
	return 0;
}

::llc::err_t testEngineManagers(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	always_printf("Engine resource operations: buffers."); llc_necs(testEngineResourceManager<::llc::SRenderBufferManager>(errors));
	always_printf("Engine resource operations: surfaces."); llc_necs(testEngineResourceManager<::llc::SSurfaceManager>(errors));
	always_printf("Engine resource operations: meshes."); llc_necs(testEngineResourceManager<::llc::SMeshManager>(errors));
	always_printf("Engine resource operations: skins."); llc_necs(testEngineResourceManager<::llc::SSkinManager>(errors));
	llc_necs(testEngineEntityManager(errors));
	llc_necs(testEngineNodeManager(errors));
	llc_necs(testEngineBodyManager(errors));
	llc_necs(testEngineBufferFormats(errors));
	llc_necs(testEngineShaderManager(errors));
	return testEngineGraph(errors);
}
