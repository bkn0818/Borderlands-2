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

	//�Ÿ��� ���� ��ƼŬ�� ũ�� ����
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	D3DDEVICE->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	//�ؽ�ó�� ���� �̿��ϱ�
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

		//���ؽ� ���۸� ����� ���� ����
		if (vbOffset >= vbSize) vbOffset = 0;

		Particle* v = 0;
		vb->Lock(vbOffset * sizeof(Particle),
				 vbBatchSize * sizeof(Particle),
				 (LPVOID*)&v,
				 vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch = 0;

		//��� ��ƼŬ�� ������ �ɶ�����
		list<Attribute>::iterator iter = particles.begin();

		for (iter; iter != particles.end(); ++iter)
		{
			if (iter->isAlive)
			{
				//�� �ܰ��� ������ ��ƼŬ�� ���� ���ؽ� ���� ���׸�Ʈ�� ����
				v->position = iter->position;
				v->color = (D3DCOLOR)iter->color;
				v++;
				numParticlesInBatch++; //�ܰ� ī���� ����

				//���� �ܰ谡 ��� ä���� �ִ���
				if (numParticlesInBatch == vbBatchSize)
				{
					vb->Unlock();
					D3DDEVICE->DrawPrimitive(D3DPT_POINTLIST,
											 vbOffset,
											 vbBatchSize);
					//���� �ܰ谡 �׷����� ���� ���� �ܰ踦 ��ƼŬ�� ä���
					//���� �ܰ��� ó�� ���������� �̵�

					vbOffset += vbBatchSize;
					//���ؽ� ������ ��踦 �Ѵ� �޸𸮷� �������� �ʴ´�
					//��踦 ���� ��� ���� ����
					if (vbOffset >= vbSize) vbOffset = 0;

					vb->Lock(vbOffset * sizeof(Particle),
							 vbBatchSize * sizeof(Particle),
							 (LPVOID*)&v,
							 vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

					//���� �ܰ踦 ���� ����
					numParticlesInBatch = 0;
				}
			}
		}
		vb->Unlock();
		//(numParticlesInBatch == vbBatchSize) ������ �������� ���ؼ�
		//������ �ܰ谡 ���������� �ʴ� ��찡 �߻��� �� �ִµ� �ٷ� �̰����� �����ȴ�
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
