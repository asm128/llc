// Copyright (c) 2011 Pablo Ariel Zorrilla Cepeda
// 
// License: http://www.opensource.org/licenses/mit-license.php
//
#include "llc_ro_model.h"

#include "llc_file.h"
#include "llc_view_serialize.h"

static	bool			roRSMVersionAtLeast		(::llc::u0_t major, ::llc::u0_t minor, ::llc::u0_t requiredMajor, ::llc::u0_t requiredMinor) {
	return major > requiredMajor || (major == requiredMajor && minor >= requiredMinor);
}

tplt<tpnm _tArray>
static	::llc::err_t	roRSMResize				(_tArray & output, ::llc::u2_t count, ::llc::u2_t bytesPerItem, ::llc::vcu0_t input, ::llc::vcst_t section) {
	if_true_fef(bytesPerItem && count > input.size() / bytesPerItem, "Invalid %.*s count: %u. Remaining bytes: %u.", LLC_VIEW_ARGS(section), count, input.size());
	if_fail_fef(output.resize(count, {}), "Cannot resize %.*s to %u elements.", LLC_VIEW_ARGS(section), count);
	return 0;
}

static	::llc::err_t	roRSMNodeLoad
	( ::llc::SRORSMNode	& output
	, ::llc::vcu0_t		& input
	, ::llc::u0_t			  versionMajor
	, ::llc::u0_t			  versionMinor
	, ::llc::u2_t			  indexNode
	) {
	if_fail_fef(::llc::loadPOD(input, output.Name		), "Cannot read RSM node %u name.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.ParentName	), "Cannot read RSM node %u parent name.", indexNode);

	::llc::u2_t				count				= {};
	if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u texture count.", indexNode);
	if_fail_fef(::roRSMResize(output.Textures, count, szof(::llc::u2_t), input, "node textures"), "Node: %u.", indexNode);
	for(::llc::u2_t iTexture = 0; iTexture < output.Textures.size(); ++iTexture)
		if_fail_fef(::llc::loadPOD(input, output.Textures[iTexture]), "Cannot read RSM node %u texture %u.", indexNode, iTexture);

	if_fail_fef(::llc::loadPOD(input, output.Transform		), "Cannot read RSM node %u transform.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.Offset			), "Cannot read RSM node %u offset.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.Position		), "Cannot read RSM node %u position.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.Rotation		), "Cannot read RSM node %u rotation.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.RotationAxis	), "Cannot read RSM node %u rotation axis.", indexNode);
	if_fail_fef(::llc::loadPOD(input, output.Scale			), "Cannot read RSM node %u scale.", indexNode);

	if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u vertex count.", indexNode);
	if_fail_fef(::roRSMResize(output.Vertices, count, szof(::llc::n3f2_t), input, "node vertices"), "Node: %u.", indexNode);
	for(::llc::u2_t iVertex = 0; iVertex < output.Vertices.size(); ++iVertex)
		if_fail_fef(::llc::loadPOD(input, output.Vertices[iVertex]), "Cannot read RSM node %u vertex %u.", indexNode, iVertex);

	if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u attribute count.", indexNode);
	::llc::u2_c				bytesPerAttribute	= ::roRSMVersionAtLeast(versionMajor, versionMinor, 1, 2) ? 12U : 8U;
	if_fail_fef(::roRSMResize(output.Attributes, count, bytesPerAttribute, input, "node attributes"), "Node: %u.", indexNode);
	for(::llc::u2_t iAttribute = 0; iAttribute < output.Attributes.size(); ++iAttribute) {
		::llc::SRORSMAttribute	& attribute			= output.Attributes[iAttribute];
		if(::roRSMVersionAtLeast(versionMajor, versionMinor, 1, 2))
			if_fail_fef(::llc::loadPOD(input, attribute.Color), "Cannot read RSM node %u attribute %u color.", indexNode, iAttribute);
		if_fail_fef(::llc::loadPOD(input, attribute.UV), "Cannot read RSM node %u attribute %u UV.", indexNode, iAttribute);
	}

	if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u triangle count.", indexNode);
	::llc::u2_c				bytesPerTriangle		= ::roRSMVersionAtLeast(versionMajor, versionMinor, 1, 2) ? 24U : 20U;
	if_fail_fef(::roRSMResize(output.Triangles, count, bytesPerTriangle, input, "node triangles"), "Node: %u.", indexNode);
	for(::llc::u2_t iTriangle = 0; iTriangle < output.Triangles.size(); ++iTriangle) {
		::llc::SRORSMTriangle	& triangle			= output.Triangles[iTriangle];
		if_fail_fef(::llc::loadPOD(input, triangle.VertexIndices	), "Cannot read RSM node %u triangle %u vertex indices.", indexNode, iTriangle);
		if_fail_fef(::llc::loadPOD(input, triangle.AttributeIndices), "Cannot read RSM node %u triangle %u attribute indices.", indexNode, iTriangle);
		if_fail_fef(::llc::loadPOD(input, triangle.Texture		), "Cannot read RSM node %u triangle %u texture.", indexNode, iTriangle);
		if_fail_fef(::llc::loadPOD(input, triangle.Padding		), "Cannot read RSM node %u triangle %u padding.", indexNode, iTriangle);
		if_fail_fef(::llc::loadPOD(input, triangle.TwoSided		), "Cannot read RSM node %u triangle %u two-sided flag.", indexNode, iTriangle);
		if(::roRSMVersionAtLeast(versionMajor, versionMinor, 1, 2))
			if_fail_fef(::llc::loadPOD(input, triangle.SmoothGroup), "Cannot read RSM node %u triangle %u smooth group.", indexNode, iTriangle);
	}

	if(::roRSMVersionAtLeast(versionMajor, versionMinor, 1, 5)) {
		if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u position-key count.", indexNode);
		if_fail_fef(::roRSMResize(output.PositionKeys, count, 16, input, "node position keys"), "Node: %u.", indexNode);
		for(::llc::u2_t iKey = 0; iKey < output.PositionKeys.size(); ++iKey) {
			if_fail_fef(::llc::loadPOD(input, output.PositionKeys[iKey].Frame	), "Cannot read RSM node %u position key %u frame.", indexNode, iKey);
			if_fail_fef(::llc::loadPOD(input, output.PositionKeys[iKey].Position), "Cannot read RSM node %u position key %u position.", indexNode, iKey);
		}
	}

	if_fail_fef(::llc::loadPOD(input, count), "Cannot read RSM node %u rotation-key count.", indexNode);
	if_fail_fef(::roRSMResize(output.RotationKeys, count, 20, input, "node rotation keys"), "Node: %u.", indexNode);
	for(::llc::u2_t iKey = 0; iKey < output.RotationKeys.size(); ++iKey) {
		if_fail_fef(::llc::loadPOD(input, output.RotationKeys[iKey].Frame	), "Cannot read RSM node %u rotation key %u frame.", indexNode, iKey);
		if_fail_fef(::llc::loadPOD(input, output.RotationKeys[iKey].Rotation), "Cannot read RSM node %u rotation key %u rotation.", indexNode, iKey);
	}
	return 0;
}

::llc::err_t			llc::roRSMLoad				(::llc::SRORSM & output, ::llc::vcu0_t input) {
	output						= {};
	::llc::astsc_t<4>		magic				= {};
	if_fail_fe(::llc::loadPOD(input, magic));
	if_true_fef(0 != memcmp(magic.begin(), "GRSM", 4), "Invalid RSM magic: '%c%c%c%c'.", magic[0], magic[1], magic[2], magic[3]);
	if_fail_fe(::llc::loadPOD(input, output.VersionMajor));
	if_fail_fe(::llc::loadPOD(input, output.VersionMinor));
	if_fail_fe(::llc::loadPOD(input, output.FrameCount));
	if_fail_fe(::llc::loadPOD(input, output.ShadeType));
	if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 4))
		if_fail_fe(::llc::loadPOD(input, output.Alpha));
	if_fail_fe(::llc::loadPOD(input, output.Unknown));

	::llc::u2_t				count				= {};
	if_fail_fe(::llc::loadPOD(input, count));
	if_fail_fe(::roRSMResize(output.Textures, count, 40, input, "textures"));
	for(::llc::u2_t iTexture = 0; iTexture < output.Textures.size(); ++iTexture)
		if_fail_fef(::llc::loadPOD(input, output.Textures[iTexture]), "Cannot read RSM texture %u.", iTexture);

	if_fail_fe(::llc::loadPOD(input, output.MainNode));
	if_fail_fe(::llc::loadPOD(input, count));
	if_fail_fe(::roRSMResize(output.Nodes, count, 180, input, "nodes"));
	for(::llc::u2_t iNode = 0; iNode < output.Nodes.size(); ++iNode)
		if_fail_fef(::roRSMNodeLoad(output.Nodes[iNode], input, output.VersionMajor, output.VersionMinor, iNode), "Cannot load RSM node %u.", iNode);

	if_fail_fe(::llc::loadPOD(input, count));
	::llc::u2_c				bytesPerVolumeBox	= ::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 3) ? 40U : 36U;
	if_fail_fe(::roRSMResize(output.VolumeBoxes, count, bytesPerVolumeBox, input, "volume boxes"));
	for(::llc::u2_t iBox = 0; iBox < output.VolumeBoxes.size(); ++iBox) {
		::llc::SRORSMVolumeBox	& box				= output.VolumeBoxes[iBox];
		if_fail_fef(::llc::loadPOD(input, box.Size		), "Cannot read RSM volume box %u size.", iBox);
		if_fail_fef(::llc::loadPOD(input, box.Position	), "Cannot read RSM volume box %u position.", iBox);
		if_fail_fef(::llc::loadPOD(input, box.Rotation	), "Cannot read RSM volume box %u rotation.", iBox);
		if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 3))
			if_fail_fef(::llc::loadPOD(input, box.Flags), "Cannot read RSM volume box %u flags.", iBox);
	}

	return 0;
}

static ::llc::err_t roRSMViewTake(::llc::vcu0_t & input, ::llc::u2_t size, ::llc::vcu0_t & output) {
	if_fail_fe(input.slice(output, 0, size));
	if_fail_fe(input.slice(input, size));
	return 0;
}

tplt<tpnm T>
static ::llc::err_t roRSMViewTakeArray(::llc::vcu0_t & input, ::llc::u2_t count, ::llc::view<cnst T> & output, ::llc::vcst_t section) {
	if_true_fef(count > input.size() / szof(T), "Invalid %.*s count: %u. Remaining bytes: %u.", LLC_VIEW_ARGS(section), count, input.size());
	output = count ? ::llc::view<cnst T>{reinterpret_cast<cnst T*>(input.begin()), count} : ::llc::view<cnst T>{};
	if_fail_fe(input.slice(input, count * szof(T)));
	return 0;
}

static ::llc::err_t roRSMViewInvalidate(::llc::SRORSMView & output) {
	output.VersionMajor = output.VersionMinor = 0;
	output.FrameCount = 0;
	output.ShadeType = 0;
	output.Alpha = 0xFF;
	output.Unknown = output.MainNode = output.Trailing = {};
	output.Textures = {};
	output.VolumeBoxes11 = {};
	output.VolumeBoxes13 = {};
	output.TextureCount = output.VolumeBoxCount = 0;
	return output.Nodes.clear();
}

static ::llc::err_t roRSMViewParse(::llc::vcu0_t input, ::llc::SRORSMView & output) {
	::llc::astsc_t<4> magic = {};
	if_fail_fe(::llc::loadPOD(input, magic));
	if_true_fef(0 != memcmp(magic.begin(), "GRSM", 4), "Invalid RSM magic: '%c%c%c%c'.", magic[0], magic[1], magic[2], magic[3]);
	if_fail_fe(::llc::loadPOD(input, output.VersionMajor));
	if_fail_fe(::llc::loadPOD(input, output.VersionMinor));
	if_fail_fe(::llc::loadPOD(input, output.FrameCount));
	if_fail_fe(::llc::loadPOD(input, output.ShadeType));
	if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 4))
		if_fail_fe(::llc::loadPOD(input, output.Alpha));
	if_fail_fe(::roRSMViewTake(input, 16, output.Unknown));

	if_fail_fe(::llc::loadPOD(input, output.TextureCount));
	if_fail_fe(::roRSMViewTakeArray(input, output.TextureCount, output.Textures, "textures"));
	if_fail_fe(::roRSMViewTake(input, 40, output.MainNode));
	::llc::u2_t nodeCount = 0;
	if_fail_fe(::llc::loadPOD(input, nodeCount));
	if_true_fef(nodeCount > input.size() / 180, "Invalid node count: %u. Remaining bytes: %u.", nodeCount, input.size());
	if_fail_fe(output.Nodes.resize(nodeCount, {}));
	for(::llc::u2_t iNode = 0; iNode < nodeCount; ++iNode) {
		::llc::SRORSMNodeView & node = output.Nodes[iNode];
		if_fail_fe(::roRSMViewTake(input, 40, node.Name));
		if_fail_fe(::roRSMViewTake(input, 40, node.ParentName));
		if_fail_fe(::llc::loadPOD(input, node.TextureCount));
		if_fail_fe(::roRSMViewTakeArray(input, node.TextureCount, node.Textures, "node textures"));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::m3a3f2_t), node.Transform));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::n3f2_t), node.Offset));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::n3f2_t), node.Position));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::f2_t), node.Rotation));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::n3f2_t), node.RotationAxis));
		if_fail_fe(::roRSMViewTake(input, szof(::llc::n3f2_t), node.Scale));
		if_fail_fe(::llc::loadPOD(input, node.VertexCount));
		if_fail_fe(::roRSMViewTakeArray(input, node.VertexCount, node.Vertices, "node vertices"));
		if_fail_fe(::llc::loadPOD(input, node.AttributeCount));
		if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 2))
			if_fail_fe(::roRSMViewTakeArray(input, node.AttributeCount, node.Attributes12, "node attributes"));
		else
			if_fail_fe(::roRSMViewTakeArray(input, node.AttributeCount, node.Attributes11, "node attributes"));
		if_fail_fe(::llc::loadPOD(input, node.TriangleCount));
		if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 2))
			if_fail_fe(::roRSMViewTakeArray(input, node.TriangleCount, node.Triangles12, "node triangles"));
		else
			if_fail_fe(::roRSMViewTakeArray(input, node.TriangleCount, node.Triangles11, "node triangles"));
		if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 5)) {
			if_fail_fe(::llc::loadPOD(input, node.PositionKeyCount));
			if_fail_fe(::roRSMViewTakeArray(input, node.PositionKeyCount, node.PositionKeys, "node position keys"));
		}
		if_fail_fe(::llc::loadPOD(input, node.RotationKeyCount));
		if_fail_fe(::roRSMViewTakeArray(input, node.RotationKeyCount, node.RotationKeys, "node rotation keys"));
	}
	if_fail_fe(::llc::loadPOD(input, output.VolumeBoxCount));
	if(::roRSMVersionAtLeast(output.VersionMajor, output.VersionMinor, 1, 3))
		if_fail_fe(::roRSMViewTakeArray(input, output.VolumeBoxCount, output.VolumeBoxes13, "volume boxes"));
	else
		if_fail_fe(::roRSMViewTakeArray(input, output.VolumeBoxCount, output.VolumeBoxes11, "volume boxes"));
	output.Trailing = input;
	return 0;
}

::llc::err_t llc::roRSMViewLoad(::llc::vcu0_t input, ::llc::SRORSMView & output) {
	if_fail_fe(::roRSMViewInvalidate(output));
	return ::roRSMViewParse(input, output);
}

::llc::err_t llc::roRSMViewLoad(::llc::vcst_t filename, ::llc::SRORSMFile & file) {
	if_fail_fe(::roRSMViewInvalidate(file.View));
	if_fail_fe(file.Raw.clear());
	if_fail_fef(::llc::fileToMemory(filename, file.Raw), "Failed to load file '%.*s'.", LLC_VIEW_ARGS(filename));
	return roRSMViewLoad(file.Raw, file.View);
}
