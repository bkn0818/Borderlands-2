#pragma once

class MainGameManager : public SceneNode
{
	MainGameManager();
	~MainGameManager();

	virtual HRESULT Init();
	void Update();
	void Render();
	void Destroy();
	void SetLight();
	void WndProc(HWND g_HWND_Window, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	D3DLIGHT9				light;
	CameraCtrl*				cameraCtrl;
	GridCtrl*				gridCtrl;
	LPD3DXMESH				sSphereMesh;
	LPD3DXMESH				bSphereMesh;
	std::vector<Group*>		group;
	std::vector<SphereInfo> sphereList;
	std::vector<VertexPC>	vertexList;
	D3DMATERIAL9			pickedMatarial;
	D3DMATERIAL9			normalMaterial;
	D3DXVECTOR3				pickedPos;
	SkinnedMesh*			zealot;
	CharacterCtrl*			chaCtrl;
	HeightMap*				mapObj;
	std::vector<SkinnedMesh*> smList;
	FrustumCulling*			fcull;
	Environment*			environment;
};

