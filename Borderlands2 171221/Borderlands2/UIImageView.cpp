#include "stdafx.h"
#include "UIImageView.h"

UIImageView::UIImageView()
	: color(D3DCOLOR_XRGB(255, 255, 255))
{}
UIImageView::~UIImageView() {}

void UIImageView::SetScale(float x, float y, float z)
{
	D3DXMatrixScaling(&worldMatrix, x, y, z);
}

void UIImageView::Render(LPD3DXSPRITE sprite, const char * progressBarName)
{
	RECT rc = { 0, 0, size.x, size.y };

	sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	if (progressBarName == "Bullet")
	{
		D3DXMATRIX matResult;
		D3DXVECTOR2 scaling(-1.0f, 1.0f);
		D3DXVECTOR2 rotCenter(GetLocalPos().x - 1000, GetLocalPos().y + (1233 * 1000));
		float rotation = D3DXToRadian(0.1f);

		D3DXMatrixTransformation2D(&matResult, NULL, 0.f, &scaling, &rotCenter, rotation, NULL);
		sprite->SetTransform(&matResult);
	}

	sprite->Draw(texture, &rc, NULL, &GetLocalPos(), color);

	sprite->End();



	//UIObject::Render(sprite, progressBarName);
}
