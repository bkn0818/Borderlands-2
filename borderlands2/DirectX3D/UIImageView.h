#pragma once
#include "UIObject.h"

class UIImageView : public UIObject
{
public:
	UIImageView();
	virtual ~UIImageView();

	GETSET( D3DCOLOR, color, Color );
	//GETSET_ADD_REF( LPDIRECT3DTEXTURE9, texture, Texture );
	GETSET_REF( LPDIRECT3DTEXTURE9, texture, Texture );

	virtual void Render( LPD3DXSPRITE sprite, RECT drawRect) override;
};

