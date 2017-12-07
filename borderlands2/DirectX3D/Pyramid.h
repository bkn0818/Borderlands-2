#pragma once
class Pyramid
{
public:
	Pyramid();
	~Pyramid();

	void Init(D3DCOLOR color, D3DXMATRIX worldMat);
	void Render();
	void SetMaterialColor(D3DXCOLOR c);

private:
	D3DMATERIAL9			material;
	std::vector<VertexPNC>	vertexPNCList;
	D3DXMATRIXA16			worldMatrix;
	IDirect3DVertexBuffer9*	vb;
};

