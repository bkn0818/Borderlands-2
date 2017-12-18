#include "stdafx.h"
#include "Particle_Firework.h"

Particle_Firework::Particle_Firework(D3DXVECTOR3 * org, int numParticles)
{
	origin		= *org;
	size		= 0.9f;
	vbSize		= 2048;
	vbOffset	= 0;
	vbBatchSize = 512;
}

void Particle_Firework::ResetParticle(Attribute * att)
{
	att->isAlive	= true;
	att->position	= origin;

	D3DXVECTOR3 min = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 max = D3DXVECTOR3( 0.0f,  0.0f,  0.0f);

	GetRandomVector(&att->velocity, &min, &max);

	D3DXVec3Normalize(&att->velocity, &att->velocity);
	att->velocity	*= 100.0f;
	att->color		= D3DXCOLOR(GetRandomFloat(0.0f,1.0f), 
								GetRandomFloat(0.0f, 1.0f), 
								GetRandomFloat(0.0f, 1.0f),
						   1.0f);
	att->age		= 0.0f;
	att->life		= 2.0f;
}

void Particle_Firework::Update(float dt)
{
	list<Attribute>::iterator iter = particles.begin();
	list<Attribute>::iterator iter2 = particles.begin();
	float distance = 0;
	D3DXVECTOR3 r;

	for (iter; iter != particles.end(); ++iter)
	{
		//iter->position = iter->position + (iter->velocity) * dt;
		//iter->acceleration *= 0;
		//
		//for (iter2; iter != particles.end(); ++iter2)
		//{
		//	r = iter2->position - iter->position;
		//	distance = GetDistance3D(iter2->position, iter->position);
		//	if (distance > 1.0f)
		//	{
		//		iter->acceleration += r*(120 / particles.size()) / pow(distance, 3);
		//	}
		//	iter->velocity = iter->velocity + iter->acceleration * dt;
		//}
		if (iter->isAlive)
		{
			iter->position += iter->velocity * dt;
			iter->age += dt;
		
			if (iter->age > iter->life) iter->isAlive = false;
		}
	}
}

void Particle_Firework::PreRender()
{
	ParticleSystem::PreRender();

	D3DDEVICE->SetRenderState(D3DRS_SRCBLEND,	D3DBLEND_ONE);
	D3DDEVICE->SetRenderState(D3DRS_DESTBLEND,	D3DBLEND_ONE);
	D3DDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void Particle_Firework::PostRender()
{
	ParticleSystem::PostRender();
	D3DDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
