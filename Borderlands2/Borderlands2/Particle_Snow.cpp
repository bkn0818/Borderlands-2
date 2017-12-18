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

	//�������� ��ġ ����(���� x, y ��ǥ)
	GetRandomVector(&attribute->position, &boundingBox.min, &boundingBox.max);

	//����(y��ǥ)�� �׻� ��� ������ �ֻ������ ����
	attribute->position.y = boundingBox.max.y;

	//�����̴� �Ʒ������� �������� �ణ �������� ���Ѵ�
	attribute->velocity.x = GetRandomFloat(0.0f, 1.0f) * -3.0f;
	attribute->velocity.y = GetRandomFloat(0.0f, 1.0f) * -10.0f;
	attribute->velocity.z = 0.0f;

	//����� ������
	attribute->color = RED;
}

void Particle_Snow::Update(float deltaTime)
{
	list<Attribute>::iterator iter = particles.begin();
	for (iter; iter != particles.end(); ++iter)
	{
		iter->position += iter->velocity * deltaTime;

		//����Ʈ�� ��踦 ����ٸ�
		if (!boundingBox.IsPointInside(iter->position))
			ResetParticle(&(*iter));
	}
}
