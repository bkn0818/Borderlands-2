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
	D3DXVECTOR3 position;		//월드스페이스 내의 파티클 위치
	D3DXVECTOR3 velocity;		//파티클의 초당 이동속도
	D3DXVECTOR3 acceleration;	//파티클의 초당 가속력
	D3DXCOLOR	color;			//파티클의 색상
	D3DXCOLOR	colorFade;		//파티클이 시간이 지나면서 바뀔 색
	float		life;			//소멸할 때까지 유지될 시간
	float		age;			//파티클의 나이
	bool		isAlive;		//생사 유무
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
	D3DXVECTOR3				origin;				//파티클 생성 위치
	BoundingBox				boundingBox;	//이동할 수 있는 영역
	float					emitRate;			//새로운 파티클 추가 비율(초당 파티클 수)
	float					size;				//모든 파티클의 크기
	int						maxParticles;		//최대 파티클 개수
	LPDIRECT3DTEXTURE9		tex;				//파티클 텍스처
	LPDIRECT3DVERTEXBUFFER9 vb;					//파티클 버텍스 버퍼
	std::list<Attribute>	particles;			//파티클 속성 리스트
												
	DWORD					vbSize;				
	DWORD					vbOffset;			
	DWORD					vbBatchSize;		

};

