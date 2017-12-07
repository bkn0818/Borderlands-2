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
	environment = new Environment;
	environment->SetEnvironment();

	g_pCamera->SetupCamera(D3DDEVICE, 300);
	g_pCamera->SetEye(&D3DXVECTOR3(0, 0, 0));

	gridCtrl = new GridCtrl;
	gridCtrl->Init();

	charCtrl = new CharacterCtrl;
	charCtrl->Init();

	BoundingBox bound;
	ZeroMemory(&bound, sizeof(BoundingBox));
	bound.max = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);
	bound.min = D3DXVECTOR3(100.0f, 100.0f, 100.0f);
	snow = new Particle_Snow(&bound, 5000);
	snow->Init("snowflake.dds");

	player = g_pSkinnedMeshManager->GetSkinnedMesh("zealot", "TurretGun.X");
	player->SetRandomTrackPosition();
	player->SetPosition(D3DXVECTOR3(0, 0, 0));

	g_pSoundManager->AddSound("mainSound", "DisgaeaMenuSceneBGM.mp3", true, true);

	x = g_ptMouse.x;
	y = g_ptMouse.y;

	SetLight();
	D3DDEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);

	return S_OK;
}

void GameScene::Release(void)
{
}

void GameScene::Update(void)
{
	snow->Update(g_pTimeManager->GetDeltaTime());
	charCtrl->Update(nullptr);
	charCtrl->GetCameraLookAt(g_pCamera->GetLookAt(), g_pCamera->GetSide());
	g_pCamera->ProcessMouse(D3DDEVICE, x, y, g_HWND_Window);
	g_pCamera->SetEye(charCtrl->GetPosition());
}

void GameScene::Render(void)
{
	snow->Render();
	environment->Render();
	gridCtrl->Render();
	player->UpdateAndRender();
}

void GameScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			static int n = 0;
			for each(auto p in smList)
			{
				p->SetAnimationIndex(++n);
			}
		}
		break;
	}
}

void GameScene::SetLight()
{
	D3DXVECTOR3 direction(0.0f, -100.0f, 0.0f);
	D3DXVec3Normalize(&direction, &direction);
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;

	D3DDEVICE->SetLight(0, &light);
	D3DDEVICE->LightEnable(0, true);
}
