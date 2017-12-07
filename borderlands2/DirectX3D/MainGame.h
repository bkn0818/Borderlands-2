#pragma once
using namespace std;

class MainGame : public SceneNode
{
private:
	vector<SkinnedMesh*>	meshList;
	Environment*	environment;
	CameraCtrl*		camCtrl;
	GridCtrl*		gridCtrl;
	MapTool*		mapTool;
	SceneNode*		mainScene;
	SceneNode*		loadingScene;
	SceneNode*		gameScene;
	SkinnedMesh*	sm;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	void MainProc(HWND, UINT, WPARAM, LPARAM);

	MainGame();
	~MainGame();
};

