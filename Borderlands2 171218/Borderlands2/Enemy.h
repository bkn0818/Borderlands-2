#pragma once
#include "SkinnedMesh.h"

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
	RANGE_HEAD,		//0 머리 
	RANGE_ATTACK,	//1 attack range 몬스터의 공격 범위
	RANGE_RECOG,	//2 recognition range 플레이어를 인식하는 범위
	RANGE_ASSAULT,	//3 assaulted range 피격 당하는 범위 
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
	tagBoundingSphere			range[RANGE_END];	// 범위들 
	D3DXVECTOR3					pos;				// 위치
	int							curAnimSet;
	int							state;
	int							hp;

	// 위치 선형 보간====================================
	D3DXMATRIXA16				worldMT;
	std::vector<D3DXVECTOR3>	posList;			// 이동할 위치들
	D3DXVECTOR3					prevPos;
	D3DXVECTOR3					targetPos;			// 최종 위치
	float						moveInterval;		// 이동 간격
	float						passedActionTime;
	// ==================================================

	bool						haveTarget;			// 인식했다면 공격모드로 타겟 지정 
	bool						showRange;

	void SetBoundingSphere(tagBoundingSphere& sphere, float radius, int r, int g, int b);
	bool CollisionSphere(D3DXVECTOR3 spCenter1, float spRadius1, D3DXVECTOR3 spCenter2, float spRadius2);


public:
	Enemy() {}
	~Enemy() {}

	virtual HRESULT Init(D3DXVECTOR3 position) = 0;
	virtual void Update(D3DXVECTOR3 position) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

};