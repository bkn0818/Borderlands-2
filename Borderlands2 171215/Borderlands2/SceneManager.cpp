#include "stdafx.h"
#include "SceneManager.h"

SceneNode* SceneManager::currentScene = nullptr;
SceneNode* SceneManager::loadingScene = nullptr;
SceneNode* SceneManager::readyScene = nullptr;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

//DWORD LoadingThread(LPVOID prc)
//{
//	SceneManager::readyScene->Init();
//
//	SceneManager::currentScene = SceneManager::readyScene;
//
//	SceneManager::loadingScene->Release();
//	SceneManager::loadingScene = nullptr;
//	SceneManager::readyScene = nullptr;
//
//	return 0;
//}

HRESULT SceneManager::Init(void)
{
	currentScene = NULL;
	loadingScene = NULL;
	readyScene = NULL;


	return S_OK;
}

void SceneManager::Release(void)
{
	for each(auto p in mSceneList)
	{
		if (p.second)
		{
			if (p.second == currentScene) p.second->Release();
			SAFE_DELETE(p.second);
		}
	}
	mSceneList.clear();
}

void SceneManager::Update(void)
{
	if (currentScene) currentScene->Update();
}

void SceneManager::Render(void)
{
	if (currentScene) currentScene->Render();
}

void SceneManager::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (currentScene) currentScene->MainProc(hWnd, msg, wParam, lParam);
}

SceneNode * SceneManager::AddScene(string sceneName, SceneNode * scene)
{
	//해당 씬이 존재치 않으면 널 반환
	if (!scene) return NULL;

	mSceneList.insert(make_pair(sceneName, scene));

	return scene;
}

SceneNode * SceneManager::AddLoadingScene(string loadingSceneName, SceneNode * scene)
{
	//해당 씬이 존재치 않으면 널 반환
	if (!scene) return NULL;

	mSceneList.insert(make_pair(loadingSceneName, scene));

	return scene;
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	map<string, SceneNode*>::iterator find = mSceneList.find(sceneName);

	//씬이 존재하지 않으면 실패를 알려줘라
	if (find == mSceneList.end()) return E_FAIL;

	//찾은 씬이 현재씬이면 바꾸지 않는다
	if (find->second == currentScene) return S_OK;

	//씬 전환이 성공하면~
	if (SUCCEEDED(find->second->Init()))
	{
		//순서가 마음에 안들면 먼저 씬을 바꾸고, 그 전에 씬에 데이터를 날려주는
		//형태로 바꿔도 상관없다.

		if (currentScene) currentScene->Release();

		currentScene = find->second;

		return S_OK;
	}

	return E_FAIL;
}

HRESULT SceneManager::ChangeScene(string sceneName, string loadingSceneName)
{
	map<string, SceneNode*>::iterator find = mSceneList.find(sceneName);

	//씬이 존재하지 않으면 실패를 알려줘라
	if (find == mSceneList.end()) return E_FAIL;

	//찾은 씬이 현재씬이면 바꾸지 않는다
	if (find->second == currentScene) return S_OK;

	map<string, SceneNode*>::iterator findLoading = mLoadingSceneList.find(loadingSceneName);

	if (find == mLoadingSceneList.end()) return ChangeScene(loadingSceneName);

	//씬 전환이 성공하면~
	if (SUCCEEDED(find->second->Init()))
	{
		if (currentScene) currentScene->Release();

		loadingScene = findLoading->second;

		readyScene = find->second;

		//CloseHandle(CreateThread(NULL, 0, LoadingThread, NULL, 0, &loadingThreadID));

	}

	return E_FAIL;
}


