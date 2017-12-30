#pragma once
#include "Enemy.h"
#define PI 3.141592f

enum ANIMLIST_BULLYMONG
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

// 던질 물체 
struct tagThrowObj
{
	LPD3DXMESH mesh;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR3 lerpPos;	// 보간용 
	D3DXVECTOR3 pos;
	float gravity;
	float angle; // 시작 지점과 플레이어 지점과의 각도 
	float passedTime;
	float throwTime;
	float nowY;				// 지금 y축 위치 
	std::vector<VertexPC> posList;
};

class Bullymong : public Enemy
{
private:
	// 테스트용 ========
	float r1, r2;
	float px, py, pz;
	// =================

	tagThrowObj rockObj;
	bool isThrow;
	int count;
	bool throwStart;

public:
	Bullymong();
	~Bullymong();

	HRESULT Init(D3DXVECTOR3 position);
	void Update(iMap* obj, D3DXVECTOR3 playerPosition, SphereInfo* playerSphere);
	void Render();
	void Release();

	void Collision(D3DXVECTOR3 position, SphereInfo* playerSphere);
	void Move(D3DXVECTOR3 playerPosition);
	void Attack();

	void Assaulted(POINT mouse);

	void ShowRange();

	// 테스트용
	void SetAnim(int n);
};