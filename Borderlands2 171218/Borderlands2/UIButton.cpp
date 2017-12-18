#include "stdafx.h"
#include "UIButton.h"

UIButton::UIButton()
	:buttonState(NORMAL)
{
}

UIButton::~UIButton()
{
}

void UIButton::SetTexture(std::string normal, std::string mouseover, std::string selected)
{
	D3DXIMAGE_INFO imageInfo;
	textures[NORMAL] = g_pTextureManager->GetTexture(normal, &imageInfo);
	textures[MOUSEOVER] = g_pTextureManager->GetTexture(mouseover);
	textures[SELECT] = g_pTextureManager->GetTexture(selected);
	SetSize(D3DXVECTOR2(imageInfo.Width, imageInfo.Height));
}

void UIButton::Update()
{
	UIObject::UpdateWorldTM();

	RECT rc;
	SetRect(&rc, localPos.x, localPos.y, localPos.x + size.x, localPos.y + size.y);

	if (PtInRect(&rc, g_ptMouse))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			buttonState = SELECT;
			if (buttonDelegate) buttonDelegate->OnClick(this);
		}
		else
		{
			buttonState = MOUSEOVER;
		}
	}
	else if (!PtInRect(&rc, g_ptMouse)) buttonState = NORMAL;

	UIObject::UpdateChildren();
}

void UIButton::Render(LPD3DXSPRITE sprite, RECT drawRect)
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	RECT rc = { 0, 0, size.x, size.y };
	sprite->Draw(textures[buttonState], &rc, 0, &GetLocalPos(), D3DCOLOR_XRGB(255, 255, 255));

	sprite->End();

	UIObject::Render(sprite, drawRect);
}

