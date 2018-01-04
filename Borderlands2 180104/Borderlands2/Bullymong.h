#pragma once
#include "Enemy.h"
#define PI 3.141592f

// bullymong의 animation들
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
	float					angle;		// 물체가 움직이기 시작할 지점과 플레이어 지점과의 각도 
	float					passedTime;	// 움직인 시간 
	float					throwTime;	// 보간 타이밍 
	float					nowY;		// 지금 y축 위치 

										// 던져진 물체의 궤도 파악을 위해. 
	std::vector<VertexPC>	posList;
};


class Bullymong : public Enemy
{
private:
	tagThrowObj				objRock;
	float					passedTime;
	bool					isThrow;
	bool					throwStart;


	int count;
	DWORD animFrame[BMONG_ANIM_END];

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


	void		Move(D3DXVECTOR3 posPlayer);
	void		ThrowObj(D3DXVECTOR3 posPlayer);


	void Attack();

	void Assaulted(POINT mouse);

	void ShowBSphere();

};