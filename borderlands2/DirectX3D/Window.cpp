#include "stdafx.h"
#include "Window.h"


Window::Window()
{
}


Window::~Window()
{
}

LRESULT Window::WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

HWND Window::CreateWin(TCHAR * pStrTitleName, TCHAR * pStrAppName, HINSTANCE hInst, WNDPROC WndProc, DWORD dwStyle)
{
	return HWND();
}

void Window::CenterWindow(HWND hwnd)
{
}

void Window::SetWindowSize(int nX, int nY)
{
}

void Window::Run()
{
}
