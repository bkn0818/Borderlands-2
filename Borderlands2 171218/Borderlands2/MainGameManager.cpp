#include "stdafx.h"
#include "MainGameManager.h"

MainGameManager::MainGameManager()
{
}


MainGameManager::~MainGameManager()
{

}

void MainGameManager::Init()
{
	srand((unsigned int)time(0));
	g_pSceneManager->Init();
	g_pInput->Init(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);

	keyTestFont = g_pFontManager->GetFont(FontManager::QUEST);
	mainScene = new MainScene;
	gameScene = new GameScene;
	loadingScene = new LoadingScene;
	maptoolScene = new MaptoolScene;
	g_pSceneManager->AddScene("mainScene", mainScene);
	g_pSceneManager->AddScene("loadingScene", loadingScene);
	g_pSceneManager->AddScene("gameScene", gameScene);
	g_pSceneManager->AddScene("maptool", maptoolScene);
	g_pSceneManager->ChangeScene("loadingScene");
}

void MainGameManager::Update()
{
	g_pTimeManager->Update();
	g_pSceneManager->Update();
}

void MainGameManager::Render()
{
	g_pSceneManager->Render();
}

void MainGameManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_pInput->WndProc(msg, wParam, lParam);
	g_pSceneManager->MainProc(hWnd, msg, wParam, lParam);
}

void MainGameManager::Destroy()
{
	g_pSkinnedMeshManager->Destroy();
	g_pInput->FreeDirectInput();
}

void MainGameManager::LogRender()
{
	RECT rc;
	BYTE state;
	SetRect(&rc,
			0,
			0,
			200,
			200);

	std::string keyLog("DirectInput KeyLog - ");
	keyLog += g_pInput->GetKeyLog();


	state = g_pInput->keyStateLeft;

	keyLog += '\n';
	keyLog += "VK_LEFT - ";
	switch (state) {
	case INPUT_PRESS_REPEAT:
	{
		keyLog += "INPUT_PRESS_REPEAT";
	}
	break;
	case INPUT_PRESS:
	{
		keyLog += "INPUT_PRESS";
	}
	break;
	case INPUT_UNPRESS_REPEAT:
	{
		keyLog += "INPUT_UNPRESS_REPEAT";
	}
	break;
	case INPUT_UNPRESS:
	{
		keyLog += "INPUT_UNPRESS";
	}
	break;
	}


	state = g_pInput->keyStateBackSpace;

	keyLog += '\n';
	keyLog += "DIK_BACK - ";
	switch (state) {
	case INPUT_PRESS_REPEAT:
	{
		keyLog += "INPUT_PRESS_REPEAT";
	}
	break;
	case INPUT_PRESS:
	{
		keyLog += "INPUT_PRESS";
	}
	break;
	case INPUT_UNPRESS_REPEAT:
	{
		keyLog += "INPUT_UNPRESS_REPEAT";
	}
	break;
	case INPUT_UNPRESS:
	{
		keyLog += "INPUT_UNPRESS";
	}
	break;
	}


	keyTestFont->DrawTextA(nullptr,
						   keyLog.c_str(),
						   keyLog.length(),
						   &rc,
						   textFormat,
						   color);

}

void MainGameManager::SetLight()
{
	D3DXVECTOR3 direction(rand() % 100, -rand() % 100, rand() % 100);
	D3DXVec3Normalize(&direction, &direction);

	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Ambient = light.Diffuse = light.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = direction;


	D3DDEVICE->SetLight(0, &light);
	D3DDEVICE->LightEnable(0, true);

}





