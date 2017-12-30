#include "stdafx.h"
#include "Enemy.h"

void Enemy::SetBoundingSphere(tagBoundingSphere& sphere, float radius, int r, int g, int b)
{
	sphere.sphereInfo.fRadius = radius;

	D3DXCreateSphere(D3DDEVICE, radius, 20, 20, &sphere.sphereMesh, NULL);

	ZeroMemory(&sphere.material, sizeof(D3DMATERIAL9));
	sphere.material.Diffuse.r = sphere.material.Ambient.r = float(r / 255);
	sphere.material.Diffuse.g = sphere.material.Ambient.g = float(g / 255);
	sphere.material.Diffuse.b = sphere.material.Ambient.b = float(b / 255);
	sphere.material.Diffuse.a = sphere.material.Ambient.a = 1.f;

/*
	LPD3DXMESH tempSp = NULL;
	D3DXCreateSphere(D3DDEVICE, radius, 10, 10, &tempSp, NULL);
	tempSp->CloneMeshFVF(0, VertexPC::FVF, D3DDEVICE, &sphere.sphereMesh);

	LPDIRECT3DVERTEXBUFFER9 tempVBuffer;
	if (SUCCEEDED(sphere.sphereMesh->GetVertexBuffer(&tempVBuffer)))
	{
		int numVerts = sphere.sphereMesh->GetNumVertices();
		VertexPC* vertex = NULL; 

		tempVBuffer->Lock(0, 0, (void**)&vertex, 0);
		{
			for (int i = 0; i < numVerts; ++i)
				vertex[i].c = D3DCOLOR_COLORVALUE(float(r / 255), float(g / 255), float(b / 255), 1.0f);
		}
		tempVBuffer->Unlock();
		tempVBuffer->Release();
	}
*/	
}

bool Enemy::CollisionSphere(D3DXVECTOR3 spCenter1, float spRadius1, D3DXVECTOR3 spCenter2, float spRadius2)
{
	D3DXVECTOR3 temp = spCenter2 - spCenter1;
	distance = D3DXVec3Length(&temp);

	if (distance <= (spRadius1 + spRadius2))
		return true;

	return false;
}
