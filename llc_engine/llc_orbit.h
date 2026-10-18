#include "llc_axis.h"
#include "llc_quat.h"

#ifndef LLC_ORBTIER_H
#define LLC_ORBTIER_H

namespace llc
{

#pragma pack(push, 1)
	struct SOrbit {
		float		Radius				= {}; // 57.9	// (10^6 km)	// 7
		float		AxialTilt			= {}; // degrees
		float		RotationPeriod		= {}; // in units
		float		RotationUnit		= {}; // 
		::llc::AXIS	RotationAxis		= ::llc::AXIS_Y_POSITIVE; // 
		::llc::AXIS	TiltAxis			= ::llc::AXIS_X_POSITIVE; // 
	};

	struct SOrbiter {
		SOrbit			Body ;	// The mass or energy body representing the object orbiting around a center.
		SOrbit			Orbit;	// The orbit around the center the body orbits around.
	};
#pragma pack(pop)

	stincxp	double			degreesToRadians		(double degrees) { return ::llc::math_2pi / 360.0 * degrees; }

	// Calculate the axial inclination of the planet IN RADIANS
	tplt<tpnm _tAxis>
	static ::llc::error_t	orientationFromEuler	(::llc::AXIS iAxis, double axialTilt, ::llc::quat<_tAxis> & orientation) {
		const _tAxis				rotationValue			= _tAxis(degreesToRadians(axialTilt));
		switch(iAxis) {
		case ::llc::AXIS_X_NEGATIVE: orientation.MakeFromEuler(-rotationValue, 0, 0); break;
		case ::llc::AXIS_X_POSITIVE: orientation.MakeFromEuler( rotationValue, 0, 0); break;
		case ::llc::AXIS_Y_NEGATIVE: orientation.MakeFromEuler(0, -rotationValue, 0); break;
		case ::llc::AXIS_Y_POSITIVE: orientation.MakeFromEuler(0,  rotationValue, 0); break;
		case ::llc::AXIS_Z_NEGATIVE: orientation.MakeFromEuler(0, 0, -rotationValue); break;
		case ::llc::AXIS_Z_POSITIVE: orientation.MakeFromEuler(0, 0,  rotationValue); break;
		}
		orientation.Normalize();
		return 0;
	}

	// Calculate the rotation velocity of the planet IN EARTH DAYS
	tplt<tpnm _tAxis, tpnm _tAxisOrientation>
	static ::llc::error_t	orbitRotation			(::llc::AXIS iAxis, double rotation_period, double rotation_unit, const ::llc::quat<_tAxisOrientation> & orientation, ::llc::n3<_tAxis> & rotation) {
		const _tAxis				rotationValue			= _tAxis(::llc::math_2pi / rotation_period * rotation_unit);
		switch(iAxis) {
		case ::llc::AXIS_X_NEGATIVE: rotation = {-rotationValue, 0, 0}; break;
		case ::llc::AXIS_X_POSITIVE: rotation = { rotationValue, 0, 0}; break;
		case ::llc::AXIS_Y_NEGATIVE: rotation = {0, -rotationValue, 0}; break;
		case ::llc::AXIS_Y_POSITIVE: rotation = {0,  rotationValue, 0}; break;
		case ::llc::AXIS_Z_NEGATIVE: rotation = {0, 0, -rotationValue}; break;
		case ::llc::AXIS_Z_POSITIVE: rotation = {0, 0,  rotationValue}; break;
		}
		rotation				= orientation.RotateVector(rotation);		// Rotate our calculated torque in relation to the planetary axis
		return 0;
	}

	// Calculate the rotation velocity of the planet IN EARTH DAYS
	tplt<tpnm _tAxis, tpnm _tAxisOrientation>
	static ::llc::error_t	orbitCenter			(::llc::AXIS iAxis, double position, ::llc::n3<_tAxis> & center) {
		switch(iAxis) {
		case ::llc::AXIS_X_NEGATIVE: center = {-position, 0, 0}; break;
		case ::llc::AXIS_X_POSITIVE: center = { position, 0, 0}; break;
		case ::llc::AXIS_Y_NEGATIVE: center = {0, -position, 0}; break;
		case ::llc::AXIS_Y_POSITIVE: center = {0,  position, 0}; break;
		case ::llc::AXIS_Z_NEGATIVE: center = {0, 0, -position}; break;
		case ::llc::AXIS_Z_POSITIVE: center = {0, 0,  position}; break;
		}
		return 0;
	}

} // namespace

#endif // LLC_ORBTIER_H