#include "stdafx.h"
#include "Loading.h"


Loading::Loading()
{
}


Loading::~Loading()
{
}

HRESULT Loading::Init()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	D3DXIMAGE_INFO info;
	D3DXCreateSprite(D3DDEVICE, &backgroundSprite);
	background = new UIImageView;
	LPDIRECT3DTEXTURE9 tex = g_pTextureManager->GetTexture("Texture/background.jpg", &info);
	background->SetTexture(tex);
	background->SetSize(D3DXVECTOR2(info.Width, info.Height));
	background->SetDebugRender(true);

	UIRoot = new UIObject;
	UIRoot = background;

	return S_OK;
}

void Loading::Release()
{
}

void Loading::Update()
{
	UIRoot->Update();
	if (LoadingDone()) g_pSceneManager->ChangeScene("gameScene");
}

void Loading::Render()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UIRoot->Render(backgroundSprite, rc);
}

void Loading::LoadTexture(const char * fileName, D3DXIMAGE_INFO * imageInfo)
{
	LoadItem* item = new LoadItem;
	item->InitForTexture(fileName, imageInfo);
	vLoadItem.push_back(item);
}

void Loading::LoadXFile(char* folderName, char * fileName)
{
	LoadItem* item = new LoadItem;
	item->InitForXFile(folderName, fileName);
	vLoadItem.push_back(item);
}

void Loading::LoadSound(string keyName, const char * fileName, bool bgm, bool loop)
{
	LoadItem* item = new LoadItem;
	item->InitForSound(keyName, fileName, bgm, loop);
	vLoadItem.push_back(item);
}

bool Loading::LoadingDone()
{
	if (currentGauge >= vLoadItem.size())
	{
		return TRUE;
	}

	LoadItem* item = vLoadItem[currentGauge];

	switch (item->GetLoadingKind())
	{
	case LOAD_KIND_TEXTURE:
	{
		tagTextureResource img = item->GetTextureResource();
		g_pTextureManager->GetTexture(img.fileName, img.imgInfo);
	}
	break;

	case LOAD_KIND_XFILE:
	{
		tagXFileResource xFile = item->GetXFileResource();
		g_pSkinnedMeshManager->GetSkinnedMesh(xFile.fileName, xFile.fileName);
	}
	break;

	case LOAD_KIND_SOUND:
	{
		tagSoundResource sound = item->GetSoundResource();
		g_pSoundManager->AddSound(sound.keyName, sound.fileName, sound.bgm, sound.loop);
	}
	break;
	}

	currentGauge++;


	return FALSE;
}

LoadItem::LoadItem()
{
}

LoadItem::~LoadItem()
{
}

HRESULT LoadItem::InitForTexture(const char * fileName, D3DXIMAGE_INFO * imageInfo)
{
	kind = LOAD_KIND_TEXTURE;

	memset(&texResource, 0, sizeof(tagTextureResource));

	texResource.fileName	= fileName;
	texResource.imgInfo		= imageInfo;

	return S_OK;
}

HRESULT LoadItem::InitForSound(string keyName, const char * fileName, bool bgm, bool loop)
{
	kind = LOAD_KIND_SOUND;

	memset(&soundResource, 0, sizeof(tagSoundResource));
	
	soundResource.keyName	= keyName;
	soundResource.fileName	= fileName;
	soundResource.bgm		= bgm;
	soundResource.loop		= loop;

	return S_OK;
}

HRESULT LoadItem::InitForXFile(char* folderName, char * fileName)
{
	kind = LOAD_KIND_XFILE;

	memset(&xFileResource, 0, sizeof(tagXFileResource));

	xFileResource.folderName = folderName;
	xFileResource.fileName	 = fileName;

	return S_OK;
}
