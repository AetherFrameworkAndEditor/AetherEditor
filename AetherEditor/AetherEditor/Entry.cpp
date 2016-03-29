#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
// 標準
#include <memory>
#include <windows.h>

// 独自フレームワーク
#include <GameFrame.h>
#include <ConsoleWindow.h>
#include <WindowBase.h>
#include <GameSceneManager.h>
using namespace aetherClass;

#include "WorldReader.h"
#include "ObjectWindow.h"
#include "SceneWindow.h"
#include "PropertyWindow.h"
#include "MainLoopScene.h"
namespace{
	const int kExit = 0;
	const int kError = -1;
}
INT WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE, LPSTR, INT){
	ConsoleWindow::Create();

	auto frame = std::make_unique<GameFrame>();
	WindowBase::WindowStyleDesc desc;

	bool result;
	Vector2 screenSize(800, 600);
	float screenCenterPos = (GetSystemMetrics(SM_CYSCREEN) - screenSize._y) / 2;
	WindowBase*window[3] = { new SceneWindow(), new ObjectWindow(), new PropertyWindow() };

	desc._windowStyle =  WS_OVERLAPPED;
	desc._classStyle = CS_NOCLOSE;
	window[1]->SetWindowStyles(desc);
	window[1]->Create(L"Game", screenSize, Vector2(100, screenCenterPos));
	ShowWindow(window[1]->GetWindowHandle(), SW_HIDE);

	window[2]->SetWindowStyles(desc);
	window[2]->Create(L"Property", Vector2(310, 330),Vector2(900,130));

	desc._classStyle = NULL;
	desc._windowStyle += WS_SYSMENU;
	window[0]->SetWindowStyles(desc);
	window[0]->Create(L"Scene", screenSize,Vector2(100,screenCenterPos));

	result = frame->Initialize(window,2, 100000,1);
	if (!result)
	{
		return kError;
	}
	frame->BackgroundColor(Color(0.5, 0.5, 0.5, 1));
	auto currentScene = std::make_unique<MainLoopScene>();
	GameSceneManager::SetCurrentScene(currentScene.get());

	frame->GameRun();

	ConsoleWindow::Close();

	// 破棄	

	for (int i = 0; i < 3; ++i){
		delete window[i];
	}
	WorldObjectManager::Reset();
	currentScene.release();
	return kExit;
}