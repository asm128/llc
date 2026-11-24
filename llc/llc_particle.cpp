#include "llc_particle.h"
#include "llc_apod_serialize.h"

::llc::err_t			llc::SParticles3::IntegrateSpeed		(double secondsLastFrame)	{
	for(::llc::u2_t iShot = 0; iShot < Position.size(); ++iShot) {
		::llc::n3f2_t							& direction			= Direction	[iShot];
		::llc::n3f2_t							& position			= Position	[iShot];
		float											& speed				= Speed		[iShot];
		position									+= direction * speed * secondsLastFrame;
	}
	return 0;
}

::llc::err_t			llc::SParticles3::Remove				(int32_t iParticle)	{
	Position	.remove_unordered(iParticle);
	Direction	.remove_unordered(iParticle);
	return Speed.remove_unordered(iParticle);
}

::llc::err_t			llc::SParticles3::Create				(n3f2_c & position, n3f2_c & direction, float speed)	{
	Position	.push_back(position);
	Direction	.push_back(direction);
	return Speed.push_back(speed);
}

::llc::err_t			llc::SParticles3::Save(::llc::au0_t & output) const { 
	if_fail_fe(llc::saveView(output, Position	)); 
	if_fail_fe(llc::saveView(output, Direction	)); 
	if_fail_fe(llc::saveView(output, Speed		)); 
	info_printf("Saved %s, %i", "Position	", Position		.size()); 
	info_printf("Saved %s, %i", "Direction	", Direction	.size()); 
	info_printf("Saved %s, %i", "Speed		", Speed		.size()); 
	return 0; 
}

::llc::err_t			llc::SParticles3::Load(::llc::vcu0_t & input) { 
	if_fail_fe(llc::loadView(input, Position	));
	if_fail_fe(llc::loadView(input, Direction	));
	if_fail_fe(llc::loadView(input, Speed		));
	return 0;
}
