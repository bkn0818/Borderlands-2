#include "stdafx.h"
#include "MaterialTexture.h"


MaterialTexture::MaterialTexture()
	: texture(nullptr)
	, mtRef(-1)
{
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
}

MaterialTexture::~MaterialTexture()
{
	SAFE_RELEASE(texture);
}
