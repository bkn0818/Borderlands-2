#pragma once

class MapTool
{
private:
	D3DXVECTOR3 vEye, vAt, vUp;
	D3DXMATRIX mView, mProj;
	CameraCtrl*	camCtrl;
	SkinnedMesh* skinnedMesh;
	D3DLIGHT9 light;
	GridCtrl* grid;
	

public:
	HRESULT Init();
	bool InitInstance();
	void SetViewPort();
	void SetLight();
	void Update();
	HRESULT Release();
	void Render();
	void WndProc(HWND g_HWND_Window, UINT msg, WPARAM wParam, LPARAM lParam);

	MapTool();
	~MapTool();
};

