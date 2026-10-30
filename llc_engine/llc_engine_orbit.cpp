#include "llc_engine_orbit.h"

LLC_USING_APOD();
LLC_USING_VIEW();
LLC_USING_TYPEINT();
LLC_USING_N2();
LLC_USING_N3();

::llc::err_t			llc::initOrbitRotation	(::llc::SBodyForces & orbitForces, ::llc::AXIS rotationAxis, const ::llc::quatf2_t & orbitOrientation, f3_t rotation_period, f3_t rotation_unit) {
	orbitForces				= {};	
	::llc::orbitRotation(rotationAxis, rotation_period, rotation_unit, orbitOrientation, orbitForces.Rotation);
	return 0;
}

::llc::err_t			llc::initOrbitCenter	(::llc::SBodyCenter & orbitCenter, ::llc::AXIS rotationAxis, f3_t orbitalInclination, f3_t orbitRadius) {
	orbitCenter				= {};
	::llc::orientationFromEuler(rotationAxis, orbitalInclination, orbitCenter.Orientation);
	switch(rotationAxis) {
	case ::llc::AXIS_X_NEGATIVE:
	case ::llc::AXIS_X_POSITIVE: orbitCenter.Position.x	= f2_t(orbitRadius); break;
	case ::llc::AXIS_Y_NEGATIVE:
	case ::llc::AXIS_Y_POSITIVE: orbitCenter.Position.y	= f2_t(orbitRadius); break;
	case ::llc::AXIS_Z_NEGATIVE:
	case ::llc::AXIS_Z_POSITIVE: orbitCenter.Position.z	= f2_t(orbitRadius); break;
	}
	return 0;
}

::llc::err_t			llc::initOrbiterOrbit	(::llc::SBodyCenter & orbitCenter, ::llc::SBodyForces & orbitForces, f3_t orbitalInclination, f3_t rotationPeriod, f3_t rotationUnit) {
	orbitCenter				= {};
	orbitForces				= {};	
	::llc::orientationFromEuler	(::llc::AXIS_X_POSITIVE, orbitalInclination, orbitCenter.Orientation);
	::llc::orbitRotation		(::llc::AXIS_Y_NEGATIVE, rotationPeriod, rotationUnit, orbitCenter.Orientation, orbitForces.Rotation);
	return 0;
}

::llc::err_t			llc::initOrbiterCenter	(::llc::SBodyCenter & planetCenter, f3_t orbitalInclination, f3_t orbitRadius, f3_t radiusScale) {
	::llc::initOrbitCenter(planetCenter, ::llc::AXIS_X_POSITIVE, orbitalInclination, orbitRadius * radiusScale);
	planetCenter			= {};
	planetCenter.Position.x	= f2_t(radiusScale * orbitRadius);
	return 0;
}

static	::llc::err_t	initOrbiterBodyWithGravityCenter
	( ::llc::SBodyCenter	& planetCenter
	, ::llc::SBodyForces	& planetForces		
	, ::llc::AXIS			orbitAxis
	, ::llc::AXIS			rotationAxis
	, f3_t				axialTilt
	, f3_t				distance
	, f3_t				distance_scale
	, f3_t				rotation_period
	, f3_t				rotation_unit
	) {
	::llc::initOrbitCenter(planetCenter, orbitAxis, axialTilt, distance * distance_scale);
	::llc::orbitRotation(rotationAxis, rotation_period, rotation_unit, planetCenter.Orientation, planetForces.Rotation);
	return 0;
}

::llc::err_t			llc::createOrbiter
	( ::llc::SRigidBodyIntegrator	& bodies
	, ::llc::AXIS					orbitalInclinationAxis
	, ::llc::AXIS					obliquityToOrbitAxis
	, f3_t						orbital_inclination
	, f3_t						orbital_period
	, f3_t						orbital_unit
	, f3_t						axialTilt
	, f3_t						distance
	, f3_t						distance_scale
	, f3_t						rotation_period
	, f3_t						rotation_unit
	) {
	const ::llc::err_t		indexFirstBody				= bodies.Create(2);
	::llc::initOrbiterOrbit(bodies.Centers[indexFirstBody] = {}, bodies.Forces[indexFirstBody] = {}, orbital_inclination, orbital_period, orbital_unit);
	::initOrbiterBodyWithGravityCenter
		( bodies.Centers[indexFirstBody + 1] = {}
		, bodies.Forces	[indexFirstBody + 1] = {}
		, orbitalInclinationAxis
		, obliquityToOrbitAxis
		, axialTilt
		, distance
		, distance_scale
		, rotation_period
		, rotation_unit
		);
	return indexFirstBody;
}
