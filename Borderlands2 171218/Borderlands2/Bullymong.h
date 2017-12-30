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

// ���� ��ü 
struct tagThrowObj
{
	LPD3DXMESH mesh;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR3 lerpPos;	// ������ 
	D3DXVECTOR3 pos;
	float gravity;
	float angle; // ���� ������ �÷��̾� �������� ���� 
	float passedTime;
	float throwTime;
	float nowY;				// ���� y�� ��ġ 
	std::vector<VertexPC> posList;
};

class Bullymong : public Enemy
{
private:
	// �׽�Ʈ�� ========
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

	// �׽�Ʈ��
	void SetAnim(int n);
};