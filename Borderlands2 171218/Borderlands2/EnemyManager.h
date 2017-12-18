#pragma once
#include "Bullymong.h"
#include "Boss.h"

class EnemyManager
{
private:
	Bullymong* bm;

public:
	EnemyManager();
	~EnemyManager();

	HRESULT Init();
	void Update(D3DXVECTOR3* playerPosition);
	void Render();
	void Release();

	void Hit(POINT mouse);	// �߻����� ���� ���콺 ��ǥ ���� 

							// �׽�Ʈ��
	void SetAnim(int n);

};