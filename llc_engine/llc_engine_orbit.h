#include "llc_rigid_body.h"
#include "llc_orbit.h"

#ifndef LLC_ENGINE_ORBIT_H_23701
#define LLC_ENGINE_ORBIT_H_23701

namespace llc
{
	::llc::err_t			initOrbitRotation	(::llc::SBodyForces & orbitForces, ::llc::AXIS rotationAxis, const ::llc::quatf2_t & orbitOrientation, double rotation_period, double rotation_unit);
	::llc::err_t			initOrbitCenter		(::llc::SBodyCenter & orbitCenter, ::llc::AXIS rotationAxis, double orbitalInclination, double orbitRadius);

	stin ::llc::err_t	initOrbit			(::llc::SBodyCenter & orbitCenter, ::llc::SBodyForces & orbitForces, const ::llc::SOrbit & orbit) {
		initOrbitCenter(orbitCenter, orbit.RotationAxis, orbit.AxialTilt, orbit.Radius);
		return initOrbitRotation(orbitForces, orbit.RotationAxis, orbitCenter.Orientation, orbit.RotationPeriod, orbit.RotationUnit);
	}

	::llc::err_t			initOrbiterOrbit	(::llc::SBodyCenter & orbitCenter , ::llc::SBodyForces & orbitForces, double orbitalInclination, double orbital_period, double orbital_unit);
	::llc::err_t			initOrbiterCenter	(::llc::SBodyCenter & planetCenter, double orbitalInclination, double orbitRadius, double radiusScale);

	stin ::llc::err_t	initOrbiterBody		(::llc::SBodyCenter & planetCenter, ::llc::SBodyForces & planetForces, double axialTilt, double	rotation_period, double	rotation_unit) {
		return initOrbiterOrbit(planetCenter, planetForces, axialTilt, rotation_period, rotation_unit);
	}
	stin	::llc::err_t	initOrbiterOrbit	(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& orbitCenter, ::llc::SBodyForces & orbitForces, double rotationUnit) { 
		return ::llc::initOrbiterOrbit(orbitCenter, orbitForces, bodyData.Orbit.AxialTilt, bodyData.Orbit.RotationPeriod, rotationUnit); 
	}
	stin	::llc::err_t	initOrbiterCenter	(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& planetCenter, double distanceScale) {
		return ::llc::initOrbiterCenter(planetCenter, bodyData.Orbit.AxialTilt, bodyData.Orbit.Radius, distanceScale);
	}
	stin	::llc::err_t	initOrbiterBody		(const ::llc::SOrbiter & bodyData, ::llc::SBodyCenter	& planetCenter, ::llc::SBodyForces & planetForces, double rotationUnit) {
		return ::llc::initOrbiterBody(planetCenter, planetForces, bodyData.Body.AxialTilt, bodyData.Body.RotationPeriod, rotationUnit);
	}
	stin	::llc::err_t	initOrbiterOrbit	(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, int32_t rigidBodyIndex, double rotationUnit) {
		return ::llc::initOrbiterOrbit(integrator.Centers[rigidBodyIndex], integrator.Forces[rigidBodyIndex], bodyData.Orbit.AxialTilt, bodyData.Orbit.RotationPeriod, rotationUnit); 
	}
	stin	::llc::err_t	initOrbiterCenter	(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, int32_t rigidBodyIndex, double distanceScale) {
		return ::llc::initOrbiterCenter(integrator.Centers[rigidBodyIndex], bodyData.Orbit.AxialTilt, bodyData.Orbit.Radius, distanceScale);
	}
	stin	::llc::err_t	initOrbiterBody		(const ::llc::SOrbiter & bodyData, ::llc::SRigidBodyIntegrator & integrator, int32_t rigidBodyIndex, double rotationUnit) {
		return ::llc::initOrbiterOrbit(integrator.Centers[rigidBodyIndex], integrator.Forces[rigidBodyIndex], bodyData.Body.AxialTilt, bodyData.Body.RotationPeriod, rotationUnit);
	}

	// deprecated
	::llc::err_t			createOrbiter
		( ::llc::SRigidBodyIntegrator	& bodies
		, ::llc::AXIS	orbitalInclinationAxis
		, ::llc::AXIS	obliquityToOrbitAxis
		, double		orbitalInclination
		, double		orbitalPeriod
		, double		orbitalUnit
		, double		obliquityToOrbit
		, double		distance
		, double		distanceScale
		, double		rotationPeriod
		, double		rotationUnit
		);
} // namespace

#endif // LLC_ENGINE_ORBIT_H_23701
