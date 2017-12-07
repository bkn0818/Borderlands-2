#pragma once

class MaterialTexture : public Object
{
public:
	MaterialTexture();
	~MaterialTexture();

protected:
	//for Mesh
	GETSET(int, mtRef, MTRef); //레퍼런스 포인터

	GETSET_REF(D3DMATERIAL9, material, Material);
	GETSET_ADD_REF(LPDIRECT3DTEXTURE9, texture, Texture);
};

