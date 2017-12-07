#include "stdafx.h"
#include "Group.h"


Group::Group()
	: materialTexture(nullptr)
	, vb(nullptr)
{
}


Group::~Group()
{
	SAFE_RELEASE(materialTexture);
}

void Group::SetVertexBuffer()
{
	D3DDEVICE->CreateVertexBuffer(PNTVerts.size() * sizeof(VertexPNT), 0, VertexPNT::FVF, D3DPOOL_MANAGED, &vb, 0);
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPNT));

	VertexPNT* vertices;
	vb->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < PNTVerts.size(); i++)
	{
		vertices[i] = PNTVerts[i];
	}

	vb->Unlock();
}

void Group::Render()
{
	DWORD dwFillMode;
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	D3DDEVICE->GetRenderState(D3DRS_FILLMODE, &dwFillMode);
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPNT));
	if (dwFillMode == D3DFILL_WIREFRAME)
	{
		D3DDEVICE->SetTexture(0, NULL);
	}
	else
	{
		D3DDEVICE->SetTexture(0, materialTexture->GetTexture());
	}
	
	D3DDEVICE->SetMaterial(&materialTexture->GetMaterial());
	D3DDEVICE->SetFVF(VertexPNT::FVF);
	D3DDEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, PNTVerts.size() / 3);
}

