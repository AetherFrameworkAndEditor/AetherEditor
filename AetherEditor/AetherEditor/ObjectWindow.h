#ifndef _OBJECTWINDOW_H
#define _OBJECTWINDOW_H
/*
	Game��Scene�Ɏg��

*/
#include <WindowBase.h>
class ObjectWindow :public aetherClass::WindowBase
{
public:
	ObjectWindow();
	~ObjectWindow();
	LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};

#endif