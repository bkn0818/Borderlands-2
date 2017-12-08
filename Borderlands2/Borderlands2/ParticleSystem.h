#pragma once
#include <list>

using namespace std;

struct Particle
{
	D3DXVECTOR3 position;
	D3DCOLOR	color;
	float		size;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE, };
};

struct Attribute
{
	Attribute()
	{
		life	= 0.0f;
		age		= 0.0f;	
		isAlive = true;
	}
	D3DXVECTOR3 position;		//���彺���̽� ���� ��ƼŬ ��ġ
	D3DXVECTOR3 velocity;		//��ƼŬ�� �ʴ� �̵��ӵ�
	D3DXVECTOR3 acceleration;	//��ƼŬ�� �ʴ� ���ӷ�
	D3DXCOLOR	color;			//��ƼŬ�� ����
	D3DXCOLOR	colorFade;		//��ƼŬ�� �ð��� �����鼭 �ٲ� ��
	float		life;			//�Ҹ��� ������ ������ �ð�
	float		age;			//��ƼŬ�� ����
	bool		isAlive;		//���� ����
};

struct BoundingBox
{
	D3DXVECTOR3 min, max;
	BoundingBox()
	{
		min.x = -INFINITY;
		min.y = -INFINITY;
		min.z = -INFINITY;
		max.x = INFINITY;
		max.y = INFINITY;
		max.z = INFINITY;
	}

	bool IsPointInside(D3DXVECTOR3& p)
	{
		if (p.x >= min.x && p.y >= min.y && p.z >= min.z &&
			p.x <= max.x && p.y <= max.y && p.z <= max.z)
			return true;
		else return false;
	}
};

class ParticleSystem
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual bool Init(char* fileName);
	virtual void Reset();
	virtual void ResetParticle(Attribute* att) = 0;
	virtual void AddParticle();
	virtual void Update(float deltaTime) = 0;
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	bool IsEmpty();
	bool IsDead();

protected:
	virtual void RemoveDeadParticles();

protected:
	D3DXVECTOR3				origin;				//��ƼŬ ���� ��ġ
	BoundingBox				boundingBox;	//�̵��� �� �ִ� ����
	float					emitRate;			//���ο� ��ƼŬ �߰� ����(�ʴ� ��ƼŬ ��)
	float					size;				//��� ��ƼŬ�� ũ��
	int						maxParticles;		//�ִ� ��ƼŬ ����
	LPDIRECT3DTEXTURE9		tex;				//��ƼŬ �ؽ�ó
	LPDIRECT3DVERTEXBUFFER9 vb;					//��ƼŬ ���ؽ� ����
	std::list<Attribute>	particles;			//��ƼŬ �Ӽ� ����Ʈ
												
	DWORD					vbSize;				
	DWORD					vbOffset;			
	DWORD					vbBatchSize;		

};

