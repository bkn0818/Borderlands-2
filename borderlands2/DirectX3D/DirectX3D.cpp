//#include "stdafx.h"
//#include "DirectX3D.h"
//#include "VideoDevice.h"
//
//#define MAX_LOADSTRING 100
//
//// 전역 변수:
//HINSTANCE hInst;                               
//WCHAR szTitle[MAX_LOADSTRING];                 
//WCHAR szWindowClass[MAX_LOADSTRING];   
//
//HWND		g_HWND_Window;
//POINT		g_ptMouse;
//MainGame*	g_pMainGame;
//HINSTANCE	g_hInst;
//INT			g_cmdShow;
//
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//					  _In_opt_ HINSTANCE hPrevInstance,
//					  _In_ LPWSTR    lpCmdLine,
//					  _In_ int       nCmdShow)
//{
//	//콘솔창 띄우기
//	//AllocConsole();
//	//freopen("CONOUT$", "wt", stdout);
//
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadStringW(hInstance, IDC_DIRECTX3D, szWindowClass, MAX_LOADSTRING);
//
//	MyRegisterClass(hInstance);
//
//	if (!InitInstance(hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	
//	VideoDevice::GetInstance()->Init();
//	SceneManager::GetInstance()->Init();
//	g_pMainGame = new MainGame;
//	g_pMainGame->Init();
//
//	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX3D));
//	MSG msg;
//
//	while (true)
//	{
//		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//			if (msg.message == WM_QUIT) {
//				break;
//			}
//			else
//			{
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//		}
//		else
//		{
//			g_pMainGame->Update();
//			VideoDevice::GetInstance()->RenderBegin();
//			g_pMainGame->Render();
//			VideoDevice::GetInstance()->RenderEnd();
//			g_pAutoReleasePool->Drain();
//		}
//	}
//	g_pMainGame->Release();
//	g_pTextureManager->Destroy();
//	g_pFontManager->Destroy();
//	VideoDevice::GetInstance()->Destroy();
//	g_pSceneManager->Release();
//
//	//delete VideoDevice;
//
//	return (int)msg.wParam;
//}
//
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEXW wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX3D));
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECTX3D);
//	wcex.lpszClassName = szWindowClass;
//	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassExW(&wcex);
//}
//
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//	g_hInst = hInstance;
//	g_cmdShow = nCmdShow;
//
//	HWND _hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//							   0, 0, 1280, 768, nullptr, nullptr, hInstance, nullptr);
//
//
//	if (!_hWnd) return FALSE;
//
//	g_HWND_Window = _hWnd;
//
//	ShowWindow(g_HWND_Window, nCmdShow);
//	UpdateWindow(g_HWND_Window);
//
//
//	return TRUE;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	if (g_pMainGame) g_pMainGame->MainProc(hWnd, message, wParam, lParam);
//
//	switch (message)
//	{
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_DESTROY, wParam, lParam);
//		break;
//	case WM_MOUSEMOVE:
//		g_ptMouse.x = static_cast<LONG>LOWORD(lParam);
//		g_ptMouse.y = static_cast<LONG>HIWORD(lParam);
//
//		break;
//	case WM_COMMAND:
//	{
//		int wmId = LOWORD(wParam);
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//	}
//	break;
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hWnd, &ps);
//		EndPaint(hWnd, &ps);
//	}
//	break;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}

// DirectX3D.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DirectX3D.h"

#include "VideoDevice.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


HWND g_HWND_Window;
HINSTANCE g_hInst;
MainGame* g_pMainGame;
POINT g_ptMouse;


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DIRECTX3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	VideoDevice::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	g_pMainGame = new MainGame;
	g_pMainGame->Init();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX3D));

	MSG msg;

	// 기본 메시지 루프입니다.
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else
		{
			g_pMainGame->Update();

			VideoDevice::GetInstance()->RenderBegin();

			g_pMainGame->Render();

			VideoDevice::GetInstance()->RenderEnd();

			g_pAutoReleasePool->Drain();
		}
	}


	g_pMainGame->Release();

	g_pTextureManager->Destroy();
	g_pFontManager->Destroy();
	VideoDevice::GetInstance()->Destroy();

	//delete VideoDevice;

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX3D));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCEW(IDC_DIRECTX3D);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance;

	HWND _hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
							   0, 0, 1280, 768, nullptr, nullptr, hInstance, nullptr);


	if (!_hWnd) return FALSE;

	g_HWND_Window = _hWnd;

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);


	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_pMainGame) g_pMainGame->MainProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_DESTROY, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = static_cast<LONG>LOWORD(lParam);
		g_ptMouse.y = static_cast<LONG>HIWORD(lParam);

		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}