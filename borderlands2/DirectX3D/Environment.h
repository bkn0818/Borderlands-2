#pragma once
class Environment
{
private:
	D3DXMATRIX matWorld;

private:
	SkyBox*		skyBox;
	HeightMap*	hMap;

public:
	Environment();
	~Environment();

	HRESULT		SetEnvironment();
	HRESULT		Destroy();
	void		Render();
	HeightMap*	GetHeightMap() { return hMap; };
};

