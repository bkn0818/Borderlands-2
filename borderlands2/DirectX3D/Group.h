#pragma once

class MaterialTexture;

class Group : public Object
{
public:
	Group();
	~Group();

	void SetVertexBuffer();
	void Render();

protected:
	GETSET_REF(std::vector<VertexPNT>, PNTVerts, PNTVerts);
	GETSET_REF(LPD3DXMESH, mesh, Mesh);
	GETSET_ADD_REF(MaterialTexture*, materialTexture, MaterialTexture);

	LPDIRECT3DVERTEXBUFFER9 vb;
};

