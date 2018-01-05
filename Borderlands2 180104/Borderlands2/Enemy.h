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
	SPHERE_HEAD,	//0						�Ӹ� 
	SPHERE_ASSAULT,	//1 assaulted range		�ǰ� ���ϴ� ���� 
	SPHERE_RECOG,	//2 recognition range	�÷��̾ �ν��ϴ� ����
	SPHERE_ATTACK,	//3 attack range		���� ���� ���� 
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
	D3DXVECTOR3					pos;				// ���� ��ġ
	D3DXVECTOR3					posNext;			// �̵��� ��ġ 
	float						direction;			// ���� ��ǥ������ ����(���� ����)
	DWORD						curAnimSet;			// ���� AnimationSet
	float						speed;				// speed
	float						speedWalk;
	float						speedRun;

	float						distance;			// �÷��̾���� ����

	float						moveInterval;		// �̵� ����
	float						passedActionTime;	// ��ġ ���� ���� ���� 

													// �ٸ� ���̶� �浹 ����
	D3DXVECTOR3					posOther;			// �浹�� �ٸ� ���� ��ġ 
	bool						isColliEach;		// �ٸ� enemy�� �浹�ϴ°� 


	bool						isRecog;			// �÷��̾ �ν��ߴ°� 
	bool						showBSphere;		// boundingSphere ǥ�� ���� true: ǥ�� 


	tagBoundingSphere			stBSphere[SPHERE_END];	// boundingSphere��

	void		SetHeightMap( iMap* imap );
	void		SetBoundingSphere(	tagBoundingSphere& sphere, float radius,
									int r, int g, int b );
	bool		IsCollideSphere( D3DXVECTOR3* spCenter1, float spRadius1,
								 D3DXVECTOR3* spCenter2, float spRadius2 );
	void		IsRecogPlayer( D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer );


public:
	Enemy() {}
	~Enemy() {}

	// ��� Ŭ�������� �ʼ� ���� 
	virtual HRESULT Init(D3DXVECTOR3 position) = 0;
	virtual void	Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer) = 0;
	virtual void	Render() = 0;
	virtual void	Release() = 0;

	void			GetNewPosition(D3DXVECTOR3* ptarget);
	void			SetNewTarget(D3DXVECTOR3* pEach, float radiusEach);	// ������ ���� 
	void			ComputeDirection();
	//	virtual void SetSeekingState();	// ������ ����, IDLE or WALK ���� 

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