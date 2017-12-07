#include "stdafx.h"
#include "LoadingScene.h"


HRESULT LoadingScene::Init(void)
{
	return S_OK;
}

void LoadingScene::Release(void)
{
}

void LoadingScene::Update(void)
{
}

void LoadingScene::Render(void)
{
}

void LoadingScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
		case WM_RBUTTONDOWN:
			g_pSceneManager->ChangeScene("mainScene");
			break;
		}
		break;
	}
}

LoadingScene::LoadingScene()
{
}


LoadingScene::~LoadingScene()
{
}
