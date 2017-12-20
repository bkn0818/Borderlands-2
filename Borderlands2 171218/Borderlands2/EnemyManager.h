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

	void Hit(POINT mouse);	// �߻����� ���� ���콺 ��ǥ ���� 

							// �׽�Ʈ��
	void SetAnim(int n);

};