#include "stdafx.h"
#include "GameScene.h"

GameScene::GameScene()
	:pickedPos(0, 0, 0)
{
	D3DXMatrixIdentity(&worldTM);
}

GameScene::~GameScene()
{
}

HRESULT GameScene::Init(void)
{
	player = new Player;
	player->Init();

	em = new EnemyManager;
	em->Init();

	environment = new Environment;
	environment->SetEnvironment();

	g_pCamera->SetupCamera(D3DDEVICE, 300);
	SetLight();

	return S_OK;
}

void GameScene::Release(void)
{

}

void GameScene::Update(void)
{
	g_pCamera->ProcessMouse();
	
	player->Update(environment->GetHeightMap());

	em->Update(environment->GetHeightMap(), player->GetPosition(), player->GetSphere());
}

void GameScene::Render(void)
{
	player->Render();
	em->Render();
	environment->Render();
}

void GameScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	player->MainProc(hWnd, msg, wParam, lParam);
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
		}
		break;
		case VK_ESCAPE:
		{
			exit(0);
		}
		break;
	}
}

void GameScene::SetLight()
{
	D3DXVECTOR3 direction(0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize(&direction, &direction);
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;

	D3DDEVICE->SetLight(0, &light);
	D3DDEVICE->LightEnable(0, true);
}
