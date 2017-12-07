#include "stdafx.h"
#include "Environment.h"

Environment::Environment()
{
	D3DXMatrixIdentity(&matWorld);
}

Environment::~Environment()
{
}

HRESULT Environment::SetEnvironment()
{
	LPSTR	sky[6] = { "Images/sunset_up.tga", "Images/sunset_ft.tga", "Images/sunset_bk.tga", "Images/sunset_rt.tga", "Images/sunset_lf.tga", "Images/sunset_dn.tga" };

	skyBox = new SkyBox;
	if (FAILED(skyBox->Init(sky))) return E_FAIL;

	D3DXMatrixTranslation(&matWorld, -100.0f, -10.0f, -150.0f);

	hMap = new HeightMap;
	if (FAILED(hMap->Load("HeightMap/HeightMap.raw", &matWorld))) return E_FAIL;
	hMap->SetTexture(g_pTextureManager->GetTexture("HeightMap/terrain.jpg"));

	return S_OK;
}

HRESULT Environment::Destroy()
{
	if (FAILED(skyBox->Destroy())) return E_FAIL;
	if (hMap) SAFE_RELEASE(hMap);

	return S_OK;
}

void Environment::Render()
{
	D3DXMatrixIdentity(&matWorld);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	skyBox->Render();
	hMap->Render();
}


