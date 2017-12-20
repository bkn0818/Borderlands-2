#pragma once
#include "Bullymong.h"
#include "Boss.h"

class iMap;

class EnemyManager
{
private:
	Bullymong* bm;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init();
	void Update(iMap* obj, D3DXVECTOR3 playerPosition);	
	void Render();
	void Release();

	void Hit(POINT mouse);	// 발사했을 때의 마우스 좌표 전달 

							// 테스트용
	void SetAnim(int n);

};