#include "llc_engine.h"
#include "llc_geometry_lh.h"
#include "llc_test_core.h"

#ifndef LLC_TEST_ENGINE_FIXTURE_H
#define LLC_TEST_ENGINE_FIXTURE_H

namespace engine_test {

	inln ::llc::err_t fill(::llc::SRenderBuffer & value, ::llc::u2_c index) {
		value.Desc.Format = ::llc::DATA_TYPE_FLOAT32_3;
		value.Desc.Usage = ::llc::BUFFER_USAGE_Position;
		value.Desc.PitchStride = 12;
		value.Desc.PitchRow = 36;
		value.Desc.Frequency = 1;
		llc_necs(value.Data.resize(36));
		for(::llc::u2_t i = 0; i < value.Data.size(); ++i)
			value.Data[i] = (::llc::u0_t)(i + index * 17);
		return 0;
	}

	inln ::llc::err_t fill(::llc::SSurface & value, ::llc::u2_c index) {
		value.Desc.ColorType = ::llc::COLOR_TYPE_BGRA;
		value.Desc.BitDepth = 8;
		value.Desc.Dimensions = {3, 2};
		llc_necs(value.Data.resize(24));
		for(::llc::u2_t i = 0; i < value.Data.size(); ++i)
			value.Data[i] = (::llc::u0_t)(255 - i - index * 17);
		return 0;
	}

	inln ::llc::err_t fill(::llc::SGeometryMesh & value, ::llc::u2_c index) {
		value.Desc = {::llc::GEOMETRY_TYPE_Triangle, ::llc::MESH_MODE_List, ::llc::NORMAL_MODE_Point};
		value.GeometryBuffers.clear();
		value.ConstantBuffers.clear();
		value.GeometrySlices.clear();
		llc_necs(value.GeometryBuffers.append({index, index + 1, index + 2, index + 3}));
		llc_necs(value.ConstantBuffers.append({index + 4, index + 5}));
		llc_necs(value.GeometrySlices.push_back({{index, 3}}));
		return value.GeometrySlices.push_back({{index + 3, 6}});
	}

	inln ::llc::err_t fill(::llc::SSkin & value, ::llc::u2_c index) {
		value.Material.Color.Diffuse = {.25f, .5f, .75f, 1};
		value.Material.Color.Ambient = {.125f, .25f, .375f, .5f};
		value.Material.Color.Specular = {1, .75f, .5f, .25f};
		value.Material.Emission = {.5f, 1, 2};
		value.Material.SpecularPower = 16 + (::llc::f2_t)index;
		value.Textures.clear();
		return value.Textures.append({index, index + 1});
	}

	tplt<tpnm TManager>
	::llc::err_t resources(TManager & manager) {
		cnst ::llc::vcst_t names[] = {"first", "second", "third"};
		for(::llc::u2_t i = 0; i < 3; ++i) {
			::llc::err_t id;
			llc_necs(id = manager.Create(names[i]));
			if_null_fe(manager[id].create());
			llc_necs(fill(*manager[id], i));
		}
		return 0;
	}

	inln ::llc::err_t entities(::llc::SVirtualEntityManager & manager) {
		cnst ::llc::vcst_t names[] = {"root", "spacer", "child", "grandchild"};
		for(::llc::u2_t i = 0; i < 4; ++i) {
			::llc::err_t id;
			llc_necs(id = manager.Create());
			manager.Names[id] = names[i];
			manager[id].RenderNode = i + 10;
			manager[id].RigidBody = i + 20;
		}
		llc_necs(manager.SetParent(2, 0));
		return manager.SetParent(3, 2);
	}

	inln ::llc::err_t nodes(::llc::SRenderNodeManager & manager) {
		for(::llc::u2_t i = 0; i < 3; ++i) {
			::llc::err_t id;
			llc_necs(id = manager.Create());
			manager[id] = {i + 1, i + 2, i + 3, i + 4};
			manager.Flags[id].NoDraw = (i == 1);
			manager.Flags[id].NoSpecular = (i == 2);
			manager.Transforms[id].Model.SetTranslation({(::llc::f2_t)i, 2, 3}, false);
			manager.BaseTransforms[id].Model.Scale({2, 3, 4}, true);
			manager.BaseTransforms[id].NodeSize = {4, 6, 8};
		}
		llc_necs(manager.Lights[0]->push_back({::llc::LIGHT_TYPE_Directional, 0}));
		llc_necs(manager.Lights[0]->push_back({::llc::LIGHT_TYPE_Point, 0}));
		llc_necs(manager.Lights[2]->push_back({::llc::LIGHT_TYPE_Spot, 0}));
		::llc::SCamera camera = {};
		camera.Position = {5, 6, 7};
		camera.NearFar = {.25f, 512};
		camera.Zoom = 2;
		llc_necs(manager.Cameras[0]->push_back(camera));
		camera.Position = {8, 9, 10};
		llc_necs(manager.Cameras[0]->push_back(camera));
		llc_necs(manager.LightsDirectional.resize(2));
		llc_necs(manager.LightsPoint.resize(2));
		llc_necs(manager.LightsSpot.resize(2));
		::llc::SLightDirectional directional = {};
		directional.Direction = {0, -1, 0};
		directional.Color.Diffuse = {1, .5f, .25f, 1};
		::llc::SLightPoint point = {};
		point.Range = 32;
		point.Color.Diffuse = {.25f, .5f, 1, 1};
		::llc::SLightSpot spot = {};
		spot.Direction = {1, 0, 0};
		spot.Range = 64;
		spot.SpotPower = 8;
		llc_necs(manager.LightsDirectional[0]->push_back(directional));
		llc_necs(manager.LightsPoint[0]->push_back(point));
		return manager.LightsSpot[0]->push_back(spot);
	}

	inln ::llc::err_t bodies(::llc::SRigidBodyIntegrator & manager) {
		for(::llc::u2_t i = 0; i < 3; ++i) {
			::llc::err_t id;
			llc_necs(id = manager.Create());
			manager.Flags[id].Active = true;
			manager.Flags[id].Collides = true;
			manager.Masses[id].InverseMass = .5f;
			manager.Masses[id].LinearDamping = .875f;
			manager.Masses[id].AngularDamping = .75f;
			manager.Centers[id].Position = {1 + (::llc::f2_t)i, 2, 3};
			manager.Forces[id].Velocity = {2, 1, .5f};
			manager.Forces[id].Acceleration = {0, -.25f, 0};
			manager.Forces[id].Rotation = {0, .125f, 0};
			manager.Frames[id].AccumulatedForce = {4, 5, 6};
			manager.Frames[id].AccumulatedTorque = {1, 2, 3};
			manager.BoundingVolumes[id].HalfSizes = {2 + (::llc::f2_t)i, 3, 4};
			::llc::m4f2_t transform;
			llc_necs(manager.GetTransform(id, transform));
		}
		return 0;
	}

	// Avoid the default skin builder's global rand() calls; all fixture values are explicit.
	inln ::llc::err_t engine(::llc::SEngine & value) {
		::llc::SParamsBox params = {};
		for(::llc::u2_t i = 0; i < 3; ++i) {
			llc_necs(value.CreateEntityFromGeometry("box", params.HalfSizes, false, params, value.ParamsBox
				, [params](::llc::SGeometryBuffers & geometry) { return ::llc::geometryBuildBox(geometry, params); }));
			llc_necs(value.SetPosition(i, {1 + (::llc::f2_t)i, 2, 3}));
			llc_necs(value.SetHalfSizes(i, {2 + (::llc::f2_t)i, 3, 4}));
			llc_necs(value.SetVelocity(i, {1, .5f, .25f}));
			llc_necs(value.SetPhysicsActive(i, true));
		}
		llc_necs(value.SetParent(2, 0));
		auto & graphics = *value.Scene->Graphics;
		llc_necs(resources(graphics.Surfaces));
		llc_necs(resources(graphics.Skins));
		for(::llc::u2_t i = 0; i < value.Entities.size(); ++i) {
			graphics.Skins[i]->Textures.clear();
			llc_necs(graphics.Skins[i]->Textures.push_back(i));
			value.Scene->RenderNodes[value.GetRenderNode(i)].Skin = i;
		}
		llc_necs(value.Scene->RenderNodes.Cameras[0]->push_back({}));
		llc_necs(value.CreateLight(::llc::LIGHT_TYPE_Point));
		::llc::SLightPoint light = {};
		light.Range = 20;
		llc_necs(value.Scene->RenderNodes.LightsPoint[0]->push_back(light));
		return value.Update(0);
	}
}

#endif // LLC_TEST_ENGINE_FIXTURE_H
