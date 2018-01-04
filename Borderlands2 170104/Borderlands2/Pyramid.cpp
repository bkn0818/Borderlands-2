#include "stdafx.h"
#include "Pyramid.h"


Pyramid::Pyramid()
{
}


Pyramid::~Pyramid()
{
}

void Pyramid::Init(D3DXCOLOR color, D3DXMATRIXA16 & _worldMatrix)
{

	worldMatrix = _worldMatrix;

	D3DXVECTOR3 normal(0, 0, 0);
	vertexList.push_back(VertexPN(D3DXVECTOR3(0, 0, 0), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, -1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, -1), normal));

	vertexList.push_back(VertexPN(D3DXVECTOR3(0, 0, 0), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, 1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, -1), normal));

	vertexList.push_back(VertexPN(D3DXVECTOR3(0, 0, 0), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, 1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, 1), normal));

	vertexList.push_back(VertexPN(D3DXVECTOR3(0, 0, 0), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, -1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, 1), normal));

	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, -1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, 1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, -1), normal));

	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, -1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(1, -1, 1), normal));
	vertexList.push_back(VertexPN(D3DXVECTOR3(-1, -1, 1), normal));


	for (int i = 0; i < vertexList.size(); i += 3) {
		D3DXVECTOR3 v01 = vertexList[i + 1].p - vertexList[i + 0].p;
		D3DXVECTOR3 v02 = vertexList[i + 2].p - vertexList[i + 0].p;
		D3DXVec3Cross(&normal, &v01, &v02);

		vertexList[i].n = vertexList[i + 1].n = vertexList[i + 2].n = normal;
	}

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = color;
}

void Pyramid::Render()
{
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	D3DDEVICE->SetMaterial(&material);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMatrix);
	D3DDEVICE->SetFVF(VertexPN::FVF);
	D3DDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
								vertexList.size() / 3,
								&vertexList[0],
								sizeof(VertexPN));
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, false);
}
