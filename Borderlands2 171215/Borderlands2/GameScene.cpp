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

	g_pCamera->SetupCamera(D3DDEVICE, 1000);
	g_pCamera->SetEye(&D3DXVECTOR3(0, 0, 10));
	
	gridCtrl = new GridCtrl;
	gridCtrl->Init();
	
	charCtrl = new CharacterCtrl;
	charCtrl->Init();
	
	
	player = g_pSkinnedMeshManager->GetSkinnedMesh("./XFile/", "test_hand.X");
	player->SetAnimationIndex(4);
	player->SetPosition(D3DXVECTOR3(0, 10, -40));

	BoundingBox bound;
	ZeroMemory(&bound, sizeof(bound));
	bound.min = D3DXVECTOR3(-300, -0, -300);
	bound.max = D3DXVECTOR3(300, 100, 300);
	snow = new Particle_Snow(&bound, 5000);
	snow->Init("snow/snowflake.dds");


	x = g_ptMouse.x;
	y = g_ptMouse.y;
	SetLight();

	return S_OK;
}

void GameScene::Release(void)
{

}

void GameScene::Update(void)
{
	g_pCamera->SetEye(&player->GetCamBonePos());
	g_pCamera->ProcessMouse();
	charCtrl->Update(environment->GetHeightMap());
	charCtrl->GetCameraLookAt(g_pCamera->GetLookAt(), g_pCamera->GetSide());
	player->SetPosition(*charCtrl->GetPosition());
	snow->Update(g_pTimeManager->GetDeltaTime());
}

void GameScene::Render(void)
{
	environment->Render();
	snow->Render();
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
			
			player->SetAnimationIndex(++n);
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
