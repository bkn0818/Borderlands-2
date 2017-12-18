#include "stdafx.h"
#include "SceneNode.h"

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
}

HRESULT SceneNode::Init(void)
{
	g_pSoundManager->Init();
	g_pKeyManager->Init();

	return S_OK;
}

void SceneNode::Release(void)
{
	g_pSoundManager->Release();
}

void SceneNode::Update(void)
{
	g_pSoundManager->Update();
}

void SceneNode::Render(void)
{
}

void SceneNode::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
}
