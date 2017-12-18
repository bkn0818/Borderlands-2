





//#include <imm.h>
//#include <InitGuid.h>
//#include <dinput.h>

#include "stdafx.h"
#include "InputDef.h"
#include "InputManager.h"


//BOOL CALLBACK EnumObjectsCallback ( const DIDEVICEOBJECTINSTANCE* pdidoi,
//	VOID* pContext )
//{
//	UNREFERENCED_PARAMETER ( pContext );
//	return g_pInput->SetProperty ( pdidoi );
//}


InputManager::InputManager ()
	: m_pDI ( NULL )
	, m_pKeyboard ( NULL )
	, m_hWndMainWindow ( NULL )
{
}

InputManager::~InputManager ()
{
}


HRESULT InputManager::Init ( HWND hWnd, DWORD dwCoopFlags )
{
	m_hWndMainWindow = hWnd;

	ZeroMemory ( m_DIKS, sizeof ( m_DIKS ) );
	ZeroMemory ( m_DIKSOld, sizeof ( m_DIKSOld ) );
	ZeroMemory ( m_bKeyStates, sizeof ( m_bKeyStates ) );
	ZeroMemory ( m_bKeyStatesOld, sizeof ( m_bKeyStatesOld ) );

	ZeroMemory ( m_dwDIKSTable, sizeof ( m_dwDIKSTable ) );
	ZeroMemory ( m_dwVKTable, sizeof ( m_bKeyStates ) );

	SetKeyTable ();


	HRESULT hr;

	// Create a DInput object
	if ( FAILED ( hr = DirectInput8Create ( GetModuleHandle ( NULL ), DIRECTINPUT_VERSION,
		IID_IDirectInput8, ( VOID** ) &m_pDI, NULL ) ) )
		return hr;

	// Obtain an interface to the system keyboard device.
	if ( FAILED ( hr = m_pDI->CreateDevice ( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
		return hr;

	if ( FAILED ( hr = m_pKeyboard->SetDataFormat ( &c_dfDIKeyboard ) ) )
		return hr;

	if ( FAILED ( hr = m_pKeyboard->SetCooperativeLevel ( m_hWndMainWindow, dwCoopFlags ) ) )
		return hr;

	m_pKeyboard->Acquire ();


	return S_OK;
}


void InputManager::FreeDirectInput ()
{
	if ( m_pKeyboard )
		m_pKeyboard->Unacquire ();

	SAFE_RELEASE ( m_pKeyboard );
	SAFE_RELEASE ( m_pDI );
}

void InputManager::Update ()
{
	if ( FAILED ( g_pInput->ReadImmediateData () ) )
		return;

	IsKey ( SPKL_LEFT, &keyStateLeft );
	IsKey ( SPKL_BACKSPACE, &keyStateBackSpace );
}

bool InputManager::WndProc ( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message ) {
		case WM_ACTIVATE:
			if ( WA_INACTIVE != wParam && m_pKeyboard )
			{
				m_pKeyboard->Acquire ();
			}
			break;
	}
	return false;
}

void InputManager::SetKeyTable ()
{
	//*************************************************************************
	// DirectInput keyboard scan codes Table Set.
	//*************************************************************************
	m_dwDIKSTable[SPKL_LEFT] = DIK_LEFT;
	m_dwDIKSTable[SPKL_RIGHT] = DIK_RIGHT;
	m_dwDIKSTable[SPKL_UP] = DIK_UP;
	m_dwDIKSTable[SPKL_DOWN] = DIK_DOWN;
	m_dwDIKSTable[SPKL_CTRL] = DIK_LCONTROL;
	m_dwDIKSTable[SPKL_LCTRL] = DIK_LCONTROL;
	m_dwDIKSTable[SPKL_RCTRL] = DIK_RCONTROL;
	m_dwDIKSTable[SPKL_SHIFT] = DIK_LSHIFT;
	m_dwDIKSTable[SPKL_LSHIFT] = DIK_LSHIFT;
	m_dwDIKSTable[SPKL_RSHIFT] = DIK_RSHIFT;
	m_dwDIKSTable[SPKL_ALT] = DIK_LALT;
	m_dwDIKSTable[SPKL_RALT] = DIK_RALT;
	m_dwDIKSTable[SPKL_LALT] = DIK_LALT;
	m_dwDIKSTable[SPKL_SPACE] = DIK_SPACE;
	m_dwDIKSTable[SPKL_EQUALS] = DIK_EQUALS;
	m_dwDIKSTable[SPKL_BACKSPACE] = DIK_BACK;
	m_dwDIKSTable[SPKL_ENTER] = DIK_RETURN;
	m_dwDIKSTable[SPKL_INSERT] = DIK_INSERT;
	m_dwDIKSTable[SPKL_DELETE] = DIK_DELETE;
	m_dwDIKSTable[SPKL_PGUP] = DIK_PGUP;
	m_dwDIKSTable[SPKL_PGDN] = DIK_PGDN;
	m_dwDIKSTable[SPKL_HOME] = DIK_HOME;
	m_dwDIKSTable[SPKL_END] = DIK_END;
	m_dwDIKSTable[SPKL_PLUS] = DIK_ADD;
	m_dwDIKSTable[SPKL_MINUS] = DIK_MINUS;
	m_dwDIKSTable[SPKL_MULTIPLY] = DIK_MULTIPLY;
	m_dwDIKSTable[SPKL_ESCAPE] = DIK_ESCAPE;
	m_dwDIKSTable[SPKL_SEMICOLON] = DIK_SEMICOLON;
	m_dwDIKSTable[SPKL_PERIOD] = DIK_PERIOD;
	m_dwDIKSTable[SPKL_COMMA] = DIK_COMMA;
	m_dwDIKSTable[SPKL_DIVIDE] = DIK_DIVIDE;
	m_dwDIKSTable[SPKL_TAB] = DIK_TAB;
	m_dwDIKSTable[SPKL_CAPSLOCK] = DIK_CAPITAL;
	m_dwDIKSTable[SPKL_SUBTRACT] = DIK_SUBTRACT;
	m_dwDIKSTable[SPKL_SLASH] = DIK_SLASH;
	m_dwDIKSTable[SPKL_APOSTROPHE] = DIK_APOSTROPHE;
	m_dwDIKSTable[SPKL_GRAVE] = DIK_GRAVE;
	m_dwDIKSTable[SPKL_NUMPAD0] = DIK_NUMPAD0;
	m_dwDIKSTable[SPKL_NUMPAD1] = DIK_NUMPAD1;
	m_dwDIKSTable[SPKL_NUMPAD2] = DIK_NUMPAD2;
	m_dwDIKSTable[SPKL_NUMPAD3] = DIK_NUMPAD3;
	m_dwDIKSTable[SPKL_NUMPAD4] = DIK_NUMPAD4;
	m_dwDIKSTable[SPKL_NUMPAD5] = DIK_NUMPAD5;
	m_dwDIKSTable[SPKL_NUMPAD6] = DIK_NUMPAD6;
	m_dwDIKSTable[SPKL_NUMPAD7] = DIK_NUMPAD7;
	m_dwDIKSTable[SPKL_NUMPAD8] = DIK_NUMPAD8;
	m_dwDIKSTable[SPKL_NUMPAD9] = DIK_NUMPAD9;
	m_dwDIKSTable[SPKL_0] = DIK_0;
	m_dwDIKSTable[SPKL_1] = DIK_1;
	m_dwDIKSTable[SPKL_2] = DIK_2;
	m_dwDIKSTable[SPKL_3] = DIK_3;
	m_dwDIKSTable[SPKL_4] = DIK_4;
	m_dwDIKSTable[SPKL_5] = DIK_5;
	m_dwDIKSTable[SPKL_6] = DIK_6;
	m_dwDIKSTable[SPKL_7] = DIK_7;
	m_dwDIKSTable[SPKL_8] = DIK_8;
	m_dwDIKSTable[SPKL_9] = DIK_9;
	m_dwDIKSTable[SPKL_A] = DIK_A;
	m_dwDIKSTable[SPKL_B] = DIK_B;
	m_dwDIKSTable[SPKL_C] = DIK_C;
	m_dwDIKSTable[SPKL_D] = DIK_D;
	m_dwDIKSTable[SPKL_E] = DIK_E;
	m_dwDIKSTable[SPKL_F] = DIK_F;
	m_dwDIKSTable[SPKL_G] = DIK_G;
	m_dwDIKSTable[SPKL_H] = DIK_H;
	m_dwDIKSTable[SPKL_I] = DIK_I;
	m_dwDIKSTable[SPKL_J] = DIK_J;
	m_dwDIKSTable[SPKL_K] = DIK_K;
	m_dwDIKSTable[SPKL_L] = DIK_L;
	m_dwDIKSTable[SPKL_M] = DIK_M;
	m_dwDIKSTable[SPKL_N] = DIK_N;
	m_dwDIKSTable[SPKL_O] = DIK_O;
	m_dwDIKSTable[SPKL_P] = DIK_P;
	m_dwDIKSTable[SPKL_Q] = DIK_Q;
	m_dwDIKSTable[SPKL_R] = DIK_R;
	m_dwDIKSTable[SPKL_S] = DIK_S;
	m_dwDIKSTable[SPKL_T] = DIK_T;
	m_dwDIKSTable[SPKL_U] = DIK_U;
	m_dwDIKSTable[SPKL_V] = DIK_V;
	m_dwDIKSTable[SPKL_W] = DIK_W;
	m_dwDIKSTable[SPKL_X] = DIK_X;
	m_dwDIKSTable[SPKL_Y] = DIK_Y;
	m_dwDIKSTable[SPKL_Z] = DIK_Z;
	m_dwDIKSTable[SPKL_F1] = DIK_F1;
	m_dwDIKSTable[SPKL_F2] = DIK_F2;
	m_dwDIKSTable[SPKL_F3] = DIK_F3;
	m_dwDIKSTable[SPKL_F4] = DIK_F4;
	m_dwDIKSTable[SPKL_F5] = DIK_F5;
	m_dwDIKSTable[SPKL_F6] = DIK_F6;
	m_dwDIKSTable[SPKL_F7] = DIK_F7;
	m_dwDIKSTable[SPKL_F8] = DIK_F8;
	m_dwDIKSTable[SPKL_F9] = DIK_F9;
	m_dwDIKSTable[SPKL_F10] = DIK_F10;
	m_dwDIKSTable[SPKL_F11] = DIK_F11;
	m_dwDIKSTable[SPKL_F12] = DIK_F12;
	m_dwDIKSTable[SPKL_PREVTRACK] = DIK_PREVTRACK;

	m_dwDIKSTable[SPKL_LBRACE] = DIK_LBRACKET;
	m_dwDIKSTable[SPKL_RBRACE] = DIK_RBRACKET;


	m_dwDIKSTable[SPKL_PRINT_SCREEN] = DIK_SYSRQ;
	m_dwDIKSTable[SPKL_SCROLL_LOCK] = DIK_SCROLL;
	m_dwDIKSTable[SPKL_PAUSE_BREAK] = DIK_PAUSE;
	m_dwDIKSTable[SPKL_LWINDOW] = DIK_LWIN;
	m_dwDIKSTable[SPKL_RWINDOW] = DIK_RWIN;



	//*************************************************************************
	// Virtual Keys, Standard Table Set.
	//*************************************************************************

	m_dwVKTable[SPKL_LEFT] = VK_LEFT;
	m_dwVKTable[SPKL_RIGHT] = VK_RIGHT;
	m_dwVKTable[SPKL_UP] = VK_UP;
	m_dwVKTable[SPKL_DOWN] = VK_DOWN;
	m_dwVKTable[SPKL_CTRL] = VK_LCONTROL;
	m_dwVKTable[SPKL_ALT] = VK_MENU;
	m_dwVKTable[SPKL_LCTRL] = VK_LCONTROL;
	m_dwVKTable[SPKL_RCTRL] = VK_RCONTROL;
	m_dwVKTable[SPKL_SHIFT] = VK_LSHIFT;
	m_dwVKTable[SPKL_LSHIFT] = VK_LSHIFT;
	m_dwVKTable[SPKL_RSHIFT] = VK_RSHIFT;
	m_dwVKTable[SPKL_SPACE] = VK_SPACE;
	m_dwVKTable[SPKL_BACKSPACE] = VK_BACK;
	m_dwVKTable[SPKL_ENTER] = VK_RETURN;
	m_dwVKTable[SPKL_INSERT] = VK_INSERT;
	m_dwVKTable[SPKL_DELETE] = VK_DELETE;
	m_dwVKTable[SPKL_HOME] = VK_HOME;
	m_dwVKTable[SPKL_END] = VK_END;
	m_dwVKTable[SPKL_PLUS] = VK_ADD;
	m_dwVKTable[SPKL_MULTIPLY] = VK_MULTIPLY;
	m_dwVKTable[SPKL_ESCAPE] = VK_ESCAPE;
	m_dwVKTable[SPKL_DIVIDE] = VK_DIVIDE;
	m_dwVKTable[SPKL_TAB] = VK_TAB;
	m_dwVKTable[SPKL_CAPSLOCK] = VK_CAPITAL;
	m_dwVKTable[SPKL_SUBTRACT] = VK_SUBTRACT;
	m_dwVKTable[SPKL_NUMPAD0] = VK_NUMPAD0;
	m_dwVKTable[SPKL_NUMPAD1] = VK_NUMPAD1;
	m_dwVKTable[SPKL_NUMPAD2] = VK_NUMPAD2;
	m_dwVKTable[SPKL_NUMPAD3] = VK_NUMPAD3;
	m_dwVKTable[SPKL_NUMPAD4] = VK_NUMPAD4;
	m_dwVKTable[SPKL_NUMPAD5] = VK_NUMPAD5;
	m_dwVKTable[SPKL_NUMPAD6] = VK_NUMPAD6;
	m_dwVKTable[SPKL_NUMPAD7] = VK_NUMPAD7;
	m_dwVKTable[SPKL_NUMPAD8] = VK_NUMPAD8;
	m_dwVKTable[SPKL_NUMPAD9] = VK_NUMPAD9;
	m_dwVKTable[SPKL_F1] = VK_F1;
	m_dwVKTable[SPKL_F2] = VK_F2;
	m_dwVKTable[SPKL_F3] = VK_F3;
	m_dwVKTable[SPKL_F4] = VK_F4;
	m_dwVKTable[SPKL_F5] = VK_F5;
	m_dwVKTable[SPKL_F6] = VK_F6;
	m_dwVKTable[SPKL_F7] = VK_F7;
	m_dwVKTable[SPKL_F8] = VK_F8;
	m_dwVKTable[SPKL_F9] = VK_F9;
	m_dwVKTable[SPKL_F10] = VK_F10;
	m_dwVKTable[SPKL_F11] = VK_F11;
	m_dwVKTable[SPKL_F12] = VK_F12;
}

HRESULT InputManager::ReadImmediateData ()
{
	HRESULT hr;
	if ( NULL == m_pKeyboard )
		return S_OK;

	CopyMemory ( m_DIKSOld, m_DIKS, DIKS_SIZE );
	ZeroMemory ( m_DIKS, DIKS_SIZE );

	hr = m_pKeyboard->GetDeviceState ( sizeof ( m_DIKS ), m_DIKS );
	if ( FAILED ( hr ) )
	{
		hr = m_pKeyboard->Acquire ();
		while ( hr == DIERR_INPUTLOST )
			hr = m_pKeyboard->Acquire ();


		if ( hr == DIERR_OTHERAPPHASPRIO || hr == DIERR_NOTACQUIRED ) {
			m_pKeyboard->Unacquire ();
		}

		return S_OK;
	}

	// ·Î±×...
	m_strNewText.clear ();	
	char strElement[128];
	for ( int i = 0; i < 256; i++ )
	{
		if ( m_DIKS[i] & 0x80 )
		{
			sprintf ( strElement, "0x%02x ", i );
			m_strNewText += strElement;
		}
	}
	return S_OK;
}




bool InputManager::IsKey ( int KeyCode, BYTE* pEvent )
{
	if ( m_DIKS[m_dwDIKSTable[KeyCode]] & 0x80 )
	{
		if ( m_DIKSOld[m_dwDIKSTable[KeyCode]] & 0x80 ) {
			if ( pEvent )
				*pEvent = INPUT_PRESS_REPEAT;
			return false;
		}
		else {
			if ( pEvent )
				*pEvent = INPUT_PRESS;
			return true;
		}
	}
	else {
		if ( m_DIKSOld[m_dwDIKSTable[KeyCode]] & 0x80 ) {
			if ( pEvent )
				*pEvent = INPUT_UNPRESS;
		}
		else {
			if ( pEvent )
				*pEvent = INPUT_UNPRESS_REPEAT;
		}
	}
	return false;
}


bool InputManager::ConvertProcessKey ( unsigned int* puiVirKeyCode )
{
	unsigned int i;
	for ( i = 'A'; i <= 'Z'; ++i ) {
		if ( GetAsyncKeyState ( i )  < 0 ) {
			*puiVirKeyCode = i;
			return true;
		}
	}

	for ( i = 'a'; i <= 'z'; ++i ) {
		if ( GetAsyncKeyState ( i ) < 0 ) {
			*puiVirKeyCode = i;
			return true;
		}
	}

	if ( GetAsyncKeyState ( VK_NUMPAD0 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD0; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD1 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD1; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD2 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD2; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD3 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD3; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD4 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD4; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD5 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD5; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD6 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD6; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD7 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD7; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD8 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD8; return true; }
	if ( GetAsyncKeyState ( VK_NUMPAD9 ) < 0 ) { *puiVirKeyCode = VK_NUMPAD9; return true; }
	if ( GetAsyncKeyState ( VK_MULTIPLY ) < 0 ) { *puiVirKeyCode = VK_MULTIPLY; return true; }
	if ( GetAsyncKeyState ( VK_ADD ) < 0 ) { *puiVirKeyCode = VK_ADD; return true; }
	if ( GetAsyncKeyState ( VK_SEPARATOR ) < 0 ) { *puiVirKeyCode = VK_SEPARATOR; return true; }
	if ( GetAsyncKeyState ( VK_SUBTRACT ) < 0 ) { *puiVirKeyCode = VK_SUBTRACT; return true; }
	if ( GetAsyncKeyState ( VK_DECIMAL ) < 0 ) { *puiVirKeyCode = VK_DECIMAL; return true; }
	if ( GetAsyncKeyState ( VK_DIVIDE ) < 0 ) { *puiVirKeyCode = VK_DIVIDE; return true; }
	if ( GetAsyncKeyState ( VK_OEM_1 ) < 0 ) { *puiVirKeyCode = VK_OEM_1; return true; }
	if ( GetAsyncKeyState ( VK_OEM_PLUS ) < 0 ) { *puiVirKeyCode = VK_OEM_PLUS; return true; }
	if ( GetAsyncKeyState ( VK_OEM_COMMA ) < 0 ) { *puiVirKeyCode = VK_OEM_COMMA; return true; }
	if ( GetAsyncKeyState ( VK_OEM_MINUS ) < 0 ) { *puiVirKeyCode = VK_OEM_MINUS; return true; }
	if ( GetAsyncKeyState ( VK_OEM_PERIOD ) < 0 ) { *puiVirKeyCode = VK_OEM_PERIOD; return true; }
	if ( GetAsyncKeyState ( VK_OEM_2 ) < 0 ) { *puiVirKeyCode = VK_OEM_2; return true; }
	if ( GetAsyncKeyState ( VK_OEM_3 ) < 0 ) { *puiVirKeyCode = VK_OEM_3; return true; }
	if ( GetAsyncKeyState ( VK_OEM_4 ) < 0 ) { *puiVirKeyCode = VK_OEM_4; return true; }
	if ( GetAsyncKeyState ( VK_OEM_5 ) < 0 ) { *puiVirKeyCode = VK_OEM_5; return true; }
	if ( GetAsyncKeyState ( VK_OEM_6 ) < 0 ) { *puiVirKeyCode = VK_OEM_6; return true; }
	if ( GetAsyncKeyState ( VK_OEM_7 ) < 0 ) { *puiVirKeyCode = VK_OEM_7; return true; }

	*puiVirKeyCode = VK_PROCESSKEY;
	return false;
}


void InputManager::UpdateControlKeyState ()
{
	m_bKeyStates[VK_SHIFT] = ( GetAsyncKeyState ( VK_SHIFT ) < 0 ) ? true : false;
	m_bKeyStates[VK_CONTROL] = ( GetAsyncKeyState ( VK_CONTROL ) < 0 ) ? true : false;
	m_bKeyStates[VK_MENU] = ( GetAsyncKeyState ( VK_MENU ) < 0 ) ? true : false;
	m_bKeyStates[VK_LWIN] = ( GetAsyncKeyState ( VK_LWIN ) < 0 ) ? true : false;
	m_bKeyStates[VK_RWIN] = ( GetAsyncKeyState ( VK_RWIN ) < 0 ) ? true : false;
	m_bKeyStates[VK_TAB] = ( GetAsyncKeyState ( VK_TAB ) < 0 ) ? true : false;
}















