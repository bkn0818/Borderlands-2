#pragma once

typedef Window* LPWINDOW;

class Window
{
protected:
	HWND					m_hWnd;						// ������ �ڵ�
	HMENU					m_hMenu;					// �޴� �ڵ�
	HINSTANCE				m_hInst;					// �ν��Ͻ�
	WNDPROC					m_WndProc;					// ������ ���ν���

	HWND					m_hWidthScroll;				// ��ũ�� ���� �ڵ�
	HWND					m_hHeightScroll;			// ��ũ�� ���� �ڵ�	
	int						m_iWidthScrollPos;			// ��ũ�� ���� ���� ��ġ
	int						m_iHeightScrollPos;			// ��ũ�� ���� ���� ��ġ
	int						m_iWidthScrollMax;			// ��ũ�� ���� �ִ�ġ
	int						m_iHeightScrollMax;			// ��ũ�� ���� �ִ�ġ

	TCHAR*					m_pStrAppName;				// ���ø����̼� �̸�
	TCHAR*					m_pStrTitleName;			// Ÿ��Ʋ �̸�

	int						m_iWindowWidthSize;			// �������� ���� ũ��
	int						m_iWindowHeightSize;		// �������� ���� ũ��
	RECT					m_rcWindowSize;				// �������� ��ü ����
	RECT					m_rcWindowClientSize;		// �������� Ŭ���̾�Ʈ ����

	POINT					m_ptMousePos;				// ���콺�� ��ǥ

public:
	Window();
	virtual ~Window();

	virtual LRESULT	WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND CreateWin(TCHAR* pStrTitleName = TEXT("Default"), TCHAR* pStrAppName = TEXT("Default"), HINSTANCE hInst = 0, WNDPROC WndProc = NULL, DWORD dwStyle = WS_OVERLAPPEDWINDOW);
	void CenterWindow(HWND hwnd);
	void SetWindowSize(int nX, int nY);

	// -----------------------------------------------------------------------------
	// Dest: Window Ŭ������ ��ӹ��� Ŭ������ ���������Լ��� �ݵ�� �������ؾ��Ѵ�.
	// -----------------------------------------------------------------------------
	virtual bool Init() = 0;
	virtual bool Update() = 0;
	virtual bool Render() = 0;
	virtual void Run();
};

