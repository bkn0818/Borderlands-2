#pragma once

#include "UIObject.h"

class UIButton;

class iUIButtonDelegate
{
public:
	virtual void OnClick(UIButton* pSender) = 0;
};


class UIButton : public UIObject
{
public:
	UIButton();
	virtual ~UIButton();

	virtual void SetTexture(std::string normal, std::string mouseover, std::string selected);
	virtual void Update() override;
	virtual void Render(LPD3DXSPRITE sprite, const char * progressBarName) override;


	enum ButtonState
	{
		NORMAL,
		MOUSEOVER,
		SELECT,
		STATE_MAX,
	};

	ButtonState buttonState;
	LPDIRECT3DTEXTURE9 textures[ButtonState::STATE_MAX];

	GETSET(iUIButtonDelegate*, buttonDelegate, ButtonDelegate);
};

