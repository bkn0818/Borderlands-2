#include "stdafx.h"
#include "Particle_Snow.h"

Particle_Snow::Particle_Snow(BoundingBox * Boundingbox, int numParticles)
{
	boundingBox	= *Boundingbox;
	size		= 1.0f;
	vbSize		= 2048;
	vbOffset	= 1;
	vbBatchSize = 500;

	for (int i = 0; i < numParticles; ++i)
	{
		AddParticle();
	}
}

void Particle_Snow::ResetParticle(Attribute * attribute)
{
	attribute->isAlive = true;

	//눈송이의 위치 지정(랜덤 x, y 좌표)
	GetRandomVector(&attribute->position, &boundingBox.min, &boundingBox.max);

	//높이(y좌표)는 항상 경계 상자의 최상단으로 설정
	attribute->position.y = boundingBox.max.y;

	//눈송이는 아래쪽으로 떨어지고 약간 왼쪽으로 향한다
	attribute->velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->velocity.z = 0.0f;

	//흰색의 눈송이
	attribute->color = RED;
}

void Particle_Snow::Update(float deltaTime)
{
	list<Attribute>::iterator iter = particles.begin();
	for (iter; iter != particles.end(); ++iter)
	{
		iter->position += iter->velocity * deltaTime;

		//포인트가 경계를 벗어났다면
		if (!boundingBox.IsPointInside(iter->position))
			ResetParticle(&(*iter));
	}
}
