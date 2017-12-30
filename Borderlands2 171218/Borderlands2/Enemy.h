#pragma once
#define HPMAX_BMONG 586

class iMap;
class SkinnedMesh;

enum tagEnemyState
{
	STATE_IDLE,		//1
	STATE_WALK,		//2
	STATE_RUN,		//3
	STATE_ATTACK,	//4
	STATE_DEATH,	//5
	STATE_END
};

enum tagEnemyRange
{
	RANGE_HEAD,		//0 �Ӹ� 
	RANGE_ATTACK,	//1 attack range ������ ���� ����
	RANGE_RECOG,	//2 recognition range �÷��̾ �ν��ϴ� ����
	RANGE_ASSAULT,	//3 assaulted range �ǰ� ���ϴ� ���� 
	RANGE_END
};

struct tagBoundingSphere
{
	LPD3DXMESH sphereMesh;
	D3DMATERIAL9 material;
	SphereInfo sphereInfo;
};

class Enemy
{
protected:
	SkinnedMesh*				sm;
	tagBoundingSphere			range[RANGE_END];	// ������ 
	D3DXVECTOR3					pos;				// ��ġ
	int							curAnimSet;
	int							state;
	int							hp;
	float						distance;			// �÷��̾���� ����

	// ��ġ ���� ����====================================
	D3DXVECTOR3					targetPos;			// ���� ��ġ
	float						moveInterval;		// �̵� ����
	float						passedActionTime;
	// ==================================================

	bool						isRecog;			// �ν��ߴٸ� ���� 
	bool						isMoveStart;		// �����̱� �����ߴ°� 

	bool						showRange;			// boundingSphere ǥ�� ���� 

	void SetBoundingSphere(tagBoundingSphere& sphere, float radius, int r, int g, int b);
	bool CollisionSphere(D3DXVECTOR3 spCenter1, float spRadius1, D3DXVECTOR3 spCenter2, float spRadius2);


public:
	Enemy() {}
	~Enemy() {}

	virtual HRESULT Init(D3DXVECTOR3 position) = 0;
	virtual void Update(iMap* obj, D3DXVECTOR3 playerPosition, SphereInfo* playerSphere) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

};