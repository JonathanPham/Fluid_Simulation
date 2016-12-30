#include <MarkerParticleSet.h>

MarkerParticle::MarkerParticle() :
	_pos_x(0),
	_pos_y(0),
	_vel_x(0),
	_vel_y(0)
{

}

MarkerParticle::MarkerParticle(
	MyFloat pos_x,
	MyFloat pos_y,
	MyFloat vel_x,
	MyFloat vel_y) :
	_pos_x(pos_x),
	_pos_y(pos_y),
	_vel_x(vel_x),
	_vel_y(vel_y)
{

}

MarkerParticle::~MarkerParticle()
{
	
}

MarkerParticleSet::MarkerParticleSet(int size)
{
	_particles.resize(size);
}

MarkerParticleSet::~MarkerParticleSet()
{

}

void MarkerParticleSet::addParticle(MyFloat pos_x, MyFloat pos_y)
{
	_particles.push_back(MarkerParticle(pos_x, pos_y));
}

void MarkerParticleSet::reserve(int particle_count)
{
	_particles.reserve(particle_count);
}

void MarkerParticleSet::advect(MyFloat dt)
{
	for (auto it = _particles.begin(); it != _particles.end(); it++)
	{
		it->advect(dt);
	}
}
