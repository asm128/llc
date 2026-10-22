// Copyright (c) 2011 Pablo Ariel Zorrilla Cepeda
//
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// http://www.opensource.org/licenses/mit-license.php
//
// Description by: ximosoft rolaboratory (http://rolaboratory.ximosoft.com/file-format) 
// RSM : Resource Mesh File
// --------------------------------------------------------------------------------
// Versions:
// 1.1 - base version
// 1.2 - color added to texture vertices; smoothGroup added to faces
// 1.3 - flag added to volume boxes
// 1.4 - alpha added to header
// 1.5 - position animation for each node instead of global(?)
//
// the file seems to end with a uint32_t with te value 0xcccccccc
#include "llc_apod_n2.h"
#include "llc_apod_n3.h"
#include "llc_array.h"
#include "llc_array_static.h"
#include "llc_matrix3.h"

#ifndef LLC_RO_MODEL_H_23627
#define LLC_RO_MODEL_H_23627

namespace llc
{
	struct SRORSMPositionKeyframe {
		::llc::u2_t				Frame			= {};
		::llc::n3f2_t			Position		= {};
	};

	struct SRORSMRotationKeyframe {
		::llc::u2_t				Frame			= {};
		::llc::quatf2_t			Rotation		= {};
	};

	struct SRORSMTriangle {
		::llc::astatic<::llc::u1_t, 3>	VertexIndices		= {};
		::llc::astatic<::llc::u1_t, 3>	AttributeIndices	= {};
		::llc::u1_t					Texture				= {};
		::llc::u1_t					Padding				= {};
		::llc::s2_t					TwoSided			= {};
		::llc::s2_t					SmoothGroup			= {};
	};

	struct SRORSMAttribute {
		::llc::n2f2_t			UV				= {};
		::llc::u2_t				Color			= 0xFFFFFFFFU;
	};

	struct SRORSMNode {
		::llc::astchar<40>					Name				= {};
		::llc::astchar<40>					ParentName			= {};
		::llc::apod<::llc::u2_t>			Textures			= {};
		::llc::m3a3f2_t					Transform			= {};
		::llc::n3f2_t						Offset				= {};
		::llc::n3f2_t						Position			= {};
		::llc::f2_t						Rotation			= {};
		::llc::n3f2_t						RotationAxis		= {};
		::llc::n3f2_t						Scale				= {};
		::llc::an3f2_t						Vertices			= {};
		::llc::apod<::llc::SRORSMAttribute>		Attributes			= {};
		::llc::apod<::llc::SRORSMTriangle>		Triangles			= {};
		::llc::apod<::llc::SRORSMPositionKeyframe>	PositionKeys	= {};
		::llc::apod<::llc::SRORSMRotationKeyframe>	RotationKeys	= {};
	};

	struct SRORSMVolumeBox {
		::llc::n3f2_t			Size			= {};
		::llc::n3f2_t			Position		= {};
		::llc::n3f2_t			Rotation		= {};
		::llc::u2_t				Flags			= {};
	};

	struct SRORSM {
		::llc::u0_t							VersionMajor	= {};
		::llc::u0_t							VersionMinor	= {};
		::llc::u2_t							FrameCount		= {};
		::llc::s2_t							ShadeType		= {};
		::llc::u0_t							Alpha			= 0xFFU;
		::llc::astatic<::llc::u0_t, 16>		Unknown			= {};
		::llc::aobj<::llc::astchar<40>>		Textures		= {};
		::llc::astchar<40>					MainNode		= {};
		::llc::aobj<::llc::SRORSMNode>		Nodes			= {};
		::llc::apod<::llc::SRORSMVolumeBox>	VolumeBoxes		= {};
	};

	// Each byte view aliases the input supplied to roRSMViewLoad().
	struct SRORSMNodeView {
		::llc::vcu0_t	Name, ParentName, Textures;
		::llc::vcu0_t	Transform, Offset, Position, Rotation, RotationAxis, Scale;
		::llc::vcu0_t	Vertices, Attributes, Triangles, PositionKeys, RotationKeys;
		::llc::u2_t		TextureCount = 0, VertexCount = 0, AttributeCount = 0, TriangleCount = 0, PositionKeyCount = 0, RotationKeyCount = 0;
	};

	struct SRORSMView {
		::llc::u0_t			VersionMajor = 0, VersionMinor = 0, Alpha = 0xFF;
		::llc::u2_t			FrameCount = 0;
		::llc::s2_t			ShadeType = 0;
		::llc::vcu0_t			Unknown, Textures, MainNode, VolumeBoxes, Trailing;
		::llc::u2_t			TextureCount = 0, VolumeBoxCount = 0;
		::llc::aobj<SRORSMNodeView>	Nodes;
	};

	struct SRORSMFile {
		::llc::au0_t	Raw;
		SRORSMView	View;

		SRORSMFile() = default;
		SRORSMFile(const SRORSMFile &) = delete;
		SRORSMFile & operator=(const SRORSMFile &) = delete;
	};

	::llc::err_t	roRSMLoad		(::llc::SRORSM & output, ::llc::vcu0_t input);
	::llc::err_t	roRSMViewLoad	(::llc::vcu0_t input, ::llc::SRORSMView & output);
	::llc::err_t	roRSMViewLoad	(::llc::vcst_t filename, ::llc::SRORSMFile & file);
} // namespace

#endif // LLC_RO_MODEL_H_23627
