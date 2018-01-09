#include "stdafx.h"
#include "EnemyManager.h"


// HeightMap
//       x
//       ^ +
//       | 
// z <---- - 
// +     -
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::Init()
{
//	bm[0] = new Bullymong;
//	bm[0]->Init(D3DXVECTOR3(40.0f, 0.0f, 20.0f));

//	bm[1] = new Bullymong;
//	bm[1]->Init(D3DXVECTOR3(40.0f, 0.0f, 60.0f));

	bm = new Bullymong;
	bm->Init(D3DXVECTOR3(60.0f, 0.0f, 60.0f));

//	for each(auto p in bm) {
//		bool isColli = true;
//		// 서로 좌표가 충돌하지 않을 때까지 계속 새로 pos 셋팅
//		while (CollisionEach(p->GetPos(), p->GetRadi())) {
//			p->GetNewPosition(p->GetPos());
//		}
//	}

	return S_OK;
}

void EnemyManager::Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer)
{
//	for each(auto p in bm)
//		p->Update(imap, posPlayer, spherePlayer);

	bm->Update(imap, posPlayer, spherePlayer);
}

void EnemyManager::Render()
{
//	for each(auto p in bm)
//		p->Render();
	bm->Render();
}

void EnemyManager::Release()
{
}


//-----------------------------------------------------------------------------
// 함수: EnemyManager::SetNewPosition()
// 설명: 적끼리 충돌하는 지 여부 
//-----------------------------------------------------------------------------
void EnemyManager::SetNewPosition()
{

}


//-----------------------------------------------------------------------------
// 함수: EnemyManager::CollisionEach()
// 설명: 적끼리 충돌하는 지 여부 
//		 1. 두 enemy의 pos간 거리를 구한다.
//		 2. 두 enemy의 반지름 사이의 합 > 1번 
//		 3. 2번일 때 충돌 
//-----------------------------------------------------------------------------
bool EnemyManager::CollisionEach(D3DXVECTOR3* posEnemy, float radiusEnemy)
{
	/*
	for each(auto i in bm) {
		D3DXVECTOR3 vSub;
		D3DXVec3Subtract(&vSub, i->GetPos(), posEnemy);

		float distance = i->GetRadi() + radiusEnemy;

		if (D3DXVec3LengthSq(&vSub) < distance) {
			return true;
		}
	}
	*/
	return false;
}


void EnemyManager::EnemyCollision()
{}

void EnemyManager::ObjectCollision()
{}