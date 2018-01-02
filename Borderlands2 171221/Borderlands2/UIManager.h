#pragma once

//#define UIMNG UIManager::GetInstance()

class UIManager : public iUIButtonDelegate
{
	//SINGLETON(UIManager)
private:
	LPD3DXSPRITE	sprite;
	UIObject		uiObj;

public:
	UIManager();
	~UIManager();

	void Init();
	void Release();
	void Update();
	void Destroy();
	void Render(RECT drawRect);

	void OnClick(UIButton* pSender);
};

