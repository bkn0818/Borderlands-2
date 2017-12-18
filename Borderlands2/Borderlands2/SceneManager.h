#pragma once

#define g_pSceneManager SceneManager::GetInstance()
using namespace std;

class SceneNode;

class SceneManager
{
	SINGLETON(SceneManager)
private:
	static SceneNode* currentScene;
	static SceneNode* loadingScene;
	static SceneNode* readyScene;

	map<string, SceneNode*> mSceneList;
	map<string, SceneNode*> mLoadingSceneList;
	DWORD loadingThreadID;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	void MainProc(HWND, UINT, WPARAM, LPARAM);

	//¾À Ãß°¡
	SceneNode* AddScene(string sceneName, SceneNode* scene);

	//·Îµù ¾À Ãß°¡
	SceneNode* AddLoadingScene(string loadingSceneName, SceneNode* scene);

	HRESULT ChangeScene(string sceneName);
	HRESULT ChangeScene(string sceneName, string loadingSceneName);

	//friend DWORD CALLBACK LoadingThread(LPVOID prc);
};

