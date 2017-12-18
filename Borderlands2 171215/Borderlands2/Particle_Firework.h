#pragma once
class Particle_Firework : public ParticleSystem
{
public:
	Particle_Firework(D3DXVECTOR3* org, int numParticles);

	void ResetParticle(Attribute* att);
	void Update(float dt);
	void PreRender();
	void PostRender();
};

