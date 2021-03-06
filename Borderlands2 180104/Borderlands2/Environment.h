#pragma once
class Group;
class Environment
{
private:
	D3DXMATRIX matWorld;

private:
	HeightMap*			hMap;
	std::vector<Group*> group;
	vector<SkinnedMesh*> objList;

public:
	Environment();
	~Environment();

	HRESULT		SetEnvironment();
	HRESULT		Destroy();
	void		Render();
	HeightMap*	GetHeightMap() { return hMap; };
};

