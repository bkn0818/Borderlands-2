#pragma once
#define EPSILON 0.0001f
#define HPMAX_BMONG	586
#define HPMAX_BOSS	1056

class iMap;
class SkinnedMesh;

enum ENEMYSPHERE
{
	SP_HEAD,	//0						머리 
	SP_ASSAULT,	//1 assaulted range		피격 당하는 범위 
	SP_RECOG,	//2 recognition range	플레이어를 인식하는 범위
	SP_ATTACK,	//3 attack range		근접 공격 범위 
	SP_END
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
	D3DXVECTOR3					posTarget;			// 이동할 위치 
	DWORD						curAnim;			// 현재 AnimationSet
	float						curAngle;
	float						passedTime;			// 마지막 호출까지 시간  
	float						actionTime;
	bool						isRecog;			// 플레이어 인식 여부  

	// 플레이어와의 거리, 각도
	float						distance;			
	float						angle;				

	// 간격
	float						termAnimate;
	float						termWalk;

	// 다른 적이랑 충돌 판정
	D3DXVECTOR3					posOther;			// 다른 적의 위치 
	bool						isColliOther;		// 다른 적과 충돌하는가 


	bool						showBSphere;		// boundingSphere 표시 여부 
	float						direction;			// 최종 목표까지의 방향(라디안 기준)

	tagBoundingSphere			stBSphere[SP_END];	// bounding Sphere

	void			SetHeightMap( iMap* imap );
	void			SetBoundingSphere(	tagBoundingSphere& sphere, float radius,
									int r, int g, int b );
	bool			IsCollideSphere( D3DXVECTOR3* spCenter1, float spRadius1,
								 D3DXVECTOR3* spCenter2, float spRadius2 );
	void			IsRecogPlayer( D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer );


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
	void			Assaulted(int damage);


	// get set 
	inline D3DXVECTOR3* GetPos() {
		return &pos;
	}
	inline float GetRadi() {
		return stBSphere[SP_ASSAULT].sphereInfo.fRadius;
	}
	inline void ColliOther(bool colli) {
		isColliOther = colli;
	}
};