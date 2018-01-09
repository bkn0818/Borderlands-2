#pragma once
#include "Enemy.h"
#define THROWSPEED 0.25f
#define THROWGRAVITY 0.006f
#define ANIMATETERM 4.0f
#define MOVESPEED  0.05f

// bullymong의 animation
enum ANIMSET_BULLYMONG
{
	BMONG_RUN_FRONT,	// 0
	BMONG_WALK,			// 1
	BMONG_IDLE,			// 2
	BMONG_HIT,			// 3
	BMONG_DEATH,		// 4
	BMONG_THROWROCK,	// 5
	BMONG_PUNCH_LEFT,	// 6
	BMONG_PUNCH_RIGHT,	// 7
	BMONG_ANIM_END
};

// 던질 물체 
struct tagThrowObj
{
	SkinnedMesh*			sm;
	D3DMATERIAL9			material;
	LPD3DXMESH				mesh;
	D3DXVECTOR3				pos;		// 물체의 현재 위치 
	D3DXVECTOR3				posTarget;	// 물체가 이동할 다음 위치 
	D3DXVECTOR3				posLerp;	// 보간용 위치 
	float					gravity;	// 물체에 가해질 중력 
	float					passedTime;	// 움직인 시간 
	float					actionTime;	// 보간 term 
	float					nowY;		// 지금 y축 위치 

	// 던져진 물체의 궤도 파악을 위해. 
	std::vector<VertexPC>	posList;
	float					angle;		// 물체가 움직이기 시작할 지점과 플레이어 지점과의 각도 
};


class Bullymong : public Enemy
{
private:
	tagThrowObj				objRock;
	bool					isAttack;	// 공격 상태
	bool					isMove;		// 이동 상태 
	bool					isIdle;		// 대기 상태 
	bool					throwStart; // 던지는 중 
	DWORD					dirMove;


	// 테스트용 ========
	float r1, r2;
	float px, py, pz;

public:
	Bullymong();
	~Bullymong();

	HRESULT		Init(D3DXVECTOR3 position);
	void		Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer);
	void		Render();
	void		Release();

	void		SetState(D3DXVECTOR3* posPlayer);
	void		Move(D3DXVECTOR3* posPlayer);
	void		ThrowObj(D3DXVECTOR3* posPlayer);


	void Attack();
	void Assaulted(POINT mouse);
	void ShowBSphere();

};