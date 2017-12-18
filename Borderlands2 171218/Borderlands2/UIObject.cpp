#include "stdafx.h"
#include "UIObject.h"


UIObject::UIObject()
	: localPos(0, 0, 0)
	, parent(nullptr)
	, size(0, 0)
	, isDebugRender(false)
	, tag(0)
{
	D3DXMatrixIdentity(&worldMatrix);
}

UIObject::~UIObject()
{
	Destroy();
}

void UIObject::Update()
{
	UpdateWorldTM();
	UpdateChildren();
}

void UIObject::UpdateWorldTM()
{
	worldMatrix._41 = localPos.x;
	worldMatrix._42 = localPos.y;
	worldMatrix._43 = localPos.z;

	if (parent) {
		worldMatrix._41 += parent->worldMatrix._41;
		worldMatrix._42 += parent->worldMatrix._42;
		worldMatrix._43 += parent->worldMatrix._43;
	}
}

void UIObject::UpdateChildren()
{
	for each(auto it in childList) {
		it->Update();
	}
}

void UIObject::Render(LPD3DXSPRITE sprite, RECT drawRect)
{
	if (isDebugRender) {
		float l = worldMatrix._41;
		float t = worldMatrix._42;
		float r = worldMatrix._41 + size.x;
		float b = worldMatrix._42 + size.y;

		D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
		std::vector<VertexRHWC> vecVertex;

		vecVertex.reserve(8);
		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(l, t, 0, 1), c));
		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(r, t, 0, 1), c));

		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(r, t, 0, 1), c));
		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(r, b, 0, 1), c));

		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(r, b, 0, 1), c));
		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(l, b, 0, 1), c));

		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(l, b, 0, 1), c));
		vecVertex.push_back(VertexRHWC(D3DXVECTOR4(l, t, 0, 1), c));

		D3DDEVICE->SetFVF(VertexRHWC::FVF);
		D3DDEVICE->DrawPrimitiveUP(D3DPT_LINELIST, 4, &vecVertex[0], sizeof(VertexRHWC));
	}

	for each(auto it in childList) {
		it->Render(sprite, drawRect);
	}

}

void UIObject::AddChild(UIObject * child)
{
	child->parent = this;
	childList.push_back(child);
}


void UIObject::Destroy()
{
	for each(auto it in childList) {
		it->Destroy();
	}
}

UIObject * UIObject::GetChildByTag(int _tag)
{
	if (tag == _tag)
		return this;

	for each(auto it in childList) {
		UIObject* child = it->GetChildByTag(_tag);
		if (child != nullptr)
			return child;
	}
	return nullptr;

}





