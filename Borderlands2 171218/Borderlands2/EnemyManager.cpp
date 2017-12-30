#include "stdafx.h"
#include "EnemyManager.h"


EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

HRESULT EnemyManager::Init()
{
	bm = new Bullymong;
	bm->Init(D3DXVECTOR3(40.0f, 0.0f, 20.0f));
	//x : 뒤로 앞으로
	//z : 옆으로 

	return S_OK;
}

void EnemyManager::Update(iMap* obj, D3DXVECTOR3 playerPosition, SphereInfo* playerSphere)
{
	bm->Update(obj, playerPosition, playerSphere);
}

void EnemyManager::Render()
{
	bm->Render();
}

void EnemyManager::Release()
{
}

void EnemyManager::Hit(POINT mouse)
{
	bm->Assaulted(mouse);
}

void EnemyManager::SetAnim(int n)
{
	bm->SetAnim(n);
}