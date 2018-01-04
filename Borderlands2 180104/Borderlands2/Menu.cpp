#include "stdafx.h"
#include "Menu.h"

LPSTR m_pTex;
LPSTR m_lpstr;

Menu::Menu()
{
}

HRESULT Menu::InitMenu(HWND hWnd, HWND hDlg, Plane * plane, LPDIRECT3DDEVICE9 device, D3DXVECTOR3 * pScale, float fLODRatio)
{
	m_hWnd = hWnd;
	m_hPlane = hDlg;
	m_plane = plane;
	m_pScale = pScale;
	m_fLODRatio = fLODRatio;

	m_lpstr = ".\\Texture\\HeightMap\\height129.bmp";
	m_pTex = ".\\Texture\\Texture\\Mount.bmp";

	CheckRadioButton(m_hPlane, IDC_RADIO_POINT, IDC_RADIO_SOLID, IDC_RADIO_SOLID);

	SetScrollRange(GetDlgItem(m_hPlane, IDC_SCROLLBAR_PICKING), SB_CTL, 1, 100, true);
	SetScrollRange(GetDlgItem(m_hPlane, IDC_SCROLLBAR_BIGCIRCLE), SB_CTL, 1, 200, true);
	SetScrollRange(GetDlgItem(m_hPlane, IDC_SCROLLBAR_SMALLCIRCLE), SB_CTL, 1, 200, true);
	SetScrollRange(GetDlgItem(m_hPlane, IDC_SCROLLBAR_BIGVALUE), SB_CTL, 1, 100, true);
	SetScrollRange(GetDlgItem(m_hPlane, IDC_SCROLLBAR_SMALLVALUE), SB_CTL, 1, 100, true);

	SetDlgItemInt(m_hPlane, IDC_EDIT_PICKINGVALUE, m_plane->ReturnPickingValue(), 0);
	SetDlgItemInt(m_hPlane, IDC_EDIT_BIGCIRCLE, m_plane->ReturnPickingBig(), 0);
	SetDlgItemInt(m_hPlane, IDC_EDIT_SMALLCIRCLE, m_plane->ReturnPickingSmall(), 0);
	SetDlgItemInt(m_hPlane, IDC_EDIT_BIGVALUE, m_plane->ReturnBigValue(), 0);
	SetDlgItemInt(m_hPlane, IDC_EDIT_SMALLVALUE, m_plane->ReturnSmallValue(), 0);

	return S_OK;
}

void Menu::PlaneMenu(WPARAM wParam)
{
	OPENFILENAME ofn;
	TCHAR lpstrFile[MAX_PATH] = L"";

	switch (LOWORD(wParam))
	{
		case IDC_OPEN_HEIGHT:
			memset(&ofn, 0, sizeof(OPENFILENAME));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = L"BMP 파일(*.bmp)\0*.bmp\0";
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrInitialDir = L".\\Texture\\HeightMap\\";
			ofn.Flags = OFN_NOCHANGEDIR;

			if (GetOpenFileName(&ofn) != 0)
			{
				MessageBox(m_hWnd, L"높이맵 열기 성공", L"성공", MB_OK);

				m_lpstr = WCharToChar(ofn.lpstrFile);

				m_plane->InitPlane(m_pScale, m_fLODRatio, m_lpstr, m_pTex);
			}
			break;
		case IDC_OPEN_TEXTURE:
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFilter = L"모든 파일(*.*)\0*.*\0";
			ofn.lpstrFile = lpstrFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrInitialDir = L".\\Texture\\Texture\\";
			ofn.Flags = OFN_NOCHANGEDIR;

			if (GetOpenFileName(&ofn) != 0)
			{
				MessageBox(m_hWnd, L"텍스처 열기 성공", L"성공", MB_OK);

				m_lpstr = WCharToChar(ofn.lpstrFile);

				m_plane->InitPlane(m_pScale, m_fLODRatio, m_lpstr, m_pTex);
			}
			break;

		case IDC_RADIO_POINT:
			D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
			break;
		case IDC_RADIO_WIRE:
			D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		case IDC_RADIO_SOLID:
			D3DDEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		case IDC_PICKING_UP:
			m_plane->SetPickingUpDown(1);
			break;
		case IDC_PICKING_DOWN:
			m_plane->SetPickingUpDown(-1);
			break;
		case IDC_PICKING_NORMAL:
			m_plane->SetPickingUpDown(0);
			break;
	}
}

void Menu::PlaneScroll(WPARAM wParam, LPARAM lParam)
{
	UINT id = GetDlgCtrlID(HWND(lParam));

	switch (id)
	{
		case IDC_SCROLLBAR_PICKING:
			switch (LOWORD(wParam))
			{
				case SB_LINERIGHT:
					m_plane->SetPickingValue(min((m_plane->ReturnPickingValue() + 1), 100));
					break;
				case SB_LINELEFT:
					m_plane->SetPickingValue(max((m_plane->ReturnPickingValue() - 1), 1));
					break;
				case SB_PAGERIGHT:
					m_plane->SetPickingValue(min((m_plane->ReturnPickingValue() + 10), 100));
					break;
				case SB_PAGELEFT:
					m_plane->SetPickingValue(max((m_plane->ReturnPickingValue() - 10), 1));
					break;
				case SB_THUMBTRACK:
					m_plane->SetPickingValue(HIWORD(wParam));
					break;
			}
			SetDlgItemInt(m_hPlane, IDC_EDIT_PICKINGVALUE, m_plane->ReturnPickingValue(),
						  0);

			SetScrollPos(GetDlgItem(m_hPlane, IDC_SCROLLBAR_PICKING), SB_CTL,
						 m_plane->ReturnPickingValue(), TRUE);

			break;
		case IDC_SCROLLBAR_SMALLCIRCLE:
			switch (LOWORD(wParam))
			{
			case SB_LINERIGHT:
				m_plane->SetPickingSmall(min((m_plane->ReturnPickingSmall() + 1), 200));
				break;
			case SB_LINELEFT:
				m_plane->SetPickingSmall(max((m_plane->ReturnPickingSmall() - 1), 1));
				break;
			case SB_PAGERIGHT:
				m_plane->SetPickingSmall(min((m_plane->ReturnPickingSmall() + 10), 200));
				break;
			case SB_PAGELEFT:
				m_plane->SetPickingSmall(max((m_plane->ReturnPickingSmall() - 10), 1));
				break;
			case SB_THUMBTRACK:
				m_plane->SetPickingSmall(HIWORD(wParam));
				break;
			}

			if (m_plane->ReturnPickingBig() < m_plane->ReturnPickingSmall())
			{
				m_plane->SetPickingSmall(m_plane->ReturnPickingBig() - 1);
			}

			SetDlgItemInt(m_hPlane, IDC_EDIT_SMALLCIRCLE, m_plane->ReturnPickingSmall(), 0);

			SetScrollPos(GetDlgItem(m_hPlane, IDC_SCROLLBAR_SMALLCIRCLE), SB_CTL,
						 m_plane->ReturnPickingSmall(), TRUE);

			break;
		case IDC_SCROLLBAR_BIGVALUE:
			switch (LOWORD(wParam))
			{
			case SB_LINERIGHT:
				m_plane->SetBigValue(min((m_plane->ReturnBigValue() + 1), 100));
				break;
			case SB_LINELEFT:
				m_plane->SetBigValue(max((m_plane->ReturnBigValue() - 1), 1));
				break;
			case SB_PAGERIGHT:
				m_plane->SetBigValue(min((m_plane->ReturnBigValue() + 10), 100));
				break;
			case SB_PAGELEFT:
				m_plane->SetBigValue(max((m_plane->ReturnBigValue() - 10), 1));
				break;
			case SB_THUMBTRACK:
				m_plane->SetBigValue(HIWORD(wParam));
				break;
			}

			SetDlgItemInt(m_hPlane, IDC_EDIT_BIGVALUE, m_plane->ReturnBigValue(), 0);

			SetScrollPos(GetDlgItem(m_hPlane, IDC_SCROLLBAR_BIGVALUE), SB_CTL,
						 m_plane->ReturnBigValue(), TRUE);

			break;

		case IDC_SCROLLBAR_SMALLVALUE:
			switch (LOWORD(wParam))
			{
			case SB_LINERIGHT:
				m_plane->SetSmallValue(min((m_plane->ReturnSmallValue() + 1), 100));
				break;
			case SB_LINELEFT:
				m_plane->SetSmallValue(max((m_plane->ReturnSmallValue() - 1), 1));
				break;
			case SB_PAGERIGHT:
				m_plane->SetSmallValue(min((m_plane->ReturnSmallValue() + 10), 100));
				break;
			case SB_PAGELEFT:
				m_plane->SetSmallValue(max((m_plane->ReturnSmallValue() - 10), 1));
				break;
			case SB_THUMBTRACK:
				m_plane->SetSmallValue(HIWORD(wParam));
				break;
			}

			SetDlgItemInt(m_hPlane, IDC_EDIT_SMALLVALUE, m_plane->ReturnSmallValue(), 0);

			SetScrollPos(GetDlgItem(m_hPlane, IDC_SCROLLBAR_SMALLVALUE), SB_CTL,
						 m_plane->ReturnSmallValue(), TRUE);
			break;
	}
}

void Menu::TextureMenu(WPARAM wParam)
{
}

char * Menu::WCharToChar(const wchar_t * pwstrSrc)
{
	int len = 0;
	len = (wcslen(pwstrSrc) + 1) * 2;
	char* pstr = (char*)malloc(sizeof(char)*len);
	WideCharToMultiByte(949, 0, pwstrSrc, -1, pstr, len, nullptr, nullptr);

	return pstr;
}