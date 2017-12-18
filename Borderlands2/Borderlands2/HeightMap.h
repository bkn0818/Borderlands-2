#pragma once
#include "iMap.h"

#define POLYGON_NUM 256
#define VERTEX_NUM 257

class HeightMap : public iMap
{
public:
	HeightMap();
	~HeightMap();

	void CreateMesh(std::vector<VertexPNT>& PNTList);
	bool SetHeight(IN const float& x, OUT float& y, IN const float& z, RayCtrl& r);
	D3DXVECTOR3 GetHeightMapEntry(int row, int col);
	std::vector<D3DXVECTOR3>& GetVertices() { return verts; };

	virtual void Load(char* szFullPath, D3DXMATRIX* pmat);
	virtual bool GetHeight(IN const float& x, OUT float& y, IN const float& z);
	virtual void Render();
	virtual void Release();


protected:
	GETSET_ADD_REF(LPDIRECT3DTEXTURE9, texture, Texture);
	D3DMATERIAL9				material;
	LPD3DXMESH					mesh;
	std::vector<D3DXVECTOR3>	verts;
};

