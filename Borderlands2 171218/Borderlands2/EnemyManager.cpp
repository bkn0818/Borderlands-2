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
	bm->Init(D3DXVECTOR3(50.0f, 0.0f, 0.0f));

	return S_OK;
}

void EnemyManager::Update(D3DXVECTOR3* playerPosition)
{
	D3DXVECTOR3 playerPos = *playerPosition;
	bm->Update(playerPos);
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