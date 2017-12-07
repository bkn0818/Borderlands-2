#pragma once
class SceneNode;

using namespace std;

class GameScene : public SceneNode
{
private:
	Environment*	environment;
	D3DLIGHT9		light;
	CameraCtrl*		camCtrl;
	D3DXMATRIX		worldTM;
	CharacterCtrl*	charCtrl;
	GridCtrl*		gridCtrl;
	LPD3DXMESH		pickMesh;
	bool			isSetup = false; 
	SphereInfo		sphereInfo;
	D3DXVECTOR3		pickedPos;
	D3DXVECTOR3		vPick;
	D3DMATERIAL9	material;
	DWORD			x, y;
	SkinnedMesh*	player;
	ParticleSystem*	snow;

	vector<SkinnedMesh*> smList;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	void SetLight();

	void MainProc(HWND, UINT, WPARAM, LPARAM);

	GameScene();
	~GameScene();
};

