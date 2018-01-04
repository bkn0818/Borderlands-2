// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment : 
// Creation : metalgeni 2004-08-16 오후 4:48:46
//***************************************************************************

#pragma once

//#include <dinput.h>
//#include "InputDef.h"

#define g_pInput InputManager::GetInstance()

#define DIKS_SIZE 256


class InputManager {
	SINGLETON ( InputManager );
public:


	HRESULT Init ( HWND hWnd, DWORD dwCoopFlags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	void FreeDirectInput ();

	void Update ();

	
	bool WndProc ( UINT message, WPARAM wParam, LPARAM lParam );


	void SetKeyTable ();	
	HRESULT ReadImmediateData ();

	bool IsKey ( int KeyCode, BYTE* pEvent = NULL );	


	// 현재 눌려있는 키를 검색하여 puiVirKeyCode에 할당한다.
	bool ConvertProcessKey ( unsigned int* puiVirKeyCode );

	//VK_SHIFT, VK_CONTROL, VK_MENU, VK_LWIN ,VK_RWIN, VK_TAB 가 눌려있는 상태를 업데이트한다.		
	void UpdateControlKeyState ();

	bool GetKeyState ( int iSPKLCode ) { return m_bKeyStates[m_dwVKTable[iSPKLCode]]; }

	LPCSTR GetStateLog () { return m_strNewText.c_str (); }


	std::string&  GetKeyLog () {	return m_strNewText;	}

protected:

	BYTE	m_DIKS[DIKS_SIZE];
	BYTE	m_DIKSOld[DIKS_SIZE];
	BYTE	m_DIKSState[DIKS_SIZE];

	bool	m_bKeyStates[DIKS_SIZE];
	bool	m_bKeyStatesOld[DIKS_SIZE];

	DWORD	m_dwDIKSTable[DIKS_SIZE];
	DWORD	m_dwVKTable[DIKS_SIZE];

	HWND	m_hWndMainWindow;

	LPDIRECTINPUT8       m_pDI;			// The DirectInput object
	LPDIRECTINPUTDEVICE8 m_pKeyboard;	// The keyboard device 

	// Debug Log
	std::string		m_strNewText;


public:
	BYTE	keyStateLeft;
	BYTE	keyStateBackSpace;

};


