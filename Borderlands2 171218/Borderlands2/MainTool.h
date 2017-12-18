#pragma once

class Control
{
private:
	LPDIRECT3DDEVICE9 g_pd3dDevice; //렌더링에 사용될 D3D 디바이스
	LPDIRECT3D9 g_pD3D; //D3D 디바이스를 생성할 객체 변수

	HWND g_hWnd;

	BOOL m_Lflag;

	Plane *pPlane;
	Picking *pPick;
	FrustumCulling* pFrustum;

	char*  m_lpstr;
	D3DXVECTOR3 vScale;

	float m_fLODRatio;

public:
	DWORD MouseX;
	DWORD MouseY;

	Control();

	HRESULT InitD3D(HWND hWnd);

	void CameraMove();
	void Animate(bool frflag);
	void Render(bool lfalg, bool frflag, bool sFlag);
	void InputMouse();
	void Cleanup();

	LPDIRECT3DDEVICE9 ReturnDevice();

	char* ReturnSTR();
	Plane *ReturnPlane();
	D3DXVECTOR3* ReturnScale();
	float ReturnLODRatio();
};
