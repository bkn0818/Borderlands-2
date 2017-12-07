#include "stdafx.h"
#include "Pyramid.h"


Pyramid::Pyramid()
{
}


Pyramid::~Pyramid()
{
}

void Pyramid::Init(D3DCOLOR color, D3DXMATRIX worldMat)
{
	worldMatrix = worldMat;
	std::vector<D3DXVECTOR3> vList;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	material.Ambient = material.Ambient = material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vList.push_back(D3DXVECTOR3(0, 0, 0)); vList.push_back(D3DXVECTOR3(1, -1, -1)); vList.push_back(D3DXVECTOR3(-1, -1, -1));
	vList.push_back(D3DXVECTOR3(0, 0, 0)); vList.push_back(D3DXVECTOR3(1, -1, 1)); vList.push_back(D3DXVECTOR3(1, -1, -1));
	vList.push_back(D3DXVECTOR3(0, 0, 0)); vList.push_back(D3DXVECTOR3(-1, -1, 1));	vList.push_back(D3DXVECTOR3(1, -1, 1));

	vList.push_back(D3DXVECTOR3(0, 0, 0)); vList.push_back(D3DXVECTOR3(-1, -1, -1)); vList.push_back(D3DXVECTOR3(-1, -1, 1));
	vList.push_back(D3DXVECTOR3(1, -1, -1)); vList.push_back(D3DXVECTOR3(-1, -1, 1)); vList.push_back(D3DXVECTOR3(-1, -1, -1));
	vList.push_back(D3DXVECTOR3(1, -1, -1)); vList.push_back(D3DXVECTOR3(1, -1, 1)); vList.push_back(D3DXVECTOR3(-1, -1, 1));

	for (int i = 0; i < 18; i += 3)
	{
		D3DXVECTOR3 direction;

		D3DXVECTOR3 u = vList[i + 1] - vList[i];
		D3DXVECTOR3 v = vList[i + 2] - vList[i];
		D3DXVec3Cross(&direction, &u, &v);
		D3DXVec3Normalize(&direction, &direction);

		vertexPNCList.push_back(VertexPNC(vList[i], direction, color));
		vertexPNCList.push_back(VertexPNC(vList[i + 1], direction, color));
		vertexPNCList.push_back(VertexPNC(vList[i + 2], direction, color));
	}

	D3DDEVICE->CreateVertexBuffer(vertexPNCList.size() * sizeof(VertexPNC), 0, VertexPNC::FVF, D3DPOOL_MANAGED, &vb, 0);

	VertexPNC* vertices;

	vb->Lock(0, 0, (void**)&vertices, 0);
	for (int i = 0; i < vertexPNCList.size(); ++i)
	{
		vertices[i] = vertexPNCList[i];
	}
	vb->Unlock();
}

void Pyramid::Render()
{
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPNC));
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMatrix);
	D3DDEVICE->SetMaterial(&material);
	D3DDEVICE->SetFVF(VertexPNC::FVF);
	D3DDEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexPNCList.size());
}

void Pyramid::SetMaterialColor(D3DXCOLOR c)
{
	material.Ambient = material.Diffuse = material.Specular = c;
}
