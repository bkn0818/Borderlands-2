#pragma once

typedef Window* LPWINDOW;

class Window
{
protected:
	HWND					m_hWnd;						// 윈도우 핸들
	HMENU					m_hMenu;					// 메뉴 핸들
	HINSTANCE				m_hInst;					// 인스턴스
	WNDPROC					m_WndProc;					// 윈도우 프로시저

	HWND					m_hWidthScroll;				// 스크롤 가로 핸들
	HWND					m_hHeightScroll;			// 스크롤 세로 핸들	
	int						m_iWidthScrollPos;			// 스크롤 현재 가로 위치
	int						m_iHeightScrollPos;			// 스크롤 현재 세로 위치
	int						m_iWidthScrollMax;			// 스크롤 가로 최대치
	int						m_iHeightScrollMax;			// 스크롤 세로 최대치

	TCHAR*					m_pStrAppName;				// 어플리케이션 이름
	TCHAR*					m_pStrTitleName;			// 타이틀 이름

	int						m_iWindowWidthSize;			// 윈도우의 가로 크기
	int						m_iWindowHeightSize;		// 윈도우의 세로 크기
	RECT					m_rcWindowSize;				// 윈도우의 전체 영역
	RECT					m_rcWindowClientSize;		// 윈도우의 클라이언트 영역

	POINT					m_ptMousePos;				// 마우스의 좌표

public:
	Window();
	virtual ~Window();

	virtual LRESULT	WindowMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND CreateWin(TCHAR* pStrTitleName = TEXT("Default"), TCHAR* pStrAppName = TEXT("Default"), HINSTANCE hInst = 0, WNDPROC WndProc = NULL, DWORD dwStyle = WS_OVERLAPPEDWINDOW);
	void CenterWindow(HWND hwnd);
	void SetWindowSize(int nX, int nY);

	// -----------------------------------------------------------------------------
	// Dest: Window 클래스를 상속받을 클래스는 순수가상함수를 반드시 재정의해야한다.
	// -----------------------------------------------------------------------------
	virtual bool Init() = 0;
	virtual bool Update() = 0;
	virtual bool Render() = 0;
	virtual void Run();
};

