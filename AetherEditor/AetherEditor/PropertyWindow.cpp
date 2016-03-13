#include "PropertyWindow.h"
#include <Windows.h>
#define ID_BUTTON 100
PropertyWindow::PropertyWindow()
{
}


PropertyWindow::~PropertyWindow()
{
}

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
	
	m_hWndEdit = CreateWindowEx(
		0,
		TEXT("EDIT"),
		TEXT("名前入力"),					// ウインドウのタイトル名
		WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | 
		WS_BORDER|WS_CHILD,		// ウインドウスタイル
		0,												// ウィンドウの表示X座標
		0,												// ウィンドウの表示Y座標
		180,						// ウィンドウの幅
		150,						// 高さ
		m_hWnd,														// 親ウインドウ
		NULL,														// ウインドウメニュー
		GetModuleHandle(NULL),													// インスタンスハンドル
		NULL);													 // WM_CREATE情報
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);

}