#include "stdafx.h"
#include "SkyBox.h"



SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

HRESULT SkyBox::Init(LPSTR fileName[SKY_TEXTURE])
{
	if (FAILED(CreateSkyBox())) return E_FAIL;
	SetTexture(fileName);
}

HRESULT SkyBox::CreateSkyBox()
{
	if (FAILED(D3DDEVICE->CreateVertexBuffer(24 * sizeof(SKYVERTEX), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_MANAGED, &vb, nullptr)))
	{
		return E_FAIL;
	}

	SKYVERTEX vertices[24] =
	{
		{ D3DXVECTOR3(-500.0f, 500.0f, -500.0f), D3DXVECTOR2(0.0f, 0.0f) },		// Top vertices
		{ D3DXVECTOR3(500.0f, 500.0f, -500.0f), D3DXVECTOR2( 1.0f, 0.0f) },
		{ D3DXVECTOR3(-500.0f, 500.0f,  500.0f), D3DXVECTOR2( 0.0f, 1.0f) },
		{ D3DXVECTOR3(500.0f, 500.0f,  500.0f), D3DXVECTOR2( 1.0f, 1.0f) },

		{ D3DXVECTOR3(-500.0f,  500.0f, 500.0f), D3DXVECTOR2( 0.0f, 0.0f) },		// Front vertices
		{ D3DXVECTOR3(500.0f,  500.0f, 500.0f), D3DXVECTOR2( 1.0f, 0.0f) },
		{ D3DXVECTOR3(-500.0f, -500.0f, 500.0f), D3DXVECTOR2( 0.0f, 1.0f) },
		{ D3DXVECTOR3(500.0f, -500.0f, 500.0f), D3DXVECTOR2( 1.0f, 1.0f) },

		{ D3DXVECTOR3(500.0f,  500.0f, -500.0f), D3DXVECTOR2( 0.0f, 0.0f) },		// Back vertices
		{ D3DXVECTOR3(-500.0f,  500.0f, -500.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(500.0f, -500.0f, -500.0f), D3DXVECTOR2( 0.0f, 1.0f) },
		{ D3DXVECTOR3(-500.0f, -500.0f, -500.0f), D3DXVECTOR2(1.0f, 1.0f) },
																		
		{ D3DXVECTOR3(-500.0f,  500.0f, -500.0f), D3DXVECTOR2(0.0f, 0.0f) },		// Left vertices
		{ D3DXVECTOR3(-500.0f,  500.0f,  500.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-500.0f, -500.0f, -500.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(-500.0f, -500.0f,  500.0f), D3DXVECTOR2(1.0f, 1.0f) },
																		
		{ D3DXVECTOR3(500.0f,  500.0f,  500.0f), D3DXVECTOR2( 0.0f, 0.0f) },		// Right vertices
		{ D3DXVECTOR3(500.0f,  500.0f, -500.0f), D3DXVECTOR2( 1.0f, 0.0f) },
		{ D3DXVECTOR3(500.0f, -500.0f,  500.0f), D3DXVECTOR2( 0.0f, 1.0f) },
		{ D3DXVECTOR3(500.0f, -500.0f, -500.0f), D3DXVECTOR2( 1.0f, 1.0f) },

		{ D3DXVECTOR3(-500.0f, -500.0f,  500.0f), D3DXVECTOR2(0.0f, 0.0f) },		// Bottom vertices
		{ D3DXVECTOR3(500.0f, -500.0f,  500.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR3(-500.0f, -500.0f, -500.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR3(500.0f, -500.0f, -500.0f), D3DXVECTOR2(1.0f, 1.0f) }
	};

	VOID* pVertices;
	vb->Lock(0, 24 * sizeof(SKYVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, vertices, 24 * sizeof(SKYVERTEX));
	vb->Unlock();
}

HRESULT SkyBox::SetTexture(LPSTR file[SKY_TEXTURE])
{
	for (int i = 0; i < SKY_TEXTURE; ++i)
	{
		textureList.push_back(g_pTextureManager->GetTexture(file[i]));
	}

	return S_OK;
}

HRESULT SkyBox::Destroy()
{
	if (FAILED(vb->Release())) return E_FAIL;

	return S_OK;
}

void SkyBox::Render()
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	D3DDEVICE->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(SKYVERTEX));
	for (int i = 0; i < SKY_TEXTURE; i++)
	{
		D3DDEVICE->SetTexture(0, textureList[i]);
		D3DDEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}
