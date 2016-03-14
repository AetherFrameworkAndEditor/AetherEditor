#include "PropertyWindow.h"
#include <Windows.h>
#define ID_BUTTON 100
PropertyWindow::PropertyWindow()
{
}


PropertyWindow::~PropertyWindow()
{
}
HWND m_hWndEdit;
LRESULT CALLBACK  PropertyWindow::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	UINT flg = LOWORD(wParam);
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BUTTON) {
			TCHAR szBuf[256];

			GetWindowText(m_hWndEdit, szBuf, sizeof(szBuf) / sizeof(TCHAR));
			MessageBox(NULL, szBuf, TEXT("OK"), MB_OK);
			
		}
		return 0;
	default:
	
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return S_OK;
}

void PropertyWindow::OnCreate(){
	static SCROLLINFO scr;
	static TCHAR strScroll[32];

	
	scr.cbSize = sizeof(SCROLLINFO);
	scr.fMask = SIF_PAGE | SIF_RANGE;
	scr.nMin = 0;	scr.nMax = 9;
	scr.nPage = 2;

	SetScrollInfo(m_hWnd, SB_VERT, &scr, TRUE);
	scr.fMask = SIF_POS;

	m_hWndEdit = CreateWindowEx(0, TEXT("EDIT"), TEXT("name"),
		WS_CHILD,0,	0,180,150,m_hWnd,NULL,GetModuleHandle(NULL),NULL);
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);

}