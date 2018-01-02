#include "stdafx.h"
#include "UIImageView.h"

UIImageView::UIImageView()
	:color(D3DCOLOR_XRGB(255, 255, 255))
{
}

UIImageView::~UIImageView()
{
}

void UIImageView::SetScale(float x, float y, float z)
{
	D3DXMatrixScaling(&worldMatrix, x, y, z);
}

void UIImageView::Render(LPD3DXSPRITE sprite, RECT drawRect)
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldMatrix);
	RECT rc = { drawRect.left, drawRect.top, drawRect.right, drawRect.bottom };
	sprite->Draw(texture, &rc, 0, &GetLocalPos(), color);

	sprite->End();

	UIObject::Render(sprite, drawRect);
}
