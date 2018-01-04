#pragma once
class SceneNode
{
private:

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual void MainProc(HWND, UINT, WPARAM, LPARAM);

	SceneNode();
	~SceneNode();
};

