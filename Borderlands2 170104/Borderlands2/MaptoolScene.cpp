#include "stdafx.h"
#include "MaptoolScene.h"
#include "Menu.h"
#include "MainTool.h"

Menu menu;
Control cont;
HWND m_hWnd, hMenu, hBasic, hTab, hPlane, hTexture;
BOOL CALLBACK PlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TextureProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
int flag = 0;
bool lbuttonflag = false;
bool rbuttonflag = false;
bool SpaceFlag = false;

MaptoolScene::MaptoolScene()
{
	D3DXMatrixIdentity(&worldTM);
}

MaptoolScene::~MaptoolScene()
{
}

HRESULT MaptoolScene::Init(void)
{
	SetLight();
	grid = new GridCtrl;
	grid->Init();
	g_pCamera->SetupCamera(D3DDEVICE, 500);
	g_pCamera->SetEye(&D3DXVECTOR3(0, 5, 10));

	hMap = new HeightMap();
	hMap->Load("Environment/HeightMap/HeightMap.raw", &worldTM);
	hMap->SetTexture(g_pTextureManager->GetTexture("Environment/HeightMap/terrain.jpg"));

	D3DDEVICE->CreateVertexBuffer(720 * sizeof(CUSTOMCIRCLE), 0, CUSTOMCIRCLE::FVF, D3DPOOL_MANAGED, &circle, nullptr);
	picking = new Picking;

	

	return S_OK;
}

void MaptoolScene::Release(void)
{
}

void MaptoolScene::Update(void)
{
	picking->CalculatePickingRay(g_ptMouse.x, g_ptMouse.y, g_hWnd);
	g_pCamera->ProcessKey();
	g_pCamera->ProcessMouse();
	CUSTOMCIRCLE *pCircle;

	circle->Lock(0, 720 * sizeof(CUSTOMCIRCLE), (void**)&pCircle, 0);

	for (int i = 0; i < 360; ++i)
	{
		pCircle[i].Pos.x = 20 * sin(i * 3.14f / 180.0f) + picking->PickPos.x;
		pCircle[i].Pos.z = 20 * cos(i * 3.14f / 180.0f) + picking->PickPos.z;
		pCircle[i].Pos.y = hMap->GetHeight(pCircle[i].Pos.x, pCircle[i].Pos.y, pCircle[i].Pos.z) + 2.0f;
	}

	for (int i = 360; i < 720; ++i)
	{
		pCircle[i].Pos.x = 10 * sin((i - 360) * 3.14f / 180.0f) + picking->PickPos.x;
		pCircle[i].Pos.z = 10 * cos((i - 360) * 3.14f / 180.0f) + picking->PickPos.z;
		pCircle[i].Pos.y = hMap->GetHeight(pCircle[i].Pos.x, pCircle[i].Pos.y, pCircle[i].Pos.z) + 2.0f;
	}

	circle->Unlock();
}

void MaptoolScene::Render(void)
{
	D3DDEVICE->SetStreamSource(0, circle, 0, sizeof(CUSTOMCIRCLE));
	D3DDEVICE->SetFVF(CUSTOMCIRCLE::FVF);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 0, 180);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 1, 179);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 360, 180);
	D3DDEVICE->DrawPrimitive(D3DPT_LINELIST, 361, 179);
	grid->Render();
	D3DDEVICE->SetTransform(D3DTS_WORLD, &worldTM);
	hMap->Render();
}

void MaptoolScene::SetLight()
{
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	D3DXVECTOR3 dir(0, 0, 1);
	light.Direction = dir;
	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;

	D3DDEVICE->SetLight(0, &light);
	D3DDEVICE->LightEnable(0, true);
}


void MaptoolScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_RBUTTONDOWN:
		rbuttonflag = true;
		break;

	case WM_RBUTTONUP:
		rbuttonflag = false;
		break;

	case WM_LBUTTONDOWN:
		lbuttonflag = true;
		break;

	case WM_LBUTTONUP:
		lbuttonflag = false;
		break;

	case WM_KEYDOWN:
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			SpaceFlag = !SpaceFlag;
		}
		break;

	case WM_CLOSE:
		cont.Cleanup();
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		cont.Cleanup();
		PostQuitMessage(0);
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			if (hBasic) DestroyWindow(hBasic);
			switch (TabCtrl_GetCurSel(hTab))
			{
			case 0:
				hBasic = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_PLANE), hTab, PlaneProc);
				break;
			case 1:
				hBasic = CreateDialog(g_hInst, MAKEINTRESOURCE(IDD_TEXTURE), hTab, TextureProc);
				break;
			}

			ShowWindow(hBasic, SW_SHOW);

			break;
		
		}
	}
}

BOOL CALLBACK PlaneProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
	return true;
}
BOOL CALLBACK TextureProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_DESTROY, 0, 0);
	}
	return true;
}

