#pragma once

class CubeCtrl;

class CharacterManager
{
private:
	CubeCtrl			rootCube;
	LPDIRECT3DTEXTURE9	texture;

	void SetTextureVertex(std::vector<D3DXVECTOR2>& UVList, std::vector<D3DXVECTOR2> UVIndex);

public:
	CharacterManager();
	~CharacterManager();

	void Init();
	void Update(D3DXMATRIXA16& parentWorldMatrix);
	void Render();
	D3DXVECTOR3 GetPosition() { return rootCube.GetPosition(); }
};

