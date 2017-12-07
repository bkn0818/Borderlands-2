#pragma once
class SceneNode
{
private:
	HDC hdc;
	bool managerInit;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	HDC GetHDC() { return hdc; }

	virtual void MainProc(HWND, UINT, WPARAM, LPARAM);

	SceneNode();
	~SceneNode();
};

