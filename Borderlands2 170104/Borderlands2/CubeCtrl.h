#pragma once

class Vector3;

class CubeCtrl : public GameObject
{
public:
	CubeCtrl ();
	virtual ~CubeCtrl ();

	virtual void Init ( D3DXMATRIXA16* pmat = nullptr);
	//void Update();
	virtual void Render ( D3DXMATRIXA16* pmat = nullptr);

	virtual void SetMaterialColor ( D3DXCOLOR color );


private:

	//std::vector<VertexPC>	PCVerts;
	std::vector<VertexPN>	PNVerts;
	D3DMATERIAL9			material;
};

