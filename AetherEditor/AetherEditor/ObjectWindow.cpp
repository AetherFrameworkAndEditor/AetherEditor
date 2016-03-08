#include "ObjectWindow.h"
#include <GameController.h>

using namespace aetherClass;
ObjectWindow::ObjectWindow()
{
}


ObjectWindow::~ObjectWindow()
{
}

LRESULT CALLBACK ObjectWindow::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	UINT flg = LOWORD(wParam);
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (flg == WA_ACTIVE || flg == WA_CLICKACTIVE){
			GameController::GetKey().ChangeActiveWindow(m_hWnd);
			GameController::GetMouse().ChangeActiveWindow(m_hWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return S_OK;
}
