#pragma once
#define SKY_TEXTURE 6
struct SKYVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
};

class SkyBox : public Object
{
private:				
	std::vector<LPDIRECT3DTEXTURE9>		textureList;
	LPDIRECT3DVERTEXBUFFER9				vb;

public:
	HRESULT Init(LPSTR fileName[SKY_TEXTURE]);
	HRESULT CreateSkyBox();
	HRESULT SetTexture(LPSTR file[SKY_TEXTURE]);
	HRESULT Destroy();
	void Render();

	SkyBox();
	~SkyBox();
};

