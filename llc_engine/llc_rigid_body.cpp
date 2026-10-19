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

void					llc::transformInertiaTensor	(::llc::m3a3f2_t & iitWorld, const ::llc::m3a3f2_t & iitBody, const ::llc::m4f2_t & rotmat)		{
	const float					t4							= rotmat(0, 0)*iitBody.Value[0] + rotmat(1, 0)*iitBody.Value[1] + rotmat(2, 0)*iitBody.Value[2];
	const float					t9							= rotmat(0, 0)*iitBody.Value[3] + rotmat(1, 0)*iitBody.Value[4] + rotmat(2, 0)*iitBody.Value[5];
	const float					t14							= rotmat(0, 0)*iitBody.Value[6] + rotmat(1, 0)*iitBody.Value[7] + rotmat(2, 0)*iitBody.Value[8];
	const float					t28							= rotmat(0, 1)*iitBody.Value[0] + rotmat(1, 1)*iitBody.Value[1] + rotmat(2, 1)*iitBody.Value[2];
	const float					t33							= rotmat(0, 1)*iitBody.Value[3] + rotmat(1, 1)*iitBody.Value[4] + rotmat(2, 1)*iitBody.Value[5];
	const float					t38							= rotmat(0, 1)*iitBody.Value[6] + rotmat(1, 1)*iitBody.Value[7] + rotmat(2, 1)*iitBody.Value[8];
	const float					t52							= rotmat(0, 2)*iitBody.Value[0] + rotmat(1, 2)*iitBody.Value[1] + rotmat(2, 2)*iitBody.Value[2];
	const float					t57							= rotmat(0, 2)*iitBody.Value[3] + rotmat(1, 2)*iitBody.Value[4] + rotmat(2, 2)*iitBody.Value[5];
	const float					t62							= rotmat(0, 2)*iitBody.Value[6] + rotmat(1, 2)*iitBody.Value[7] + rotmat(2, 2)*iitBody.Value[8];

	iitWorld.Value[0]			= t4 *rotmat(0, 0) + t9 *rotmat(1, 0) + t14*rotmat(2, 0);
	iitWorld.Value[3]			= t4 *rotmat(0, 1) + t9 *rotmat(1, 1) + t14*rotmat(2, 1);
	iitWorld.Value[6]			= t4 *rotmat(0, 2) + t9 *rotmat(1, 2) + t14*rotmat(2, 2);
	iitWorld.Value[1]			= t28*rotmat(0, 0) + t33*rotmat(1, 0) + t38*rotmat(2, 0);
	iitWorld.Value[4]			= t28*rotmat(0, 1) + t33*rotmat(1, 1) + t38*rotmat(2, 1);
	iitWorld.Value[7]			= t28*rotmat(0, 2) + t33*rotmat(1, 2) + t38*rotmat(2, 2);
	iitWorld.Value[2]			= t52*rotmat(0, 0) + t57*rotmat(1, 0) + t62*rotmat(2, 0);
	iitWorld.Value[5]			= t52*rotmat(0, 1) + t57*rotmat(1, 1) + t62*rotmat(2, 1);
	iitWorld.Value[8]			= t52*rotmat(0, 2) + t57*rotmat(1, 2) + t62*rotmat(2, 2);
}

void					llc::updateTransform	(::llc::SBodyCenter & bodyTransform, ::llc::m4f2_t & transformLocal)	{
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
