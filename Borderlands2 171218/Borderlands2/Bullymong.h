#pragma once
#include "Enemy.h"

enum tagBullymongAnimList
{
	BMONG_RUN_FRONT,
	BMONG_WALK,
	BMONG_IDLE,
	BMONG_HIT,
	BMONG_DEATH,
	BMONG_THROWROCK,
	BMONG_PUNCH_LEFT,
	BMONG_PUNCH_RIGHT,
	BMONG_ANIM_END
};

class Bullymong : public Enemy
{
private:

public:
	Bullymong();
	~Bullymong();

	HRESULT Init(D3DXVECTOR3 position);
	void Update(D3DXVECTOR3 position);
	void Render();
	void Release();

	//	void Collision(SphereInfo* sphere);
	void Collision(D3DXVECTOR3 position);
	void SetState(int curAnim);

	void Assaulted(POINT mouse);
	void Move(D3DXVECTOR3 playerPos);

	void ShowRange();

	// 테스트용
	void SetAnim(int n);
};