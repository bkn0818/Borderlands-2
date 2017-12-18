#pragma once

class SceneNode;

class MainScene : public SceneNode
{
private:
	UIObject*		UIRoot;
	UIImageView*	background;
	LPD3DXSPRITE	spriteBackground;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	void MainProc(HWND, UINT, WPARAM, LPARAM);

	MainScene();
	~MainScene();
};

