#include "stdafx.h"
#include "CubeCtrl.h"


CubeCtrl::CubeCtrl()
	: material()
	, vb(nullptr)
{
	D3DXMatrixIdentity(&worldMatrix);
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
}


CubeCtrl::~CubeCtrl()
{
	Destroy();
}

void CubeCtrl::Init(D3DXMATRIX& world)
{
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = material.Diffuse = material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	material.Power = 1.0f;

	D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);

	VertexPNC* vertices;

	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPNC));

	std::vector<D3DXVECTOR3> vertexList;
	
	vertexList.push_back(D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	vertexList.push_back(D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	vertexList.push_back(D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	vertexList.push_back(D3DXVECTOR3(1.0f, -1.0f, -1.0f));

	vertexList.push_back(D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	vertexList.push_back(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	vertexList.push_back(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	vertexList.push_back(D3DXVECTOR3(1.0f, -1.0f, 1.0f));

	std::vector<D3DXVECTOR3> index;
	
	index.push_back(vertexList[0]); index.push_back(vertexList[1]); index.push_back(vertexList[2]);
	index.push_back(vertexList[0]); index.push_back(vertexList[2]); index.push_back(vertexList[3]);
	index.push_back(vertexList[4]); index.push_back(vertexList[6]); index.push_back(vertexList[5]);
	
	index.push_back(vertexList[4]); index.push_back(vertexList[7]); index.push_back(vertexList[6]);
	index.push_back(vertexList[4]); index.push_back(vertexList[5]); index.push_back(vertexList[1]);
	index.push_back(vertexList[4]); index.push_back(vertexList[1]); index.push_back(vertexList[0]);
	
	index.push_back(vertexList[3]); index.push_back(vertexList[2]); index.push_back(vertexList[6]);
	index.push_back(vertexList[3]); index.push_back(vertexList[6]); index.push_back(vertexList[7]);
	index.push_back(vertexList[1]); index.push_back(vertexList[5]); index.push_back(vertexList[6]);
	
	index.push_back(vertexList[1]); index.push_back(vertexList[6]); index.push_back(vertexList[2]);
	index.push_back(vertexList[4]); index.push_back(vertexList[0]); index.push_back(vertexList[3]);
	index.push_back(vertexList[4]); index.push_back(vertexList[3]); index.push_back(vertexList[7]);
	
	for (int i = 0; i < 36; i += 3)
	{
		D3DXVECTOR3 direction;
	
		D3DXVECTOR3 u = index[i + 1] - index[i];
		D3DXVECTOR3 v = index[i + 2] - index[i];
		D3DXVec3Cross(&direction, &u, &v);
		D3DXVec3Normalize(&direction, &direction);
	
		indexList.push_back(VertexPNC(index[i], direction, color));
		indexList.push_back(VertexPNC(index[i + 1], direction, color));
		indexList.push_back(VertexPNC(index[i + 2], direction, color));
	}
	
	for (int i = 0; i < indexList.size(); i++)
	{
		D3DXVec3TransformCoord(&indexList[i].p, &indexList[i].p, &world);
	}

	D3DDEVICE->CreateVertexBuffer(indexList.size() * sizeof(VertexPNC), 0, VertexPC::FVF, D3DPOOL_MANAGED, &vb, 0);

	vb->Lock(0, 0, (void**)&vertices, 0);

	for (int i = 0; i < indexList.size(); i++)
	{
		vertices[i] = indexList[i];
	}

	vb->Unlock();
}

//void CubeCtrl::Update(D3DXMATRIXA16& parentWorldMatrix)
//{
//	D3DXMATRIXA16 rotationMatrix, translationMatrix;
//
//	rotationXAngle += rotationXAngleSpeed;
//	if (rotationXAngle < -D3DX_PI / 4.0f) {
//		rotationXAngleSpeed = -rotationXAngleSpeed;
//		rotationXAngle = -D3DX_PI / 4.0f;
//	}
//
//	if (rotationXAngle > D3DX_PI / 4.0f) {
//		rotationXAngleSpeed = -rotationXAngleSpeed;
//		rotationXAngle = D3DX_PI / 4.0f;
//	}
//
//	D3DXMatrixRotationX(&rotationMatrix, rotationXAngle);
//	D3DXMatrixTranslation(&translationMatrix,
//						  localTranslation.x,
//						  localTranslation.y,
//						  localTranslation.z);
//
//	worldMatrix = rotationMatrix * translationMatrix;
//
//	worldMatrix *= parentWorldMatrix;
//
//	for each(auto child in childList) {
//		child->Update(worldMatrix);
//	}
//
//	// 원래 하던데로 캐릭터 컨트롤만 사용할때
//	//D3DXMatrixIdentity ( &worldMatrix );
//	//worldMatrix *= parentWorldMatrix;
//}

void CubeCtrl::Move(D3DXMATRIX & worldMatrix)
{
	matWorld = worldMatrix;
}


void CubeCtrl::Render(D3DXMATRIX& worldMatrix)
{
	D3DXMATRIX scale, translation;
	D3DXVECTOR3 pos(0, 0.1f, 0);
	D3DXMatrixTranslation(&translation, pos.x, pos.y, pos.z);
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale, 10, 10, 10);
	matWorld = scale * translation * worldMatrix;
	D3DXVec3TransformCoord(&pos, &pos, &matWorld);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	D3DDEVICE->SetMaterial(&material);

	D3DDEVICE->SetStreamSource(0, vb, 0, sizeof(VertexPNC));
	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	D3DDEVICE->SetFVF(VertexPNC::FVF);
	D3DDEVICE->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);
	//D3DDEVICE->DrawPrimitiveUP(
	//	D3DPT_TRIANGLELIST,
	//	indexList.size() / 3,
	//	&indexList[0],
	//	sizeof(VertexPNC)
	//);

	for each(auto child in childList) {
		child->Render(worldMatrix);
	}
}

D3DXVECTOR3 CubeCtrl::GetPosition()
{
	return position;
}

void CubeCtrl::AddChild(CubeCtrl* cubeChild)
{
	childList.push_back(cubeChild);
}

void CubeCtrl::Destroy()
{
	SAFE_RELEASE(vb);
	vertexPT.clear();
	indexList.clear();
	for each(auto p in childList) {
		p->Destroy();
	}
	childList.clear();
}

void CubeCtrl::SetLocalTranslation(D3DXVECTOR3 & v)
{
	localTranslation = v;
}

void CubeCtrl::SetAngleSpeed(float fRotAngleSpeed)
{
	rotationXAngleSpeed = fRotAngleSpeed;
}






