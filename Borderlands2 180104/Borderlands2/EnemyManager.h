#pragma once
#include "Bullymong.h"
#include "Boss.h"
#define MAX_ENEMY 2

class iMap;

class EnemyManager
{
private:
//	Bullymong*		bm[MAX_ENEMY];
	Bullymong*		bm;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT		Init();
	void		Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer);
	void		Render();
	void		Release();

	void		SetNewPosition();
	bool		CollisionEach(D3DXVECTOR3* posEnemy, float radiusEnemy);
	void		EnemyCollision();
	void		ObjectCollision();

};