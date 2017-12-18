#pragma once

enum LOAD_KIND
{
	LOAD_KIND_TEXTURE,
	LOAD_KIND_XFILE,
	LOAD_KIND_SOUND,
	LOAD_KIND_END
};

struct tagTextureResource
{
	const char* fileName;
	D3DXIMAGE_INFO*  imgInfo;
};

struct tagXFileResource
{
	char*		folderName;
	char*		fileName;
};

struct tagSoundResource
{
	string		keyName;
	const char* fileName;
	bool		bgm;
	bool		loop;
};

class LoadItem
{
private:
	LOAD_KIND			kind;
	tagTextureResource	texResource;
	tagXFileResource	xFileResource;
	tagSoundResource	soundResource;

public:
	LoadItem();
	~LoadItem();

	HRESULT InitForTexture(const char* fileName, D3DXIMAGE_INFO* imageInfo = nullptr);
	HRESULT InitForSound(string keyName, const char* fileName, bool bgm = FALSE, bool loop = TRUE);
	HRESULT InitForXFile(char* folderName, char* fileName);

	LOAD_KIND			GetLoadingKind() { return kind; }
	tagTextureResource	GetTextureResource() { return texResource; }
	tagXFileResource	GetXFileResource() { return xFileResource; }
	tagSoundResource	GetSoundResource() { return soundResource; }
};

class Loading
{
private:
	typedef vector<LoadItem*> arrLoadItem;
	typedef vector<LoadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem			vLoadItem;
	LPD3DXSPRITE		backgroundSprite;
	UIObject*			UIRoot;
	UIImageView*		background;
	int					currentGauge;
	char				loadingFileName[128];
public:
	Loading();
	~Loading();

	HRESULT Init();
	void Release();
	void Update();
	void Render();

	void LoadTexture(const char* fileName, D3DXIMAGE_INFO* imageInfo = nullptr);
	void LoadXFile(char* folderName, char* fileName);
	void LoadSound(string keyName, const char* fileName, bool bgm = FALSE, bool loop = TRUE);

	bool LoadingDone();
	vector<LoadItem*> GetLoadItem() { return vLoadItem; }
};

