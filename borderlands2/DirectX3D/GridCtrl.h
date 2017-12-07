#pragma once

class Pyramid;

class GridCtrl
{
public:
	GridCtrl();
	~GridCtrl();

	void Init();
	void Destroy();
	void Render();

private:
	IDirect3DVertexBuffer9*		vb;
	std::vector<VertexPC>		vertexList;
	std::vector<Pyramid*>		pyramidList;
	D3DXMATRIX					worldMatrix;
	D3DMATERIAL9				material;
};

