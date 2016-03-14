#ifndef _PROPERTYWINDOW_H
#define _PROPERTYWINDOW_H
#include <WindowBase.h>
class PropertyWindow :
	public aetherClass::WindowBase
{
public:
	PropertyWindow();
	~PropertyWindow();
	LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void OnCreate()override;
};

#endif