#include "stdafx.h"
#include "Enemy.h"

void Enemy::SetBoundingSphere(tagBoundingSphere& sphere, float radius, int r, int g, int b)
{
	sphere.sphereInfo.fRadius = radius;

	D3DXCreateSphere(D3DDEVICE, radius, 10, 10, &sphere.sphereMesh, NULL);

	D3DXCOLOR xColor(float(r / 255), float(g / 255), float(b / 255), 1.0f);
	ZeroMemory(&sphere.material, sizeof(D3DMATERIAL9));
	sphere.material.Ambient = sphere.material.Diffuse = sphere.material.Specular = xColor;
}

bool Enemy::CollisionSphere(D3DXVECTOR3 spCenter1, float spRadius1, D3DXVECTOR3 spCenter2, float spRadius2)
{
	float distance;

	D3DXVECTOR3 temp = spCenter2 - spCenter1;
	distance = D3DXVec3Length(&temp);

	if (distance <= (spRadius1 + spRadius2))
		return true;

	return false;
}