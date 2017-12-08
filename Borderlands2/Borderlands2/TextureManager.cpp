#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager ()
{
}


TextureManager::~TextureManager ()
{
	
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture ( const char * fullPath, OUT D3DXIMAGE_INFO * imageInfo )
{
	return GetTexture ( std::string ( fullPath ), imageInfo );
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture ( std::string fullPath, OUT D3DXIMAGE_INFO * imageInfo )
{
	
	if ( imageInfo ) {
		if ( textureMap.find ( fullPath ) == textureMap.end () ||
			imageInfoMap.find ( fullPath ) == imageInfoMap.end () ) {
			SAFE_RELEASE ( textureMap[fullPath] );
			D3DXCreateTextureFromFileExA (
				D3DDEVICE,
				fullPath.c_str (),
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_DEFAULT,
				0,//D3DCOLOR_XRGB(255, 255, 255),
				&imageInfoMap[fullPath],
				NULL,
				&textureMap[fullPath] );
		}
		*imageInfo = imageInfoMap[fullPath];
	}
	else
	{
		if ( textureMap.find ( fullPath ) == textureMap.end () ) {
			D3DXCreateTextureFromFileA (
				D3DDEVICE,
				fullPath.c_str (),
				&textureMap[fullPath] );
		}
	}

	return textureMap[fullPath];
}

void TextureManager::Destroy ()
{
	for each( auto iter in textureMap ) {
		SAFE_RELEASE ( iter.second );
	}
	textureMap.clear ();
	imageInfoMap.clear ();
}
