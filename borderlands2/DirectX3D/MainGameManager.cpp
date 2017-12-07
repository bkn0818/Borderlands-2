#include "stdafx.h"
#include "MainGameManager.h"

MainGameManager::MainGameManager()
	: sSphereMesh(nullptr), bSphereMesh(nullptr)
{
}

MainGameManager::~MainGameManager()
{
}

HRESULT MainGameManager::Init()
{

	return S_OK;
}

void MainGameManager::Update()
{
}

void MainGameManager::Render()
{
	environment->Render();
}

void MainGameManager::Destroy()
{
	if (gridCtrl) gridCtrl->Destroy();
}

void MainGameManager::SetLight()
{
	D3DXVECTOR3 direction(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&direction, &direction);
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;

	D3DDEVICE->SetLight(0, &light);
	D3DDEVICE->LightEnable(0, true);
}

void MainGameManager::WndProc(HWND g_HWND_Window, UINT msg, WPARAM wParam, LPARAM lParam)
{
}
