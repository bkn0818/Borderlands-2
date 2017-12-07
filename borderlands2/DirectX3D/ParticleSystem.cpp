#include "stdafx.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Init(char * fileName)
{
	HRESULT hr;
	hr = D3DDEVICE->CreateVertexBuffer(vbSize * sizeof(Particle),
								  D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
								  Particle::FVF,
								  D3DPOOL_DEFAULT,
								  &vb,
								  0);
	if (FAILED(hr))
	{
		MessageBox(g_HWND_Window, L"Failed to create vertex buffer", L"Particle", 0);
		return false;
	}

	tex = g_pTextureManager->GetTexture(fileName);
	if (tex == nullptr)
	{
		MessageBox(g_HWND_Window, L"Failed to Create Texture", L"texture", 0);
		return false;
	}
	return true;
}

void ParticleSystem::Reset()
{
	list<Attribute>::iterator iter; particles.begin();

	for (iter; iter != particles.end(); ++iter)
	{
		ResetParticle(&(*iter));
	}
}

void ParticleSystem::AddParticle()
{
	Attribute att;

	ResetParticle(&att);

	particles.push_back(att);
}

void ParticleSystem::PreRender()
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	D3DDEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	D3DDEVICE->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(size));
	D3DDEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));

	//거리에 따른 파티클의 크기 제어
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	//텍스처의 알파 이용하기
	D3DDEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	D3DDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	D3DDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3DDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void ParticleSystem::Render()
{
	if (!particles.empty())
	{
		PreRender();

		D3DDEVICE->SetTexture(0, tex);
		D3DDEVICE->SetFVF(Particle::FVF);
		D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(Particle));

		//버텍스 버퍼를 벗어나면 새로 시작
		if (vbOffset >= vbSize) vbOffset = 0;

		Particle* v = 0;
		vb->Lock(vbOffset * sizeof(Particle),
				 vbBatchSize * sizeof(Particle),
				 (LPVOID*)&v,
				 vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//모든 파티클이 렌더링 될때까지
		list<Attribute>::iterator iter = particles.begin();

		for (iter; iter != particles.end(); ++iter)
		{
			if (iter->isAlive)
			{
				//한 단계의 생존한 파티클을 다음 버텍스 버퍼 세그먼트로 복사
				v->position = iter->position;
				v->color = (D3DCOLOR)iter->color;
				v++;
				numParticlesInBatch++; //단계 카운터 증가

				//현재 단계가 모두 채워져 있는지
				if (numParticlesInBatch == vbBatchSize)
				{
					vb->Unlock();
					D3DDEVICE->DrawPrimitive(D3DPT_POINTLIST,
											 vbOffset,
											 vbBatchSize);
					//현재 단계가 그려지는 동안 다음 단계를 파티클로 채운다
					//다음 단계의 처음 오프셋으로 이동

					vbOffset += vbBatchSize;
					//버텍스 버퍼의 경계를 넘는 메모리로 설정하지 않는다
					//경계를 넘을 경우 새로 시작
					if (vbOffset >= vbSize) vbOffset = 0;

					vb->Lock(vbOffset * sizeof(Particle),
							 vbBatchSize * sizeof(Particle),
							 (LPVOID*)&v,
							 vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					//다음 단계를 위한 리셋
					numParticlesInBatch = 0;
				}
			}
		}
		vb->Unlock();
		//(numParticlesInBatch == vbBatchSize) 조건이 만족되지 못해서
		//마지막 단계가 렌더링되지 않는 경우가 발생할 수 있는데 바로 이곳에서 렌더된다
		if (numParticlesInBatch)
		{
			D3DDEVICE->DrawPrimitive(D3DPT_POINTLIST,
									 vbOffset,
									 numParticlesInBatch);
		}

		vbOffset += vbBatchSize;

		PostRender();
	}
}

void ParticleSystem::PostRender()
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING,			true);
	D3DDEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE,	false);
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALEENABLE,	false);
	D3DDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
}

bool ParticleSystem::IsEmpty()
{
	return false;
}

bool ParticleSystem::IsDead()
{
	return false;
}

void ParticleSystem::RemoveDeadParticles()
{
	list<Attribute>::iterator iter = particles.begin();

	while (iter != particles.end())
	{
		if (!iter->isAlive)
		{
			iter = particles.erase(iter);
		}
		else iter++;
	}
}
