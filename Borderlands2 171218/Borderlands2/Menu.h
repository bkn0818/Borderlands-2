#ifndef MENU_H
#define MENU_H

#pragma once
#include "resource.h"
#include <commdlg.h>
#include "Plane.h"

class Menu
{
private:
	HWND				m_hWnd;
	HWND				m_hPlane;
	Plane*				m_plane;
	D3DXVECTOR3*		m_pScale;

	float				m_fLODRatio;

public:
	Menu();

	HRESULT InitMenu(HWND hWnd, HWND hDlg, Plane* plane, LPDIRECT3DDEVICE9 device, D3DXVECTOR3* pScale, float fLODRatio);

	void PlaneMenu(WPARAM wParam);
	void PlaneScroll(WPARAM wParam, LPARAM lParam);

	void TextureMenu(WPARAM wParam);
	char* WCharToChar(const wchar_t* pwstrSrc);
};

#endif