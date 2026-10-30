#include "llc_test_engine_fixture.h"
#include "llc_file.h"
#include "llc_ro_model.h"
#include "llc_view_serialize.h"

GDEFINE_ENUM_TYPE(RO_RSM_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, LOAD             , 1, "A complete RSM 1.1 through 1.5 stream must load.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, HEADER           , 2, "RSM header fields must preserve their wire values and version defaults.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, TEXTURES         , 3, "RSM texture and main-node names must load into owned storage.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, NODE             , 4, "RSM node transforms and geometry fields must preserve their wire values.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VERSION_FIELDS   , 5, "RSM version-gated fields must load when present and retain defaults when absent.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VOLUME_BOX       , 6, "RSM volume boxes must preserve geometry and their version-gated flag.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, INVALID_MAGIC    , 7, "An invalid RSM signature must fail after resetting the destination.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, TRUNCATED        , 8, "A truncated RSM stream must fail.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, PARTIAL_STATE    , 9, "A failed RSM load must retain the successfully decoded prefix.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VIEW_LOAD        , 11, "The RSM view parser must accept the same versioned fixtures.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VIEW_ALIAS       , 12, "RSM section views must alias the supplied memory or owned file bytes.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VIEW_FIELDS      , 13, "Typed RSM section views must preserve record counts and wire values.");
GDEFINE_ENUM_VALUED(RO_RSM_RESULT, VIEW_RELOAD      , 14, "Reloading a view must replace source spans while reusing node descriptors.");

sttc bool rsmVersionAtLeast(::llc::u0_t minor, ::llc::u0_t requiredMinor) { return minor >= requiredMinor; }

sttc ::llc::err_t rsmSaveName(::llc::au0_t & output, ::llc::vcst_t text) {
	::llc::astsc_t<40> value = {};
	if_true_fef(text.size() > value.size(), "RSM fixture name is too long: %u.", text.size());
	if(text.size())
		memcpy(value.begin(), text.begin(), text.size());
	return ::llc::savePOD(output, value);
}

sttc ::llc::err_t rsmFixture(::llc::au0_t & output, ::llc::u0_t minor) {
	output.clear();
	if_fail_fe(output.append((cnst ::llc::u0_t*)"GRSM", 4));
	if_fail_fe(::llc::savePOD(output, (::llc::u0_t)1));
	if_fail_fe(::llc::savePOD(output, minor));
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)120));
	if_fail_fe(::llc::savePOD(output, (::llc::s2_t)2));
	if(::rsmVersionAtLeast(minor, 4))
		if_fail_fe(::llc::savePOD(output, (::llc::u0_t)0x7F));
	::llc::astatic<::llc::u0_t, 16> unknown = {};
	for(::llc::u2_t i = 0; i < unknown.size(); ++i)
		unknown[i] = (::llc::u0_t)(0x80 + i);
	if_fail_fe(::llc::savePOD(output, unknown));

	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::rsmSaveName(output, "texture.bmp"));
	if_fail_fe(::rsmSaveName(output, "root"));
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::rsmSaveName(output, "root"));
	if_fail_fe(::rsmSaveName(output, "parent"));

	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)0));
	::llc::m3a3f2_t transform = {};
	for(::llc::u2_t i = 0; i < ::llc::size(transform.Value); ++i)
		transform.Value[i] = (::llc::f2_t)(i + 1);
	if_fail_fe(::llc::savePOD(output, transform));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{10, 11, 12}));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{13, 14, 15}));
	if_fail_fe(::llc::savePOD(output, (::llc::f2_t)16));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{17, 18, 19}));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{20, 21, 22}));

	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{23, 24, 25}));
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if(::rsmVersionAtLeast(minor, 2))
		if_fail_fe(::llc::savePOD(output, (::llc::u2_t)0x11223344));
	if_fail_fe(::llc::savePOD(output, ::llc::n2f2_t{.25f, .75f}));

	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	::llc::astatic<::llc::u1_t, 3> vertexIndices = {}, attributeIndices = {};
	vertexIndices[0] = 1; vertexIndices[1] = 2; vertexIndices[2] = 3;
	attributeIndices[0] = 4; attributeIndices[1] = 5; attributeIndices[2] = 6;
	if_fail_fe(::llc::savePOD(output, vertexIndices));
	if_fail_fe(::llc::savePOD(output, attributeIndices));
	if_fail_fe(::llc::savePOD(output, (::llc::u1_t)7));
	if_fail_fe(::llc::savePOD(output, (::llc::u1_t)8));
	if_fail_fe(::llc::savePOD(output, (::llc::s2_t)1));
	if(::rsmVersionAtLeast(minor, 2))
		if_fail_fe(::llc::savePOD(output, (::llc::s2_t)0x10203040));

	if(::rsmVersionAtLeast(minor, 5)) {
		if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
		if_fail_fe(::llc::savePOD(output, (::llc::u2_t)30));
		if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{31, 32, 33}));
	}
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)40));
	if_fail_fe(::llc::savePOD(output, ::llc::quatf2_t{.1f, .2f, .3f, .4f}));

	if_fail_fe(::llc::savePOD(output, (::llc::u2_t)1));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{41, 42, 43}));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{44, 45, 46}));
	if_fail_fe(::llc::savePOD(output, ::llc::n3f2_t{47, 48, 49}));
	if(::rsmVersionAtLeast(minor, 3))
		if_fail_fe(::llc::savePOD(output, (::llc::u2_t)0x55667788));
	return 0;
}

sttc ::llc::err_t rsmVersionTest(::llc::ATestError & errors, ::llc::u0_t minor) {
	::llc::au0_t bytes;
	if_fail_fe(::rsmFixture(bytes, minor));
	::llc::SRORSM loaded;
	cnst ::llc::err_t result = ::llc::roRSMLoad(loaded, bytes);
	LLC_TEST_REQUIREF(errors, RO_RSM_RESULT_LOAD, result < 0, "Version:1.%u, result:%i, bytes:%u.", minor, result, bytes.size());
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_HEADER, loaded.VersionMajor != 1 || loaded.VersionMinor != minor || loaded.FrameCount != 120 || loaded.ShadeType != 2, "Version:1.%u.", minor);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_HEADER, loaded.Alpha != (::rsmVersionAtLeast(minor, 4) ? 0x7F : 0xFF), "Version:1.%u, alpha:%u.", minor, loaded.Alpha);
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_TEXTURES, loaded.Textures.size() != 1 || loaded.Nodes.size() != 1 || loaded.VolumeBoxes.size() != 1);
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_TEXTURES, 0 != memcmp(loaded.Textures[0].begin(), "texture.bmp", 11) || 0 != memcmp(loaded.MainNode.begin(), "root", 4));

	cnst ::llc::SRORSMNode & node = loaded.Nodes[0];
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_NODE, node.Textures.size() != 1 || node.Vertices.size() != 1 || node.Attributes.size() != 1 || node.Triangles.size() != 1 || node.RotationKeys.size() != 1);
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_NODE, (node.Textures[0] != 0 || node.Transform.Value[0] != 1 || node.Transform.Value[8] != 9 || node.Offset != ::llc::n3f2_t{10, 11, 12} || node.Position != ::llc::n3f2_t{13, 14, 15}));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_NODE, (node.Vertices[0] != ::llc::n3f2_t{23, 24, 25} || node.Attributes[0].UV != ::llc::n2f2_t{.25f, .75f} || node.Triangles[0].TwoSided != 1 || node.RotationKeys[0].Frame != 40));
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VERSION_FIELDS, node.Attributes[0].Color != (::rsmVersionAtLeast(minor, 2) ? 0x11223344U : 0xFFFFFFFFU), "Version:1.%u, color:0x%08X.", minor, node.Attributes[0].Color);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VERSION_FIELDS, node.Triangles[0].SmoothGroup != (::rsmVersionAtLeast(minor, 2) ? 0x10203040 : 0), "Version:1.%u, smooth group:%i.", minor, node.Triangles[0].SmoothGroup);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VERSION_FIELDS, node.PositionKeys.size() != (::rsmVersionAtLeast(minor, 5) ? 1U : 0U), "Version:1.%u, position keys:%u.", minor, node.PositionKeys.size());
	if(node.PositionKeys.size())
		LLC_TEST_CHECK(errors, RO_RSM_RESULT_VERSION_FIELDS, (node.PositionKeys[0].Frame != 30 || node.PositionKeys[0].Position != ::llc::n3f2_t{31, 32, 33}));
	cnst ::llc::SRORSMVolumeBox & box = loaded.VolumeBoxes[0];
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VOLUME_BOX, (box.Size != ::llc::n3f2_t{41, 42, 43} || box.Position != ::llc::n3f2_t{44, 45, 46} || box.Rotation != ::llc::n3f2_t{47, 48, 49}));
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VERSION_FIELDS, box.Flags != (::rsmVersionAtLeast(minor, 3) ? 0x55667788U : 0U), "Version:1.%u, flags:0x%08X.", minor, box.Flags);
	return 0;
}

sttc ::llc::err_t rsmViewTest(::llc::ATestError & errors, ::llc::u0_t minor) {
	::llc::au0_t bytes;
	if_fail_fe(::rsmFixture(bytes, minor));
	::llc::SRORSMView loaded;
	cnst ::llc::err_t result = ::llc::roRSMViewLoad(bytes, loaded);
	LLC_TEST_REQUIREF(errors, RO_RSM_RESULT_VIEW_LOAD, result < 0, "Version:1.%u, result:%i, bytes:%u.", minor, result, bytes.size());
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_VIEW_FIELDS, loaded.Nodes.size() != 1 || loaded.TextureCount != 1 || loaded.VolumeBoxCount != 1);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_ALIAS, (cnst ::llc::u0_t*)loaded.Textures.begin() != bytes.begin() + 34 + (::rsmVersionAtLeast(minor, 4) ? 1 : 0), "Version:1.%u.", minor);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, loaded.VersionMajor != 1 || loaded.VersionMinor != minor || loaded.FrameCount != 120 || loaded.ShadeType != 2 || loaded.Alpha != (::rsmVersionAtLeast(minor, 4) ? 0x7F : 0xFF), "Version:1.%u.", minor);
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_FIELDS, loaded.Textures.size() != 1 || loaded.MainNode.size() != 40 || 0 != memcmp(loaded.Textures[0].Name, "texture.bmp", 11) || 0 != memcmp(loaded.MainNode.begin(), "root", 4));
	cnst ::llc::SRORSMNodeView & node = loaded.Nodes[0];
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_VIEW_FIELDS, node.TextureCount != 1 || node.Textures.size() != 1 || node.VertexCount != 1 || node.Vertices.size() != 1 || node.AttributeCount != 1 || node.TriangleCount != 1 || node.RotationKeyCount != 1 || node.RotationKeys.size() != 1);
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_ALIAS, (::llc::u3_t)node.Vertices.begin() < (::llc::u3_t)bytes.begin() || (::llc::u3_t)node.Vertices.end() > (::llc::u3_t)bytes.end(), "Version:1.%u.", minor);
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_VIEW_FIELDS, node.Attributes11.size() != (::rsmVersionAtLeast(minor, 2) ? 0U : 1U) || node.Attributes12.size() != (::rsmVersionAtLeast(minor, 2) ? 1U : 0U) || node.Triangles11.size() != (::rsmVersionAtLeast(minor, 2) ? 0U : 1U) || node.Triangles12.size() != (::rsmVersionAtLeast(minor, 2) ? 1U : 0U));
	LLC_TEST_REQUIRE(errors, RO_RSM_RESULT_VIEW_FIELDS, node.PositionKeys.size() != (::rsmVersionAtLeast(minor, 5) ? 1U : 0U) || loaded.VolumeBoxes11.size() != (::rsmVersionAtLeast(minor, 3) ? 0U : 1U) || loaded.VolumeBoxes13.size() != (::rsmVersionAtLeast(minor, 3) ? 1U : 0U));
	LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, node.Textures[0].Index != 0 || node.RotationKeys[0].Frame != 40, "Version:1.%u.", minor);
	for(cnst ::llc::SRORSMVertexWire & vertex : node.Vertices) {
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, vertex.Position.x != 23 || vertex.Position.y != 24 || vertex.Position.z != 25, "Version:1.%u, vertex:{%g,%g,%g}.", minor, (double)vertex.Position.x, (double)vertex.Position.y, (double)vertex.Position.z);
	}
	if(::rsmVersionAtLeast(minor, 2)) {
		for(cnst ::llc::SRORSMTriangle12Wire & triangle : node.Triangles12) {
			LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, triangle.VertexIndices[0] != 1 || triangle.Texture != 7 || triangle.TwoSided != 1 || triangle.SmoothGroup != 0x10203040, "Version:1.%u.", minor);
		}
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, node.Attributes12[0].Color != 0x11223344U || node.Attributes12[0].UV.x != .25f || node.Attributes12[0].UV.y != .75f, "Version:1.%u.", minor);
	}
	else {
		for(cnst ::llc::SRORSMTriangle11Wire & triangle : node.Triangles11) {
			LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, triangle.VertexIndices[0] != 1 || triangle.Texture != 7 || triangle.TwoSided != 1, "Version:1.%u.", minor);
		}
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, node.Attributes11[0].UV.x != .25f || node.Attributes11[0].UV.y != .75f, "Version:1.%u.", minor);
	}
	if(::rsmVersionAtLeast(minor, 5))
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, node.PositionKeys[0].Frame != 30 || node.PositionKeys[0].Position.x != 31, "Version:1.%u.", minor);
	if(::rsmVersionAtLeast(minor, 3))
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, loaded.VolumeBoxes13[0].Size.x != 41 || loaded.VolumeBoxes13[0].Flags != 0x55667788U, "Version:1.%u.", minor);
	else
		LLC_TEST_CHECKF(errors, RO_RSM_RESULT_VIEW_FIELDS, loaded.VolumeBoxes11[0].Size.x != 41, "Version:1.%u.", minor);
	return 0;
}

::llc::err_t testRORSM(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	for(::llc::u0_t minor = 1; minor <= 5; ++minor) {
		if_fail_fe(::rsmVersionTest(errors, minor));
		if_fail_fe(::rsmViewTest(errors, minor));
	}
	::llc::au0_t large, small;
	if_fail_fe(::rsmFixture(large, 5));
	if_fail_fe(::rsmFixture(small, 1));
	::llc::SRORSMView reused;
	if_fail_fe(::llc::roRSMViewLoad(large, reused));
	cnst ::llc::SRORSMNodeView * nodeStorage = reused.Nodes.begin();
	if_fail_fe(::llc::roRSMViewLoad(small, reused));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_RELOAD, reused.Nodes.begin() != nodeStorage || (cnst ::llc::u0_t*)reused.Textures.begin() != small.begin() + 34 || reused.VersionMinor != 1 || reused.Alpha != 0xFF || reused.Nodes.size() != 1 || reused.Nodes[0].PositionKeyCount || reused.Nodes[0].PositionKeys.size() || reused.Nodes[0].Triangles11.size() != 1 || reused.Nodes[0].Triangles12.size());
	if_fail_fe(::llc::roRSMViewLoad(small, reused));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_RELOAD, reused.Nodes.begin() != nodeStorage || reused.VersionMinor != 1 || reused.Nodes.size() != 1);

	::llc::au0_t complete;
	if_fail_fe(::rsmFixture(complete, 5));
	::llc::au0_t invalid = complete;
	invalid[0] = 'X';
	::llc::SRORSM output;
	output.VersionMajor = 9;
	if_fail_fe(output.Textures.resize(1));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_INVALID_MAGIC, 0 <= ::llc::roRSMLoad(output, invalid));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_INVALID_MAGIC, output.VersionMajor || output.Textures.size() || output.Alpha != 0xFF);
	::llc::SRORSMView invalidView;
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_LOAD, 0 <= ::llc::roRSMViewLoad(invalid, invalidView));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_FIELDS, invalidView.VersionMajor || invalidView.Nodes.size());

	output.VersionMajor = 9;
	if_fail_fe(output.Textures.resize(1));
	stxp ::llc::u2_t partialSize = 4 + szof(::llc::u0_t) * 2 + szof(::llc::u2_t);
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_TRUNCATED, 0 <= ::llc::roRSMLoad(output, {complete.begin(), partialSize}));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_PARTIAL_STATE, output.VersionMajor != 1 || output.VersionMinor != 5 || output.FrameCount != 120 || output.ShadeType || output.Textures.size() || output.Alpha != 0xFF);
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_LOAD, 0 <= ::llc::roRSMViewLoad({complete.begin(), partialSize}, invalidView));
	LLC_TEST_CHECK(errors, RO_RSM_RESULT_VIEW_FIELDS, invalidView.VersionMajor != 1 || invalidView.VersionMinor != 5 || invalidView.FrameCount != 120 || invalidView.ShadeType || invalidView.Nodes.size());

	return 0;
}
