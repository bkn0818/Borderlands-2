#pragma once
#include "ParticleSystem.h"

class Particle_Snow : public ParticleSystem
{
public:
	Particle_Snow(BoundingBox* boundingBox, int numParticles);

	void ResetParticle(Attribute* attribute);
	void Update(float deltaTime);
};

