#pragma once
#include "SceneNode.h"
#include "resource.h"

class MaptoolScene : public SceneNode
{
private:
	HeightMap*	hMap;
	D3DXMATRIX	worldTM;
	GridCtrl*	grid;
	D3DLIGHT9	light;
	LPDIRECT3DVERTEXBUFFER9 circle; //픽킹 원 정점 버퍼
	Picking* picking;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	void SetLight();
	void MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	MaptoolScene();
	~MaptoolScene();
};

