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
	OBJLoader* objLoader = new OBJLoader;
	objLoader->Load("testsky3.obj", nullptr, group);
	SAFE_RELEASE(objLoader);

	hMap = new HeightMap;
	hMap->Load("Environment/HeightMap/HeightMap.raw", &matWorld);
	hMap->SetTexture(g_pTextureManager->GetTexture("Environment/HeightMap/terrain.jpg"));

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
	for each(auto p in group) p->Render();
	D3DXMatrixIdentity(&matWorld);
	D3DDEVICE->SetTransform(D3DTS_WORLD, &matWorld);
	D3DDEVICE->SetRenderState(D3DRS_LIGHTING, true);
	hMap->Render();
}


