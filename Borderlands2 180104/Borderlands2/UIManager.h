#pragma once
#include "progressBar.h"

class UIManager //: public iUIButtonDelegate
{
private:
	LPD3DXSPRITE	sprite;
	UIObject		uiObj;

	progressBar*	emptyBar;
	progressBar*	HPBar;
	progressBar*	BulletBar;

public:
	UIManager();
	~UIManager();

	void Init();
	void Release();
	void Update();
	void Destroy();
	void Render();

	//void OnClick(UIButton* pSender);
	void OnClick(int n);
	void OnAttacked(int n);
};

