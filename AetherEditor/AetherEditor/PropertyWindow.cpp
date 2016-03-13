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
		TEXT("���O����"),					// �E�C���h�E�̃^�C�g����
		WS_OVERLAPPED | WS_CHILD | WS_VISIBLE | 
		WS_BORDER|WS_CHILD,		// �E�C���h�E�X�^�C��
		0,												// �E�B���h�E�̕\��X���W
		0,												// �E�B���h�E�̕\��Y���W
		180,						// �E�B���h�E�̕�
		150,						// ����
		m_hWnd,														// �e�E�C���h�E
		NULL,														// �E�C���h�E���j���[
		GetModuleHandle(NULL),													// �C���X�^���X�n���h��
		NULL);													 // WM_CREATE���
	ShowWindow(m_hWndEdit, SW_SHOW);
	UpdateWindow(m_hWndEdit);

}