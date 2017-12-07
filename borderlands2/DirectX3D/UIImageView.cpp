#include "stdafx.h"
#include "UIImageView.h"

UIImageView::UIImageView()
	:color(D3DCOLOR_XRGB(255, 255, 255))
{
}

UIImageView::~UIImageView()
{
}

void UIImageView::Render(LPD3DXSPRITE sprite, RECT drawRect)
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc = { drawRect.left, drawRect.top, drawRect.right, drawRect.bottom };
	sprite->Draw(texture, &rc, 0, &GetLocalPos(), color);

	sprite->End();

	UIObject::Render(sprite, drawRect);
}
