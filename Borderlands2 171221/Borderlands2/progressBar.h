#pragma once
class progressBar
{
private:
	D3DXIMAGE_INFO progressBarimgInfo;
	UIObject		progressBarRoot;
	LPD3DXSPRITE	progressBarsprite;

	const char * progressBarName;
	int progressBarCount;
	int digit;
	int printNum;
	int digit1000, digit100, digit10, digit1;
	const char * digitFullPath1000;
	const char * digitFullPath100;
	const char * digitFullPath10;
	const char * digitFullPath1;
	float MaxCount;
	float CurrentHP, CurrentBullet;

public:
	progressBar();
	~progressBar();

	void init(const char * fullPath, int x, int y, const char * bar, int value);
	void update();
	void render();
	void destroy();

	void isClick(int n);		//bullet bar
	void isAttacked(int n);		//hp bar
};

