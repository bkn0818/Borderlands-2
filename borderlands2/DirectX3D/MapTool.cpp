#include "stdafx.h"
#include "MapTool.h"

MapTool::MapTool()
	: vEye(0, 0, 0)
	, vAt(0, 0, 0)
	, vUp(0, 1, 0)
{
}


MapTool::~MapTool()
{
}


HRESULT MapTool::Init()
{
	

	
	return S_OK;
}

bool MapTool::InitInstance()
{
	

	return true;
}

void MapTool::SetViewPort()
{	

}

HRESULT MapTool::Release()
{

	return S_OK;
}

void MapTool::Update()
{
}

void MapTool::Render()
{
}

void MapTool::WndProc(HWND g_HWND_Window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
}

void MapTool::SetLight()
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