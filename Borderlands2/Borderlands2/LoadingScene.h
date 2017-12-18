#pragma once
class SceneNode;

class LoadingScene : public SceneNode
{
public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	void MainProc(HWND, UINT, WPARAM, LPARAM);
	LoadingScene();
	~LoadingScene();
};

