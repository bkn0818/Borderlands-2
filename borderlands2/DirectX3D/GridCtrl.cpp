#include "stdafx.h"

#include "Pyramid.h"

#include "GridCtrl.h"


GridCtrl::GridCtrl()
	:vb(nullptr)
{
	D3DXMatrixIdentity(&worldMatrix);
}


GridCtrl::~GridCtrl()
{
}

void GridCtrl::Init()
{
	int lineCount = 300;
	float interval = 1.0f;
	float max = lineCount * interval;
	D3DCOLOR color;

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	VertexPC* vertices;

	color = D3DCOLOR_XRGB(255, 0, 0);
	vertexList.push_back(VertexPC(D3DXVECTOR3(-max, 0.0f, 0.0f), D3DCOLOR(color)));
	vertexList.push_back(VertexPC(D3DXVECTOR3(max, 0.0f, 0.0f), D3DCOLOR(color)));

	color = D3DCOLOR_XRGB(0, 255, 0);
	vertexList.push_back(VertexPC(D3DXVECTOR3(0.0f, -max, 0.0f), D3DCOLOR(color)));
	vertexList.push_back(VertexPC(D3DXVECTOR3(0.0f, max, 0.0f), D3DCOLOR(color)));

	color = D3DCOLOR_XRGB(0, 0, 255);
	vertexList.push_back(VertexPC(D3DXVECTOR3(0.0f, 0.0f, -max), D3DCOLOR(color)));
	vertexList.push_back(VertexPC(D3DXVECTOR3(0.0f, 0.0f, max), D3DCOLOR(color)));

	for (int i = 1; i <= lineCount; ++i) {

		color = (i % 5 == 0) ? D3DCOLOR_XRGB(255, 255, 255) : D3DCOLOR_XRGB(128, 128, 128);
		vertexList.push_back(VertexPC(D3DXVECTOR3(-max, 0.0f, i), D3DCOLOR(color)));
		vertexList.push_back(VertexPC(D3DXVECTOR3(max, 0.0f, i), D3DCOLOR(color)));

		vertexList.push_back(VertexPC(D3DXVECTOR3(-max, 0.0f, -i), D3DCOLOR(color)));
		vertexList.push_back(VertexPC(D3DXVECTOR3(max, 0.0f, -i), D3DCOLOR(color)));

		vertexList.push_back(VertexPC(D3DXVECTOR3(i, 0.0f, -max), D3DCOLOR(color)));
		vertexList.push_back(VertexPC(D3DXVECTOR3(i, 0.0f, max), D3DCOLOR(color)));

		vertexList.push_back(VertexPC(D3DXVECTOR3(-i, 0.0f, -max), D3DCOLOR(color)));
		vertexList.push_back(VertexPC(D3DXVECTOR3(-i, 0.0f, max), D3DCOLOR(color)));
	}

	for (int i = 0; i < vertexList.size(); ++i)
	{
		D3DXVec3TransformCoord(&vertexList[i].p, &vertexList[i].p, &worldMatrix);
	}

	D3DDEVICE->CreateVertexBuffer(vertexList.size() * sizeof(VertexPC), 0, VertexPC::FVF, D3DPOOL_MANAGED, &vb, 0);
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPC));

	vb->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < vertexList.size(); ++i)
	{
		vertices[i] = vertexList[i];
	}

	vb->Unlock();

	D3DXMATRIXA16 matS, matR, matWorld;
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);

	Pyramid* pyramid;

	pyramid = new Pyramid;
	D3DXMatrixRotationZ(&matWorld, D3DX_PI / 2.0f);
	matWorld = matS * matWorld;
	pyramid->Init(D3DCOLOR_XRGB(255, 0, 0), matWorld);
	pyramid->SetMaterialColor(RED);
	pyramidList.push_back(pyramid);

	pyramid = new Pyramid;
	D3DXMatrixRotationX(&matWorld, D3DX_PI);
	matWorld = matS * matWorld;
	pyramid->Init(D3DCOLOR_XRGB(0, 255, 0), matWorld);
	pyramid->SetMaterialColor(GREEN);
	pyramidList.push_back(pyramid);

	pyramid = new Pyramid;
	D3DXMatrixRotationX(&matWorld, -D3DX_PI / 2.0f);
	matWorld = matS * matWorld;
	pyramid->Init(D3DCOLOR_XRGB(0, 0, 255), matWorld);
	pyramid->SetMaterialColor(BLUE);
	pyramidList.push_back(pyramid);
}

void GridCtrl::Destroy()
{
	vb->Release();
	for (int i = 0; i < pyramidList.size(); ++i) {
		delete (pyramidList[i]);
	}
	pyramidList.clear();
}

void GridCtrl::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &mat);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
	D3DDEVICE->SetFVF(VertexPC::FVF);
	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPC));
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 0, sizeof(vertexList) * sizeof(vertexList) * sizeof(vertexList));
	for each (auto p in pyramidList)
	{
		p->Render();
	}
}
