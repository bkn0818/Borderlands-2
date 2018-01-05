#pragma once
#define HPMAX_BMONG	586
#define HPMAX_BOSS	1056

class iMap;
class SkinnedMesh;

enum ENEMYSTATE
{
	STATE_IDLE,		//0
	STATE_WALK,		//1
	STATE_RUN,		//2
	STATE_ATTACK,	//3
	STATE_DEATH,	//4
	STATE_END
};

enum ENEMYSPHERE
{
	SPHERE_HEAD,	//0						머리 
	SPHERE_ASSAULT,	//1 assaulted range		피격 당하는 범위 
	SPHERE_RECOG,	//2 recognition range	플레이어를 인식하는 범위
	SPHERE_ATTACK,	//3 attack range		근접 공격 범위 
	SPHERE_END
};

struct tagBoundingSphere
{
	LPD3DXMESH sphereMesh;
	D3DMATERIAL9 material;
	SphereInfo sphereInfo;
	bool show;
};


class Enemy
{
protected:
	SkinnedMesh*				sm;
	DWORD						hp;
	D3DXVECTOR3					pos;				// 현재 위치
	D3DXVECTOR3					posNext;			// 이동할 위치 
	float						direction;			// 최종 목표까지의 방향(라디안 기준)
	DWORD						curAnimSet;			// 현재 AnimationSet
	float						speed;				// speed
	float						speedWalk;
	float						speedRun;

	float						distance;			// 플레이어와의 간격

	float						moveInterval;		// 이동 간격
	float						passedActionTime;	// 위치 선형 보간 간격 

													// 다른 적이랑 충돌 판정
	D3DXVECTOR3					posOther;			// 충돌한 다른 적의 위치 
	bool						isColliEach;		// 다른 enemy와 충돌하는가 


	bool						isRecog;			// 플레이어를 인식했는가 
	bool						showBSphere;		// boundingSphere 표시 여부 true: 표시 


	tagBoundingSphere			stBSphere[SPHERE_END];	// boundingSphere들

	void		SetHeightMap( iMap* imap );
	void		SetBoundingSphere(	tagBoundingSphere& sphere, float radius,
									int r, int g, int b );
	bool		IsCollideSphere( D3DXVECTOR3* spCenter1, float spRadius1,
								 D3DXVECTOR3* spCenter2, float spRadius2 );
	void		IsRecogPlayer( D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer );


public:
	Enemy() {}
	~Enemy() {}

	// 상속 클래스에서 필수 구현 
	virtual HRESULT Init(D3DXVECTOR3 position) = 0;
	virtual void	Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer) = 0;
	virtual void	Render() = 0;
	virtual void	Release() = 0;

	void			GetNewPosition(D3DXVECTOR3* ptarget);
	void			SetNewTarget(D3DXVECTOR3* pEach, float radiusEach);	// 목적지 셋팅 
	void			ComputeDirection();
	//	virtual void SetSeekingState();	// 목적지 선택, IDLE or WALK 선택 

	// get set 
	inline D3DXVECTOR3* GetPos() {
		return &pos;
	}
	inline float GetRadi() {
		return stBSphere[SPHERE_ASSAULT].sphereInfo.fRadius;
	}
	inline void SetColliEach(bool colli) {
		isColliEach = colli;
	}
};