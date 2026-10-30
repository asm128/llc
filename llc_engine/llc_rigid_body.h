#include "llc_matrix3.h"
#include "llc_quat.h"
#include "llc_view.h"
#include "llc_array_obj.h"
#include "llc_enum.h"

#ifndef LLC_RIGIDBODY_H_23627
#define LLC_RIGIDBODY_H_23627

namespace llc
{
#pragma pack(push, 1)
	// The following structure stores forces information for a 3D rigid body. These change almost every frame during runtime
	struct SBodyForces {
		::llc::n3f2_t		Velocity						= {};
		::llc::n3f2_t		Acceleration					= {};
		::llc::n3f2_t		Rotation						= {};
	};

	// The following structure stores mass information for a 3D rigid body. These almost never change during runtime
	struct SBodyMass {
		f2_t				LinearDamping					= 1.0f;
		f2_t				AngularDamping					= 1.0f;
		f2_t				InverseMass						= 0;
		::llc::m3a3f2_t		InverseAngularMassTensor		= {1,0,0,0,1,0,0,0,1};

		inxp	f3_t		GetMass							()	{
			return (InverseMass == 0) ? DBL_MAX : 1.0 / InverseMass; 
		}
	};

	struct SBodyCenter {
		::llc::n3f2_t		Position						= {};
		::llc::quatf2_t		Orientation						= {0, 0, 0, 1};
	};

	GDEFINE_ENUM_TYPE(BOUNDING_TYPE, u0_t);
	GDEFINE_ENUM_VALUE(BOUNDING_TYPE, Sphere	, 0);
	GDEFINE_ENUM_VALUE(BOUNDING_TYPE, AABB		, 1);
	GDEFINE_ENUM_VALUE(BOUNDING_TYPE, Ray		, 2);
	GDEFINE_ENUM_VALUE(BOUNDING_TYPE, Cylinder	, 3);

	GDEFINE_ENUM_VALUE(BOUNDING_TYPE, Inward	, 4);

	struct SBodyFlags {
		u0_t				BVType							: 3;
		u0_t				Collides						: 1;
		u0_t				Active							: 1;
		u0_t				Falling							: 1;
		u0_t				UpdatedTransform				: 1;
		u0_t				UpdatedTensorWorld				: 1;	// Tell the object that our matrices are up to date
	};

	struct SBodyFrame {
		::llc::m3a3f2_t		InverseInertiaTensorWorld		= {1, 0, 0, 0, 1, 0, 0, 0, 1};
		::llc::n3f2_t		LastFrameAcceleration			= {0, 0, 0};
		::llc::n3f2_t		AccumulatedForce				= {0, 0, 0};
		::llc::n3f2_t		AccumulatedTorque				= {0, 0, 0};

		s2_t				ClearAccumulators				()	{ AccumulatedForce = AccumulatedTorque = {}; return 0; }
	};

	struct SBodyVolume {
		::llc::n3f2_t		HalfSizes						= {.5f, .5f, .5f};
	};
#pragma pack(pop)
	void				updateTransform					(::llc::SBodyCenter & bodyTransform, ::llc::m4f2_t & transformLocal);
	s2_t				integrateForces					(f3_t duration, ::llc::SBodyFrame & bodyFrame, ::llc::SBodyForces & bodyForce, const ::llc::SBodyMass & bodyMass);
	s2_t				integratePosition				(f3_t duration, f3_t durationHalfSquared, ::llc::SBodyFlags & bodyFlags, ::llc::SBodyCenter & bodyTransform, const ::llc::SBodyForces & bodyForces);
	void				transformInertiaTensor			(::llc::m3a3f2_t & iitWorld, const ::llc::m3a3f2_t & iitBody, const ::llc::m4f2_t &rotmat);


	struct SRigidBodyIntegrator {
		::llc::apod<::llc::SBodyFrame	>	Frames							= {};
		::llc::apod<::llc::SBodyFlags	>	Flags							= {};
		::llc::apod<::llc::SBodyMass	>	Masses							= {};
		::llc::apod<::llc::SBodyForces	>	Forces							= {};
		::llc::apod<::llc::SBodyCenter	>	Centers							= {};
		::llc::apod<::llc::SBodyVolume	>	BoundingVolumes					= {};
		::llc::apod<::llc::m4f2_t		>	TransformsLocal					= {};

		stxp	::llc::m4f2_t				MatrixIdentity4					= {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
		stxp	::llc::m3a3f2_t				MatrixIdentity3					= {1,0,0,0,1,0,0,0,1};

		::llc::err_t						Load							(::llc::vcu0_t & input);
		::llc::err_t						Save							(::llc::au0_t & output)	const;
		inline	::llc::err_t				Load							(::llc::vcst_t & input)			{ return Load(*((::llc::vcu0_t*)&input)); }
		inline	::llc::err_t				Save							(::llc::as0_t & output)	const	{ return Save(*((::llc::apod<u0_t>*)&output)); }

		s2_t 							ZeroCenters						()	{ return Centers.fill({}); }
		s2_t 							ZeroForces						()	{ return Forces.fill({}); }
		s2_t 							ZeroFrames						()	{ return Frames.fill({}); }

		s2_t 							Reset							()	{
			ZeroForces();
			ZeroCenters();
			return ZeroFrames();
		}
		
		s2_t 							Clear							()	{
			::llc::clear(Frames, Flags, Forces, Masses, Centers, BoundingVolumes, TransformsLocal);
			return 0;
		}

		s2_t 							Clone							(::llc::u2_t iBody)	{
			Frames					.push_back(::llc::SBodyFrame		{Frames				[iBody]});
			Flags					.push_back(::llc::SBodyFlags		{Flags				[iBody]});
			Forces					.push_back(::llc::SBodyForces		{Forces				[iBody]});
			Masses					.push_back(::llc::SBodyMass			{Masses				[iBody]});
			Centers					.push_back(::llc::SBodyCenter		{Centers			[iBody]});
			BoundingVolumes			.push_back(::llc::SBodyVolume		{BoundingVolumes	[iBody]});
			return TransformsLocal	.push_back(::llc::m4f2_t				{TransformsLocal	[iBody]});
		}

		s2_t 							Create							()	{
			Frames					.push_back({});
			Flags					.push_back({});
			Forces					.push_back({});
			Masses					.push_back({});
			Centers					.push_back({});
			BoundingVolumes			.push_back({});
			return TransformsLocal	.push_back(MatrixIdentity4);
		}

		s2_t 							Delete							(::llc::u2_t iBody)	{
			Frames					.remove_unordered(iBody);
			Flags					.remove_unordered(iBody);
			Forces					.remove_unordered(iBody);
			Masses					.remove_unordered(iBody);
			Centers					.remove_unordered(iBody);
			BoundingVolumes			.remove_unordered(iBody);
			return TransformsLocal	.remove_unordered(iBody);
		}

		s2_t 							Create							(::llc::u2_t bodyCount)	{
			const ::llc::u2_t							indexFirstBody					= Centers.size();
			const ::llc::u2_t							newSize							= indexFirstBody + bodyCount;
			if_fail_fe(Frames				.resize(newSize, {}));				
			if_fail_fe(Flags				.resize(newSize, {}));
			if_fail_fe(Forces				.resize(newSize, {}));
			if_fail_fe(Masses				.resize(newSize, {}));
			if_fail_fe(Centers			.resize(newSize, {}));
			if_fail_fe(BoundingVolumes	.resize(newSize, {}));		
			if_fail_fe(TransformsLocal	.resize(newSize, MatrixIdentity4));
			return indexFirstBody;
		}

		s2_t 							GetTransform					(::llc::u2_t iBody, ::llc::m4f2_t	& transform)	{
			::llc::SBodyFlags						& bodyFlags						= Flags				[iBody];
			::llc::m4f2_t							& bodyTransformLocal			= TransformsLocal	[iBody];
			if(false == bodyFlags.UpdatedTransform || false == bodyFlags.UpdatedTensorWorld) {
				if(false == bodyFlags.UpdatedTransform) {
					::llc::SBodyCenter						& bodyCenter					= Centers[iBody];
					::llc::updateTransform(bodyCenter, bodyTransformLocal);
					bodyFlags.UpdatedTransform			= true;
					bodyFlags.UpdatedTensorWorld		= false;
				}
				if(false == bodyFlags.UpdatedTensorWorld) {
					::llc::SBodyFrame						& bodyFrame						= Frames[iBody];
					::llc::SBodyMass						& bodyMass						= Masses[iBody];
					::llc::transformInertiaTensor(bodyFrame.InverseInertiaTensorWorld, bodyMass.InverseAngularMassTensor, bodyTransformLocal);
					bodyFlags.UpdatedTensorWorld		= true;
				}
			}
			transform							= bodyTransformLocal;
			return 0;
		}

		void								AddForceAtPoint					(::llc::u2_t iBody, const ::llc::n3f2_t & force, const ::llc::n3f2_t & point)	{
			::llc::SBodyCenter						& bodyCenter					= Centers	[iBody];
			::llc::SBodyFlags						& bodyFlags						= Flags	[iBody];
			::llc::SBodyFrame						& bodyFrame						= Frames[iBody];
			bodyFrame.AccumulatedTorque			+= (point - bodyCenter.Position).Cross( force );// Convert to coordinates relative to center of mass.
			bodyFrame.AccumulatedForce 			+= force;
			bodyFlags.Active					= true;
		}

		s2_t								Integrate						(f3_t duration)	{
			const f3_t							durationHalfSquared				= duration * duration * 0.5;
			::llc::m4f2_t							dummy;
			for(::llc::u2_t iBody = 0; iBody < Flags.size(); ++iBody) {
				::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
				if(false == bodyFlags.Active)
					continue;

				GetTransform(iBody, dummy); // ensures tensor matrix is up to date

				::llc::SBodyFrame					& bodyFrame						= Frames	[iBody];
				::llc::SBodyForces					& bodyForces					= Forces	[iBody];
				::llc::SBodyMass					& bodyMass						= Masses	[iBody];
				::llc::SBodyCenter					& bodyCenter					= Centers	[iBody];
				::llc::integrateForces	(duration, bodyFrame, bodyForces, bodyMass);
				::llc::integratePosition(duration, durationHalfSquared, bodyFlags, bodyCenter, bodyForces);
				if(bodyForces.Acceleration.LengthSquared() < .001 && bodyForces.Velocity.LengthSquared() < .001 && bodyForces.Rotation.LengthSquared() < .00000001) {
					bodyFlags.Active				= false;
					bodyForces						= {};
				}
			}
			return 0;
		}

		inline	const SBodyFlags&			GetBodyFlags					(::llc::u2_t iBody)	const	{ return Flags  [iBody]; }
		inline	const SBodyFrame&			GetBodyFrame					(::llc::u2_t iBody)	const	{ return Frames [iBody]; }
		inline	const SBodyForces&			GetBodyForces					(::llc::u2_t iBody)	const	{ return Forces [iBody]; }
		inline	const SBodyMass&			GetBodyMass						(::llc::u2_t iBody)	const	{ return Masses [iBody]; }
		inline	const SBodyCenter&			GetBodyCenter					(::llc::u2_t iBody)	const	{ return Centers[iBody]; }
		inline	SBodyFlags&					GetBodyFlags					(::llc::u2_t iBody)			{ return Flags  [iBody]; }
		inline	SBodyFrame&					GetBodyFrame					(::llc::u2_t iBody)			{ return Frames [iBody]; }
		inline	SBodyForces&				GetBodyForces					(::llc::u2_t iBody)			{ return Forces [iBody]; }
		inline	SBodyMass&					GetBodyMass						(::llc::u2_t iBody)			{ return Masses [iBody]; }
		inline	SBodyCenter&				GetBodyCenter					(::llc::u2_t iBody)			{ return Centers[iBody]; }


		inline	bool						Active							(::llc::u2_t iBody)									const	{ return Flags[iBody].Active; }
		inline	bool						Collides						(::llc::u2_t iBody)									const	{ return Flags[iBody].Collides; }
		inline	f2_t						GetMass							(::llc::u2_t iBody)									const	{ return 1.0f / Masses[iBody].InverseMass; }
		inline	void						GetPosition						(::llc::u2_t iBody, ::llc::n3f2_t & position)			const	{ position		= Centers[iBody].Position; }
		inline	void						GetOrientation					(::llc::u2_t iBody, ::llc::quatf2_t & orientation)		const	{ orientation	= Centers[iBody].Orientation; }
		inline	void						GetRotation						(::llc::u2_t iBody, ::llc::n3f2_t & rotation)			const	{ rotation		= Forces[iBody].Rotation;		}
		inline	void						GetAcceleration					(::llc::u2_t iBody, ::llc::n3f2_t & acceleration)		const	{ acceleration	= Forces[iBody].Acceleration;	}
		inline	void						GetVelocity						(::llc::u2_t iBody, ::llc::n3f2_t & velocity)			const	{ velocity		= Forces[iBody].Velocity;		}

		inline	void						SetActive						(::llc::u2_t iBody, bool active)								{ Flags[iBody].Active	= active; }
		inline	void						SetCollides						(::llc::u2_t iBody, bool collides)								{ Flags[iBody].Collides	= collides; }
		inline	void						SetHalfSizes					(::llc::u2_t iBody, const ::llc::n3f2_t & halfSizes)			{ BoundingVolumes[iBody].HalfSizes	= halfSizes; }
		
		inline	void						SetMassInverse					(::llc::u2_t iBody, f2_t inverseMass)							{ Masses[iBody].InverseMass = inverseMass; }
		inline	void						SetMass							(::llc::u2_t iBody, f2_t mass)								{ Masses[iBody].InverseMass = 1.0f / mass; }
		inline	void						AddForce						(::llc::u2_t iBody, const ::llc::n3f2_t & force)				{ Frames[iBody].AccumulatedForce += force; }

		void								SetPosition						(::llc::u2_t iBody, const ::llc::n3f2_t & newPosition)			{
			::llc::SBodyCenter						& bodyCenter					= Centers[iBody];
			if(0 == memcmp(&newPosition.x, &bodyCenter.Position.x, sizeof(::llc::n3f2_t)))
				return;
			bodyCenter.Position					= newPosition;
			::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
			bodyFlags.UpdatedTransform			= 
			bodyFlags.UpdatedTensorWorld		= false;
		}
		void								SetOrientation					(::llc::u2_t iBody, const ::llc::quatf2_t & newOrientation)		{
			::llc::SBodyCenter						& bodyCenter					= Centers[iBody];
			if(0 == memcmp(&newOrientation.x, &bodyCenter.Orientation.x, sizeof(::llc::quat<f2_t>)))
				return;
			bodyCenter.Orientation				= newOrientation;
			::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
			bodyFlags.UpdatedTransform			= 
			bodyFlags.UpdatedTensorWorld		= false;
		}
		void								SetVelocity						(::llc::u2_t iBody, const ::llc::n3f2_t & newVelocity)			{
			::llc::SBodyForces						& bodyForces					= Forces[iBody];
			if(0 == memcmp(&newVelocity.x, &bodyForces.Velocity.x, sizeof(::llc::n3f2_t)))
				return;
			bodyForces.Velocity					= newVelocity;
			::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
			bodyFlags.Active					= true;
			if(newVelocity.y) 
				bodyFlags.Falling					= true; 
		}
		void								SetAcceleration					(::llc::u2_t iBody, const ::llc::n3f2_t & acceleration)			{
			::llc::SBodyForces						& bodyForces					= Forces[iBody];
			if(0 == memcmp(&acceleration.x, &bodyForces.Acceleration.x, sizeof(::llc::n3f2_t)))
				return;
			bodyForces.Acceleration				= acceleration;
			::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
			bodyFlags.Active					= true;
		}
		void								SetRotation						(::llc::u2_t iBody, const ::llc::n3f2_t & newRotation)			{
			::llc::SBodyForces						& bodyForces					= Forces[iBody];
			if(0 == memcmp(&newRotation.x, &bodyForces.Rotation.x, sizeof(::llc::n3f2_t)))
				return;
			bodyForces.Rotation					= newRotation;
			::llc::SBodyFlags						& bodyFlags						= Flags[iBody];
			bodyFlags.Active					= true;
			if(newRotation.z || newRotation.x) 
				bodyFlags.Falling					= true; 
		}

	};

} // namespace

#endif // CED_RIGIDBODY_H_23627
