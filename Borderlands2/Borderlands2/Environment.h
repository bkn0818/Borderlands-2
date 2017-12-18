#pragma once
class Group;
class Environment
{
private:
	D3DXMATRIX matWorld;

private:
	SkyBox*				skyBox;
	HeightMap*			hMap;
	std::vector<Group*> group;

public:
	Environment();
	~Environment();

	HRESULT		SetEnvironment();
	HRESULT		Destroy();
	void		Render();
	HeightMap*	GetHeightMap() { return hMap; };
};

