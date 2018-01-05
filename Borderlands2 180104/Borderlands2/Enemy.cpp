#include "stdafx.h"
#include "Enemy.h"


//-----------------------------------------------------------------------------
// Enemy::SetHeightMap()
// 설명: HeightMap에 따른 y좌표 처리 
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
// 설명: 반지름, rgb값을 받아 bouding Sphere의 mesh와 material을 생성한다. 
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
// 설명: enemy - 위치, bouding Sphere 반지름
//       player - 위치, bouding Sphere 반지름을 받아 충돌 여부를 확인한다.
//       두 위치의 거리 <= 두 반지름의 합일 때 충돌 
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
// 설명: 플레이어 sphere와 충돌 여부를 검사하고
//		 충돌 시 인식 상태로 돌입한다.
//-----------------------------------------------------------------------------
void Enemy::IsRecogPlayer(D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer)
{
	if (IsCollideSphere(&pos, stBSphere[SPHERE_RECOG].sphereInfo.fRadius, &posPlayer, spherePlayer->fRadius))
		isRecog = true;
}


//-----------------------------------------------------------------------------
// Enemy::GetNewPosition()
// 설명: 목표 지점을 랜덤하게 설정한다. 
//		 y좌표는 현재 pos.y의 값으로 설정한다. 
//-----------------------------------------------------------------------------
void Enemy::GetNewPosition(D3DXVECTOR3* ptarget)
{
	ptarget->x = (float)(rand() % 256) / 256.f;
	ptarget->y = pos.y;
	ptarget->z = (float)(rand() % 256) / 256.f;
}


//-----------------------------------------------------------------------------
// 함수: Enemy::SetNewTarget()
// 설명: 새로 이동할 지점을 찾는다.
//-----------------------------------------------------------------------------
void Enemy::SetNewTarget(D3DXVECTOR3* pEach, float radiusEach)
{
	// 목표지점를 찾는다.
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
// 설명: 현재 지점에서 목표 지점까지의 방향을 구한다. 
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

	// 1. 같은 세로줄 
	if (vDiff.z == 0.f) {
		if (vDiff.x > 0.f)		// Next Position이 뒤쪽
			direction = D3DX_PI;
		else					// Next Position이 앞쪽 
			direction = 0.0f;
	}
	// 2. 현재 position보다 Next position이 왼쪽
	else if (vDiff.z > 0.f) {
		direction = acosf(vDiff.x);
	}
	// 3. Next position이 현재 position보다 오른쪽
	else if (vDiff.z < 0.f) {
		direction = acosf(-vDiff.x) + D3DX_PI;
	}
}