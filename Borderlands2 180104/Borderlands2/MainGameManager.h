#pragma once


class MainGameManager
{
	SINGLETON(MainGameManager);

	void Init ();
	void Update ();
	void Render ( );
	void WndProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void Destroy();

	void LogRender ();

private:

	void SetLight ();

	MainScene*		mainScene;
	GameScene*		gameScene;
	MaptoolScene*	maptoolScene;
	LoadingScene*	loadingScene;

	LPD3DXFONT		keyTestFont;

	DWORD 			textFormat = DT_LEFT | DT_TOP | DT_NOCLIP;
	DWORD 			color = D3DCOLOR_XRGB ( 255, 255, 255 );
	DWORD			x, y;
};

