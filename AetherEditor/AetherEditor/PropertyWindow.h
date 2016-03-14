#ifndef _PROPERTYWINDOW_H
#define _PROPERTYWINDOW_H
#include <WindowBase.h>
#include <string>
#include "WorldObjectManager.h"

class PropertyWindow :
	public aetherClass::WindowBase
{
public:
	PropertyWindow();
	~PropertyWindow();
	LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void OnCreate()override;

private:
	void CheckWorldObject();

	// 
	void SetName();
	void SetPosition();
	void SetRotation();
	void SetScale();
	void SetColor();
	void DeleteObject();

	// Createån
	void CreateNameEdit();
	void CreatePositionEdit();
	void CreateRotationEdit();
	void CreateScaleEdit();
	void CreateTextureEdit();
	void CreateMaterialEdit();
	void CreateColorEdit();
	void CreateDeleteEdit();
private:

	HWND m_inputNameEdit;
	HWND m_inputPositionEdit[3];
	HWND m_inputRotationEdit[3];
	HWND m_inputScaleEdit[3];
	HWND m_inputColorEdit[4];
	HWND m_inputTextureEdit;
	HWND m_inputMaterialEdit;

	CurrentSelectObject m_prevSelectObject;
};

#endif