#include "stdafx.h"

ASEFrame::ASEFrame()
	:materialTexture(nullptr)
{
	D3DXMatrixIdentity(&worldTM);
	D3DXMatrixIdentity(&localTM);
}

ASEFrame::~ASEFrame()
{
	SAFE_RELEASE(materialTexture);
}

void ASEFrame::Update(int keyFrame, D3DXMATRIXA16* parent)
{
	D3DXMATRIXA16 matLocalR, matLocalT;

	CalcLocalTranslationMatrix(keyFrame, matLocalT);
	CalcLocalRotationMatrix(keyFrame, matLocalR);

	localTM = matLocalR * matLocalT;
	worldTM = localTM;

	//부모 노드가 있으면 월드매트릭스에 부모 노드 월드매트릭스를 계속 곱해준다
	if (parent) worldTM *= *parent;

	for each(auto p in childList) p->Update(keyFrame, &worldTM);
}

void ASEFrame::Render()
{
	if (materialTexture != nullptr)
	{
		D3DDEVICE->SetTransform(D3DTS_WORLD, &worldTM);
		D3DDEVICE->SetTexture(0, materialTexture->GetTexture());
		D3DDEVICE->SetMaterial(&materialTexture->GetMaterial());
		D3DDEVICE->SetFVF(VertexPNT::FVF);
		D3DDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
									PNTVerts.size() / 3,
									&PNTVerts[0],
									sizeof(VertexPNT)
									);
	}

	for each(auto it in childList) it->Render();
}

void ASEFrame::AddChild(ASEFrame* child)
{
	if (child == nullptr) return;

	SAFE_ADDREF(child);
	childList.push_back(child);
}

void ASEFrame::Destroy()
{
	for each(auto p in childList) p->Destroy();
	Release();
}

void ASEFrame::CalcLocalTM(ASEFrame * pParent)
{
	localTM = worldTM; 
	if (pParent)
	{
		D3DXMATRIXA16 inverseMat;
		D3DXMatrixInverse(&inverseMat, NULL, &pParent->GetWorldTM());
		localTM = worldTM * inverseMat;
	}
}

void ASEFrame::CalcLocalTranslationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16 & mat)
{
	D3DXMatrixIdentity(&mat);
	if (posTrackList.empty())
	{
		mat._41 = localTM._41;
		mat._42 = localTM._42;
		mat._43 = localTM._43;
	}
	else if (nKeyFrame <= posTrackList.front().n)
	{
		mat._41 = posTrackList.front().v.x;
		mat._42 = posTrackList.front().v.y;
		mat._43 = posTrackList.front().v.z;
	}
	else if (nKeyFrame >= posTrackList.back().n)
	{
		mat._41 = posTrackList.back().v.x;
		mat._42 = posTrackList.back().v.y;
		mat._43 = posTrackList.back().v.z;
	}

	else
	{
		int next = 0;
		for (int i = 0; i < posTrackList.size(); ++i)
		{
			if (nKeyFrame < posTrackList[i].n)
			{
				next = i;
				break;
			}
		}

		int prev = next - 1;
		float t = (nKeyFrame - posTrackList[prev].n) / (float)(posTrackList[next].n - posTrackList[prev].n);

		D3DXVECTOR3 v;
		D3DXVec3Lerp(&v, &posTrackList[prev].v, &posTrackList[next].v, t);
		//Translation 정보를 매트릭스에 넣음
		mat._41 = v.x;
		mat._42 = v.y;
		mat._43 = v.z;
	}
}

void ASEFrame::CalcLocalRotationMatrix(IN int nKeyFrame, OUT D3DXMATRIXA16 & mat)
{
	D3DXMatrixIdentity(&mat);
	if (rotTrackList.empty())
	{
		mat = localTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
	}
	else if (nKeyFrame <= rotTrackList.front().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &rotTrackList.front().q);
	}
	else if (nKeyFrame >= rotTrackList.back().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &rotTrackList.back().q);
	}

	else
	{
		int next = -1;
		for (size_t i = 0; i < rotTrackList.size(); i++)
		{
			if (nKeyFrame < rotTrackList[i].n)
			{
				next = i;
				break;
			}
		}
		int prev = next - 1;

		float t = (nKeyFrame - rotTrackList[prev].n) / (float)(rotTrackList[next].n - rotTrackList[prev].n);

		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &rotTrackList[prev].q, &rotTrackList[next].q, t);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
}
