#pragma once
class SceneNode;

using namespace std;

class GameScene : public SceneNode
{
private:
	Environment*	environment;
	D3DLIGHT9		light;
	D3DXMATRIX		worldTM;
	CharacterCtrl*	charCtrl;
	GridCtrl*		gridCtrl;
	D3DXVECTOR3		pickedPos;
	D3DXVECTOR3		vPick;
	D3DMATERIAL9	material;
	DWORD			x, y;
	SkinnedMesh*	player;
	Particle_Snow*	snow;

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

