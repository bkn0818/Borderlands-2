#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

HRESULT MainScene::Init(void)
{
	background = new UIImageView;
	background->SetLocalPos(D3DXVECTOR3(0, 0, 0));
	
	RECT rc;
	GetClientRect(g_HWND_Window, &rc);
	LPDIRECT3DTEXTURE9 texture = g_pTextureManager->GetTexture("sprites/mainBackground.jpg");
	background->SetTexture(texture);
	background->SetSize(D3DXVECTOR2(rc.right, rc.bottom));
	background->SetLocalPos(D3DXVECTOR3(0, 0, 0));
	background->SetDebugRender(true);
	D3DXCreateSprite(D3DDEVICE, &spriteBackground);

	UIRoot = background;
	
	return S_OK;
}

void MainScene::Release(void)
{
}

void MainScene::Update(void)
{
	if (UIRoot) UIRoot->Update();
}

void MainScene::Render(void)
{
	RECT rc;
	GetClientRect(g_HWND_Window, &rc);
	if (UIRoot) UIRoot->Render(spriteBackground, rc);
}

void MainScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//cam->WndProc(hWnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_RBUTTONDOWN:
		g_pSceneManager->ChangeScene("gameScene");
		break;
	}
}

