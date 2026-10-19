#include "llc_engine_orbit.h"

::llc::error_t			llc::initOrbitRotation	(::llc::SBodyForces & orbitForces, ::llc::AXIS rotationAxis, const ::llc::quatf2_t & orbitOrientation, double rotation_period, double rotation_unit) {
	orbitForces				= {};	
	::llc::orbitRotation(rotationAxis, rotation_period, rotation_unit, orbitOrientation, orbitForces.Rotation);
	return 0;
}

::llc::error_t			llc::initOrbitCenter	(::llc::SBodyCenter & orbitCenter, ::llc::AXIS rotationAxis, double orbitalInclination, double orbitRadius) {
	orbitCenter				= {};
	::llc::orientationFromEuler(rotationAxis, orbitalInclination, orbitCenter.Orientation);
	switch(rotationAxis) {
	case ::llc::AXIS_X_NEGATIVE:
	case ::llc::AXIS_X_POSITIVE: orbitCenter.Position.x	= float(orbitRadius); break;
	case ::llc::AXIS_Y_NEGATIVE:
	case ::llc::AXIS_Y_POSITIVE: orbitCenter.Position.y	= float(orbitRadius); break;
	case ::llc::AXIS_Z_NEGATIVE:
	case ::llc::AXIS_Z_POSITIVE: orbitCenter.Position.z	= float(orbitRadius); break;
	}
	return 0;
}

::llc::error_t			llc::initOrbiterOrbit	(::llc::SBodyCenter & orbitCenter, ::llc::SBodyForces & orbitForces, double orbitalInclination, double rotationPeriod, double rotationUnit) {
	orbitCenter				= {};
	orbitForces				= {};	
	::llc::orientationFromEuler	(::llc::AXIS_X_POSITIVE, orbitalInclination, orbitCenter.Orientation);
	::llc::orbitRotation		(::llc::AXIS_Y_NEGATIVE, rotationPeriod, rotationUnit, orbitCenter.Orientation, orbitForces.Rotation);
	return 0;
}

::llc::error_t			llc::initOrbiterCenter	(::llc::SBodyCenter & planetCenter, double orbitalInclination, double orbitRadius, double radiusScale) {
	::llc::initOrbitCenter(planetCenter, ::llc::AXIS_X_POSITIVE, orbitalInclination, orbitRadius * radiusScale);
	planetCenter			= {};
	planetCenter.Position.x	= float(radiusScale * orbitRadius);
	return 0;
}

static	::llc::error_t	initOrbiterBodyWithGravityCenter
	( ::llc::SBodyCenter	& planetCenter
	, ::llc::SBodyForces	& planetForces		
	, ::llc::AXIS			orbitAxis
	, ::llc::AXIS			rotationAxis
	, double				axialTilt
	, double				distance
	, double				distance_scale
	, double				rotation_period
	, double				rotation_unit
	) {
	::llc::initOrbitCenter(planetCenter, orbitAxis, axialTilt, distance * distance_scale);
	::llc::orbitRotation(rotationAxis, rotation_period, rotation_unit, planetCenter.Orientation, planetForces.Rotation);
	return 0;
}

::llc::error_t			llc::createOrbiter
	( ::llc::SRigidBodyIntegrator	& bodies
	, ::llc::AXIS					orbitalInclinationAxis
	, ::llc::AXIS					obliquityToOrbitAxis
	, double						orbital_inclination
	, double						orbital_period
	, double						orbital_unit
	, double						axialTilt
	, double						distance
	, double						distance_scale
	, double						rotation_period
	, double						rotation_unit
	) {
	const ::llc::error_t		indexFirstBody				= bodies.Create(2);
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
