#include "llc_rigid_body.h"
#include "llc_orbit.h"

#ifndef LLC_ENGINE_ORBIT_H_23701
#define LLC_ENGINE_ORBIT_H_23701

namespace llc
{
	::llc::err_t			initOrbitRotation	(::llc::SBodyForces & orbitForces, ::llc::AXIS rotationAxis, const ::llc::quatf2_t & orbitOrientation, f3_t rotation_period, f3_t rotation_unit);
	::llc::err_t			initOrbitCenter		(::llc::SBodyCenter & orbitCenter, ::llc::AXIS rotationAxis, f3_t orbitalInclination, f3_t orbitRadius);

	stin ::llc::err_t	initOrbit			(::llc::SBodyCenter & orbitCenter, ::llc::SBodyForces & orbitForces, const ::llc::SOrbit & orbit) {
		initOrbitCenter(orbitCenter, orbit.RotationAxis, orbit.AxialTilt, orbit.Radius);
		return initOrbitRotation(orbitForces, orbit.RotationAxis, orbitCenter.Orientation, orbit.RotationPeriod, orbit.RotationUnit);
	}

	::llc::err_t			initOrbiterOrbit	(::llc::SBodyCenter & orbitCenter , ::llc::SBodyForces & orbitForces, f3_t orbitalInclination, f3_t orbital_period, f3_t orbital_unit);
	::llc::err_t			initOrbiterCenter	(::llc::SBodyCenter & planetCenter, f3_t orbitalInclination, f3_t orbitRadius, f3_t radiusScale);

	stin ::llc::err_t	initOrbiterBody		(::llc::SBodyCenter & planetCenter, ::llc::SBodyForces & planetForces, f3_t axialTilt, f3_t	rotation_period, f3_t	rotation_unit) {
		return initOrbiterOrbit(planetCenter, planetForces, axialTilt, rotation_period, rotation_unit);
	}
	stin	::llc::err_t	initOrbiterOrbit	(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& orbitCenter, ::llc::SBodyForces & orbitForces, f3_t rotationUnit) { 
		return ::llc::initOrbiterOrbit(orbitCenter, orbitForces, bodyData.Orbit.AxialTilt, bodyData.Orbit.RotationPeriod, rotationUnit); 
	}
	stin	::llc::err_t	initOrbiterCenter	(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& planetCenter, f3_t distanceScale) {
		return ::llc::initOrbiterCenter(planetCenter, bodyData.Orbit.AxialTilt, bodyData.Orbit.Radius, distanceScale);
	}
	stin	::llc::err_t	initOrbiterBody		(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& planetCenter, ::llc::SBodyForces & planetForces, f3_t rotationUnit) {
		return ::llc::initOrbiterBody(planetCenter, planetForces, bodyData.Body.AxialTilt, bodyData.Body.RotationPeriod, rotationUnit);
	}
	stin	::llc::err_t	initOrbiterOrbit	(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, s2_t rigidBodyIndex, f3_t rotationUnit) {
		return ::llc::initOrbiterOrbit(integrator.Centers[rigidBodyIndex], integrator.Forces[rigidBodyIndex], bodyData.Orbit.AxialTilt, bodyData.Orbit.RotationPeriod, rotationUnit); 
	}
	stin	::llc::err_t	initOrbiterCenter	(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, s2_t rigidBodyIndex, f3_t distanceScale) {
		return ::llc::initOrbiterCenter(integrator.Centers[rigidBodyIndex], bodyData.Orbit.AxialTilt, bodyData.Orbit.Radius, distanceScale);
	}
	stin	::llc::err_t	initOrbiterBody		(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, s2_t rigidBodyIndex, f3_t rotationUnit) {
		return ::llc::initOrbiterOrbit(integrator.Centers[rigidBodyIndex], integrator.Forces[rigidBodyIndex], bodyData.Body.AxialTilt, bodyData.Body.RotationPeriod, rotationUnit);
	}

	// deprecated
	::llc::err_t			createOrbiter
		( ::llc::SRigidBodyIntegrator	& bodies
		, ::llc::AXIS	orbitalInclinationAxis
		, ::llc::AXIS	obliquityToOrbitAxis
		, f3_t		orbitalInclination
		, f3_t		orbitalPeriod
		, f3_t		orbitalUnit
		, f3_t		obliquityToOrbit
		, f3_t		distance
		, f3_t		distanceScale
		, f3_t		rotationPeriod
		, f3_t		rotationUnit
		);
} // namespace

#endif // LLC_ENGINE_ORBIT_H_23701
