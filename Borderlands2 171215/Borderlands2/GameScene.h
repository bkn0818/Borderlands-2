#pragma once
class SceneNode;
struct Bone;
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
	ParticleSystem*	snow;
	ParticleSystem* exp;
	LPD3DXFRAME		camBone;
	D3DXVECTOR3*	v;

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

