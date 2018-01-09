#pragma once
#define EPSILON 0.0001f
#define HPMAX_BMONG	586
#define HPMAX_BOSS	1056

class iMap;
class SkinnedMesh;

enum ENEMYSPHERE
{
	SP_HEAD,	//0						�Ӹ� 
	SP_ASSAULT,	//1 assaulted range		�ǰ� ���ϴ� ���� 
	SP_RECOG,	//2 recognition range	�÷��̾ �ν��ϴ� ����
	SP_ATTACK,	//3 attack range		���� ���� ���� 
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
	D3DXVECTOR3					pos;				// ���� ��ġ
	D3DXVECTOR3					posTarget;			// �̵��� ��ġ 
	DWORD						curAnim;			// ���� AnimationSet
	float						curAngle;
	float						passedTime;			// ������ ȣ����� �ð�  
	float						actionTime;
	bool						isRecog;			// �÷��̾� �ν� ����  

	// �÷��̾���� �Ÿ�, ����
	float						distance;			
	float						angle;				

	// ����
	float						termAnimate;
	float						termWalk;

	// �ٸ� ���̶� �浹 ����
	D3DXVECTOR3					posOther;			// �ٸ� ���� ��ġ 
	bool						isColliOther;		// �ٸ� ���� �浹�ϴ°� 


	bool						showBSphere;		// boundingSphere ǥ�� ���� 
	float						direction;			// ���� ��ǥ������ ����(���� ����)

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

	// ��� Ŭ�������� �ʼ� ���� 
	virtual HRESULT Init(D3DXVECTOR3 position) = 0;
	virtual void	Update(iMap* imap, D3DXVECTOR3 posPlayer, SphereInfo* spherePlayer) = 0;
	virtual void	Render() = 0;
	virtual void	Release() = 0;


	void			GetNewPosition(D3DXVECTOR3* ptarget);
	void			SetNewTarget(D3DXVECTOR3* pEach, float radiusEach);	// ������ ���� 
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