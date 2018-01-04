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
	UIRoot = new UIObject;
	background = new UIImageView;
	background->SetLocalPos(D3DXVECTOR3(0, 0, 0));
	
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	LPDIRECT3DTEXTURE9 texture = g_pTextureManager->GetTexture("sprites/mainBackground.jpg");
	background->SetTexture(texture);
	background->SetSize(D3DXVECTOR2(rc.right, rc.bottom));
	background->SetLocalPos(D3DXVECTOR3(0, 0, 0));
	//background->SetDebugRender(true);
	D3DXCreateSprite(D3DDEVICE, &spriteBackground);

	UIRoot = background;
	
	return S_OK;
}

void MainScene::Release(void)
{
	spriteBackground->Release();
	if (UIRoot) UIRoot->Destroy();
	if (background) background->Destroy();
}

void MainScene::Update(void)
{
	if (UIRoot) UIRoot->Update();
}

void MainScene::Render(void)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	if (UIRoot) UIRoot->Render(spriteBackground, NULL);
}

void MainScene::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_RBUTTONDOWN:
		g_pSceneManager->ChangeScene("gameScene");
		break;
	}
}

