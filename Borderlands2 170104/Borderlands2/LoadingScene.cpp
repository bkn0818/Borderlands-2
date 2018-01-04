#include "stdafx.h"
#include "LoadingScene.h"
#include "Loading.h"

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::Init(void)
{
	loading = new Loading;
	loading->Init();

	LoadTexture();
	LoadSound();
	LoadXFile();

	return S_OK;
}

void LoadingScene::Release(void)
{
	SAFE_RELEASE(loading);
}

void LoadingScene::Update(void)
{
	loading->Update();
}

void LoadingScene::Render(void)
{
	loading->Render();
}

void LoadingScene::LoadTexture()
{
	loading->LoadTexture("snow/snowflake.dds");
}

void LoadingScene::LoadSound()
{
	loading->LoadSound("mainSound", "DisgaeaMenuSceneBGM.mp3", true, true);
}

void LoadingScene::LoadXFile()
{
	loading->LoadXFile("./XFile/", "test_hand.X");
}
