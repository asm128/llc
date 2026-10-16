#include "llc_rigid_body.h"
#include "llc_apod_serialize.h"
#include "llc_axis.h"

::llc::error_t			llc::SRigidBodyIntegrator::Load	(::llc::vcu0_t & input) { 
	llc_necs(llc::loadView(input, Frames			));
	llc_necs(llc::loadView(input, Flags			));
	llc_necs(llc::loadView(input, Forces			));
	llc_necs(llc::loadView(input, Masses			));
	llc_necs(llc::loadView(input, Centers			));
	llc_necs(llc::loadView(input, TransformsLocal	));
	return 0;											
}

::llc::error_t			llc::SRigidBodyIntegrator::Save	(::llc::au0_t & output) const { 
	llc_necs(llc::saveView(output, Frames			));
	llc_necs(llc::saveView(output, Flags			));
	llc_necs(llc::saveView(output, Forces			));
	llc_necs(llc::saveView(output, Masses			));
	llc_necs(llc::saveView(output, Centers		));
	llc_necs(llc::saveView(output, TransformsLocal));

	info_printf("Saved %s, %i", "Frames"			, Frames			.size());
	info_printf("Saved %s, %i", "Flags"				, Flags				.size());
	info_printf("Saved %s, %i", "Forces"			, Forces			.size());
	info_printf("Saved %s, %i", "Masses"			, Masses			.size());
	info_printf("Saved %s, %i", "Transforms"		, Centers			.size());
	info_printf("Saved %s, %i", "TransformsLocal"	, TransformsLocal	.size());
	return 0; 
}

void					llc::transformInertiaTensor	(::llc::m3f32 & iitWorld, const ::llc::m3f32 & iitBody, const ::llc::m4f32 & rotmat)		{
	const float					t4							= rotmat._11*iitBody._11 + rotmat._21*iitBody._12 + rotmat._31*iitBody._13;
	const float					t9							= rotmat._11*iitBody._21 + rotmat._21*iitBody._22 + rotmat._31*iitBody._23;
	const float					t14							= rotmat._11*iitBody._31 + rotmat._21*iitBody._32 + rotmat._31*iitBody._33;
	const float					t28							= rotmat._12*iitBody._11 + rotmat._22*iitBody._12 + rotmat._32*iitBody._13;
	const float					t33							= rotmat._12*iitBody._21 + rotmat._22*iitBody._22 + rotmat._32*iitBody._23;
	const float					t38							= rotmat._12*iitBody._31 + rotmat._22*iitBody._32 + rotmat._32*iitBody._33;
	const float					t52							= rotmat._13*iitBody._11 + rotmat._23*iitBody._12 + rotmat._33*iitBody._13;
	const float					t57							= rotmat._13*iitBody._21 + rotmat._23*iitBody._22 + rotmat._33*iitBody._23;
	const float					t62							= rotmat._13*iitBody._31 + rotmat._23*iitBody._32 + rotmat._33*iitBody._33;

	iitWorld._11			= t4 *rotmat._11 + t9 *rotmat._21 + t14*rotmat._31;
	iitWorld._21			= t4 *rotmat._12 + t9 *rotmat._22 + t14*rotmat._32;
	iitWorld._31			= t4 *rotmat._13 + t9 *rotmat._23 + t14*rotmat._33;
	iitWorld._12			= t28*rotmat._11 + t33*rotmat._21 + t38*rotmat._31;
	iitWorld._22			= t28*rotmat._12 + t33*rotmat._22 + t38*rotmat._32;
	iitWorld._32			= t28*rotmat._13 + t33*rotmat._23 + t38*rotmat._33;
	iitWorld._13			= t52*rotmat._11 + t57*rotmat._21 + t62*rotmat._31;
	iitWorld._23			= t52*rotmat._12 + t57*rotmat._22 + t62*rotmat._32;
	iitWorld._33			= t52*rotmat._13 + t57*rotmat._23 + t62*rotmat._33;
}

void					llc::updateTransform	(::llc::SBodyCenter & bodyTransform, ::llc::m4f32 & transformLocal)	{
	//transformLocal.Scale( bodyTransform.Scale, true );
	bodyTransform.Orientation.Normalize();
	transformLocal.SetOrientation(bodyTransform.Orientation);
	transformLocal.SetTranslation(bodyTransform.Position, false);
}

::llc::error_t			llc::integrateForces	(double duration, ::llc::SBodyFrame & bodyFrame, ::llc::SBodyForces & bodyForce, const ::llc::SBodyMass & bodyMass) {
	// -- Calculate linear acceleration from force inputs.
	bodyFrame.LastFrameAcceleration	= bodyForce.Acceleration;
	bodyFrame.LastFrameAcceleration.AddScaled(bodyFrame.AccumulatedForce, bodyMass.InverseMass);
	// -- Adjust velocities
	bodyForce.Velocity.AddScaled(bodyFrame.LastFrameAcceleration, (float)duration);	// Update linear velocity from both acceleration and impulse.

	// -- Calculate angular acceleration from torque inputs.
	const ::llc::n3f2_t			angularAcceleration		= bodyFrame.InverseInertiaTensorWorld.Transform(bodyFrame.AccumulatedTorque);
	bodyForce.Rotation.AddScaled(angularAcceleration, duration);	// Update angular velocity from both acceleration and impulse.

	// Impose drag.
	bodyForce.Velocity		*= pow((double)bodyMass.LinearDamping , duration);
	bodyForce.Rotation		*= pow((double)bodyMass.AngularDamping, duration);
	bodyFrame.ClearAccumulators();
	return 0;
}

::llc::error_t			llc::integratePosition	(double duration, double durationHalfSquared, ::llc::SBodyFlags & bodyFlags, ::llc::SBodyCenter & bodyTransform, const ::llc::SBodyForces & bodyForces) {
	bodyTransform.Position		.AddScaled(bodyForces.Velocity, duration);
	bodyTransform.Position		.AddScaled(bodyForces.Velocity, durationHalfSquared);
	bodyTransform.Orientation	.AddScaled(bodyForces.Rotation, duration);	// Update angular position.
	bodyTransform.Orientation.Normalize();
	bodyFlags.UpdatedTransform		= false;
	bodyFlags.UpdatedTensorWorld	= false;
	return 0;
}
