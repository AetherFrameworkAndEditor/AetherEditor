#ifndef _SCENEWINDOW_H
#define _SCENEWINDOW_H

#include "WindowBase.h"
#include"Font.h"
#include<memory>
	class SceneWindow :
		public aetherClass::WindowBase
	{
	public:
		SceneWindow();
		~SceneWindow();
	private:
		LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)override;
		void OnCreate()override;
		void UpdateCommand(int);
		std::string OpenDialogBox();
		std::shared_ptr<aetherClass::Font>m_font;
	};

#endif