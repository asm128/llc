#include "llc_engine.h"

static	::llc::error_t	detectAABB				(const ::llc::SRigidBodyIntegrator & integrator, uint32_t bodyA, uint32_t bodyB, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) { (void)integrator, (void)bodyA, (void)bodyB, (void)contactCache, (void)contactsDetected; return 0; }
static	::llc::error_t	detectCylinder			(const ::llc::SRigidBodyIntegrator & integrator, uint32_t bodyA, uint32_t bodyB, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) { (void)integrator, (void)bodyA, (void)bodyB, (void)contactCache, (void)contactsDetected; return 0; }
static	::llc::error_t	detectCylinderAABB		(const ::llc::SRigidBodyIntegrator & integrator, uint32_t cylinder, uint32_t aabb, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) { (void)integrator, (void)cylinder, (void)aabb, (void)contactCache, (void)contactsDetected; return 0; }

static	::llc::error_t	detectSphere			(const ::llc::SRigidBodyIntegrator & integrator, uint32_t rigidBodyA, uint32_t rigidBodyB, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) {
	const ::llc::n3f2_t			& radiusA				= integrator.BoundingVolumes[rigidBodyA].HalfSizes;
	const ::llc::n3f2_t			& positionA				= integrator.Centers[rigidBodyA].Position;
	const ::llc::n3f2_t			& radiusB				= integrator.BoundingVolumes[rigidBodyB].HalfSizes;
	const ::llc::n3f2_t			& positionB				= integrator.Centers[rigidBodyB].Position;

	const float					maxDistance				= radiusA.x + radiusB.x;
	const float					collisionThreshold		= maxDistance * maxDistance;
	contactCache.CenterDistance	= positionB - positionA;
	const float					distanceSquared			= (float)contactCache.CenterDistance.LengthSquared();
	if(distanceSquared >= collisionThreshold) 
		return 0;

	contactCache.DistanceLength	= distanceSquared ? sqrt(distanceSquared) : 0.0f;
	llc_necs(contactsDetected.push_back(contactCache));
	return ::llc::error_t(1 + rigidBodyB);
}

static	::llc::error_t	detectSphereAABB		(const ::llc::SRigidBodyIntegrator & integrator, uint32_t bodySphere, uint32_t bodyAABB    , ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) { (void)integrator, (void)bodySphere, (void)bodyAABB, (void)contactCache, (void)contactsDetected; return 0; }
static	::llc::error_t	detectSphereCylinder	(const ::llc::SRigidBodyIntegrator & integrator, uint32_t bodySphere, uint32_t bodyCylinder, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) { (void)integrator, (void)bodySphere, (void)bodyCylinder, (void)contactCache, (void)contactsDetected; return 0; }
static	::llc::error_t	detectSame				(const ::llc::SRigidBodyIntegrator & integrator, ::llc::BOUNDING_TYPE bvA, uint32_t rigidBodyA, uint32_t rigidBodyB, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) {
	::llc::error_t				result					= -1;
	switch(bvA) {
	default								: warning_printf("Unsupported type: (%i) %s.", bvA, ::llc::get_value_namep(bvA)); break; 
	case ::llc::BOUNDING_TYPE_Sphere	: result = ::detectSphere  (integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected); break; 
	case ::llc::BOUNDING_TYPE_Cylinder	: result = ::detectCylinder(integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected); break; 
	case ::llc::BOUNDING_TYPE_AABB		: result = ::detectAABB    (integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected); break; 
	} // switch bvA
	return result;
}

static	::llc::error_t	collisionDetectAny		(const ::llc::SRigidBodyIntegrator & integrator, ::llc::BOUNDING_TYPE bvA, ::llc::BOUNDING_TYPE bvB, uint32_t rigidBodyA, uint32_t rigidBodyB, ::llc::SContact & contactCache, ::llc::apod<::llc::SContact> & contactsDetected) {
	::llc::error_t				result					= 0;
	if(bvA == bvB) 
		return detectSame(integrator, bvA, rigidBodyA, rigidBodyB, contactCache, contactsDetected);

	switch(bvA) {
	case ::llc::BOUNDING_TYPE_Sphere	: 
		switch(bvB) { // rigid body A is Sphere
		case ::llc::BOUNDING_TYPE_AABB		: if_true_i(result = ::detectSphereAABB		(integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected)); break; 
		case ::llc::BOUNDING_TYPE_Cylinder	: if_true_i(result = ::detectSphereCylinder	(integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected)); break; 
		}  // switch bvB
		break;
	case ::llc::BOUNDING_TYPE_Cylinder	: 
		switch(bvB) { // rigid body A is Cylinder
		case ::llc::BOUNDING_TYPE_AABB		: if_true_i(result = ::detectCylinderAABB  (integrator, rigidBodyA, rigidBodyB, contactCache, contactsDetected)); break; 
		case ::llc::BOUNDING_TYPE_Sphere	: if_true_i(result = ::detectSphereCylinder(integrator, rigidBodyB, rigidBodyA, contactCache, contactsDetected)); break; 
		}  // switch bvB
		break;
	case ::llc::BOUNDING_TYPE_AABB		: 
		switch(bvB) { // rigid body A is AABB
		case ::llc::BOUNDING_TYPE_Cylinder	: if_true_i(result = ::detectCylinderAABB(integrator, rigidBodyB, rigidBodyA, contactCache, contactsDetected)); break; 
		case ::llc::BOUNDING_TYPE_Sphere	: if_true_i(result = ::detectSphereAABB  (integrator, rigidBodyB, rigidBodyA, contactCache, contactsDetected)); break; 
		}  // switch bvB
		break;
	} // switch bvA
	return result;
}

::llc::error_t			llc::collisionDetect	(const ::llc::SEngine & engine, double totalSeconds, ::llc::apod<::llc::SContact> & contactsDetected) {
	uint32_t					count					= contactsDetected.size();
	for(::llc::SContact	contact	= {totalSeconds}; contact.EntityA < engine.Entities.size(); ++contact.EntityA) {
		const ::llc::SEntity	& entityA				= engine.Entities[contact.EntityA];
		if(entityA.RigidBody >= engine.Integrator.Flags.size())
			continue;

		const ::llc::SBodyFlags		& flagsA				= engine.Integrator.Flags[entityA.RigidBody];
		if(false == flagsA.Collides)
			continue;

		for(contact.EntityB = contact.EntityA + 1; contact.EntityB < engine.Entities.size(); ++contact.EntityB) {
			const ::llc::SEntity	& entityB				= engine.Entities[contact.EntityB];
			if(entityB.RigidBody >= engine.Integrator.Flags.size())
				continue;

			const ::llc::SBodyFlags		& flagsB				= engine.Integrator.Flags[entityB.RigidBody];
			if(false == flagsB.Collides)
				continue;

			llc_necs(::collisionDetectAny(engine.Integrator, (::llc::BOUNDING_TYPE)flagsA.BVType, (::llc::BOUNDING_TYPE)flagsB.BVType, entityA.RigidBody, entityB.RigidBody, contact, contactsDetected));
		}
	}
	//ginfo_if(contactsDetected.size() - count, "%i collisions detected.", contactsDetected.size() - count);
	return contactsDetected.size() - count;
}

//				"\nEntityA       : %i"
//				"\nEntityB       : %i"
//				"\nCenterDistance: " N3_F32
//				"\nDistanceLength: %f"
//				, contact.EntityA
//				, contact.EntityB
//				, llc_xyz(contact.CenterDistance)
//				, contact.DistanceLength
//				);
