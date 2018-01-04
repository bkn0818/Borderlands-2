#pragma once
#include "UIObject.h"

class UIImageView : public UIObject
{
public:
	UIImageView();
	virtual ~UIImageView();

	GETSET( D3DCOLOR, color, Color );
	GETSET_REF( LPDIRECT3DTEXTURE9, texture, Texture );

	void SetScale(float x, float y, float z);
	virtual void Render(LPD3DXSPRITE sprite, const char * progressBarName) override;
	
};

