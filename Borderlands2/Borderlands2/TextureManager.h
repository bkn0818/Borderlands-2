#pragma once

#define g_pTextureManager TextureManager::GetInstance()

class TextureManager
{
	SINGLETON ( TextureManager )

private:
	std::map<std::string, LPDIRECT3DTEXTURE9>	textureMap;
	std::map<std::string, D3DXIMAGE_INFO>		imageInfoMap;


public:
	LPDIRECT3DTEXTURE9 GetTexture ( const char* fullPath, OUT D3DXIMAGE_INFO* imageInfo = nullptr );
	LPDIRECT3DTEXTURE9 GetTexture ( std::string fullPath, OUT D3DXIMAGE_INFO* imageInfo = nullptr );

	void Destroy ();

};

