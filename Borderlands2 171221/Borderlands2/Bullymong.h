#pragma once
#include "Enemy.h"
#define PI 3.141592f

// bullymong�� animation��
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

// ���� ��ü 
struct tagThrowObj
{
	SkinnedMesh*			sm;
	D3DMATERIAL9			material;
	LPD3DXMESH				mesh;
	D3DXVECTOR3				pos;		// ��ü�� ���� ��ġ 
	D3DXVECTOR3				posTarget;	// ��ü�� �̵��� ���� ��ġ 
	D3DXVECTOR3				posLerp;	// ������ ��ġ 
	float					gravity;	// ��ü�� ������ �߷� 
	float					angle;		// ��ü�� �����̱� ������ ������ �÷��̾� �������� ���� 
	float					passedTime;	// ������ �ð� 
	float					throwTime;	// ���� Ÿ�̹� 
	float					nowY;		// ���� y�� ��ġ 

										// ������ ��ü�� �˵� �ľ��� ����. 
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

	// �׽�Ʈ�� ========
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