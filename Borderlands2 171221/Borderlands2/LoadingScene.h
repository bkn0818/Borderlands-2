#pragma once

class Loading;
class SceneNode;

class LoadingScene : public SceneNode
{
private:
	Loading* loading;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);
	void LoadTexture();
	void LoadSound();
	void LoadXFile();

	LoadingScene();
	~LoadingScene();
};

