#include "stdafx.h"
#include "Enemy.h"


//-----------------------------------------------------------------------------
// Enemy::SetHeightMap()
// ����: HeightMap�� ���� y��ǥ ó�� 
//-----------------------------------------------------------------------------
void Enemy::SetHeightMap(iMap* imap)
{
	D3DXVECTOR3 temp = pos;

	if (imap && imap->GetHeight(temp.x, temp.y, temp.z))
		pos = temp;

	sm->SetPosition(pos);
}


//-----------------------------------------------------------------------------
// Enemy::SetBoundingSphere()
// ����: ������, rgb���� �޾� bouding Sphere�� mesh�� material�� �����Ѵ�. 
//-----------------------------------------------------------------------------
void Enemy::SetBoundingSphere(tagBoundingSphere& sphere, float radius, int r, int g, int b)
{
	sphere.sphereInfo.fRadius = radius;

	D3DXCreateSphere(D3DDEVICE, radius, 20, 20, &sphere.sphereMesh, NULL);

	ZeroMemory(&sphere.material, sizeof(D3DMATERIAL9));
	D3DXCOLOR color = D3DXCOLOR(float(r / 255), float(g / 255), float(b / 255), 1.0f);
	sphere.material.Ambient = sphere.material.Diffuse = sphere.material.Specular = color;

	sphere.show = false;
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


//-----------------------------------------------------------------------------
// Enemy::IsCollideSphere()
// ����: enemy - ��ġ, bouding Sphere ������
//       player - ��ġ, bouding Sphere �������� �޾� �浹 ���θ� Ȯ���Ѵ�.
//       �� ��ġ�� �Ÿ� <= �� �������� ���� �� �浹 
//-----------------------------------------------------------------------------
bool Enemy::IsCollideSphere(D3DXVECTOR3* spCenter1, float spRadius1, D3DXVECTOR3* spCenter2, float spRadius2)
{
	D3DXVECTOR3 vSub;
	D3DXVec3Subtract(&vSub, spCenter2, spCenter1);
	distance = D3DXVec3Length(&vSub);

	if (distance <= (spRadius1 + spRadius2))
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Enemy::IsRecogPlayer(D3DXVECTOR3 position, SphereInfo* playerSphere)
// ����: �÷��̾� sphere�� �浹 ���θ� �˻��ϰ�
//		 �浹 �� �ν� ���·� �����Ѵ�.
//-----------------------------------------------------------------------------
void Enemy::IsRecogPlayer(D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer)
{
	if (IsCollideSphere(&pos, stBSphere[SPHERE_RECOG].sphereInfo.fRadius, &posPlayer, spherePlayer->fRadius))
		isRecog = true;
}


//-----------------------------------------------------------------------------
// Enemy::GetNewPosition()
// ����: ��ǥ ������ �����ϰ� �����Ѵ�. 
//		 y��ǥ�� ���� pos.y�� ������ �����Ѵ�. 
//-----------------------------------------------------------------------------
void Enemy::GetNewPosition(D3DXVECTOR3* ptarget)
{
	ptarget->x = (float)(rand() % 256) / 256.f;
	ptarget->y = pos.y;
	ptarget->z = (float)(rand() % 256) / 256.f;
}


//-----------------------------------------------------------------------------
// �Լ�: Enemy::SetNewTarget()
// ����: ���� �̵��� ������ ã�´�.
//-----------------------------------------------------------------------------
void Enemy::SetNewTarget(D3DXVECTOR3* pEach, float radiusEach)
{
	// ��ǥ������ ã�´�.
	while (isColliEach)
	{
		GetNewPosition(&posNext);

		if (!IsCollideSphere(pEach, radiusEach, &pos, stBSphere[SPHERE_ASSAULT].sphereInfo.fRadius))
			isColliEach = false;
	}

	ComputeDirection();
}


//-----------------------------------------------------------------------------
// Enemy::ComputeDirection()
// ����: ���� �������� ��ǥ ���������� ������ ���Ѵ�. 
//-----------------------------------------------------------------------------
void Enemy::ComputeDirection()
{
	//   2        3     1
	//   |  |  |  |  |  | + x 
	//   pT |  |  |  |  p
	//   |  |  p  |  |  |
	//   |  |  |  |  |  pT
	//   |  |  |  pT |  |
	// --|--|--|--|--|--| -
	// +          -
	// z  
	D3DXVECTOR3 vDiff;
	D3DXVec3Subtract(&vDiff, &posNext, &pos);
	D3DXVec3Normalize(&vDiff, &vDiff);

	// 1. ���� ������ 
	if (vDiff.z == 0.f) {
		if (vDiff.x > 0.f)		// Next Position�� ����
			direction = D3DX_PI;
		else					// Next Position�� ���� 
			direction = 0.0f;
	}
	// 2. ���� position���� Next position�� ����
	else if (vDiff.z > 0.f) {
		direction = acosf(vDiff.x);
	}
	// 3. Next position�� ���� position���� ������
	else if (vDiff.z < 0.f) {
		direction = acosf(-vDiff.x) + D3DX_PI;
	}
}