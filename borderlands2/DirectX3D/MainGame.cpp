#include "stdafx.h"
#include "MainGame.h"
#include "MainScene.h"
#include "LoadingScene.h"
#include "GameScene.h"

HRESULT MainGame::Init(void)
{
	SceneNode::Init();
	mainScene = new MainScene;
	loadingScene = new LoadingScene;
	gameScene = new GameScene;
	
	g_pSceneManager->AddScene("mainScene", mainScene);
	g_pSceneManager->AddScene("loadingScene", loadingScene);
	g_pSceneManager->AddScene("gameScene", gameScene);
	
	g_pSceneManager->ChangeScene("mainScene");

	return S_OK;
}

void MainGame::Release(void)
{
	SceneNode::Release();
	return VOID();
}

void MainGame::Update(void)
{
	SceneNode::Update();
	TimeManager::GetInstance()->Update();
	g_pSceneManager->Update();
}

void MainGame::Render(void)
{
	SceneNode::Render();
	g_pSceneManager->Render();
}

void MainGame::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_pSceneManager->MainProc(hWnd, msg, wParam, lParam);
}

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}
