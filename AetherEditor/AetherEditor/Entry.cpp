#ifndef _AETHER_H
#define _AETHER_H
#pragma comment (lib,"Aether.lib")
#endif
// 標準
#include <memory>
#include <windows.h>

// 独自フレームワーク
#include <GameEntry.h>
#include <GameFrame.h>
#include <ConsoleWindow.h>
#include <WindowBase.h>
#include <GameSceneManager.h>
using namespace aetherClass;

#include "WorldReader.h"
#include "ObjectWindow.h"
#include"SceneWindow.h"
#include "MainLoopScene.h"
namespace{
	const int kExit = 0;
	const int kError = -1;
}
INT WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE, LPSTR, INT){
	ConsoleWindow::Create();

	auto frame = std::make_unique<GameFrame>();

	bool result;
	Vector2 screenSize(800, 600);
	WindowBase*window[3] = { new SceneWindow(), new ObjectWindow(), new WindowBase() };

	WindowBase::WindowStyleDesc desc;
	desc._windowStyle = WS_OVERLAPPED | WS_SYSMENU;

	window[0]->SetWindowStyles(desc);
	window[0]->Create(L"Scene", screenSize);

	desc._windowStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_MAXIMIZEBOX| WS_SYSMENU;
	desc._classStyle = CS_NOCLOSE;
	window[1]->SetWindowStyles(desc);
	window[1]->Create(L"Game", screenSize, Vector2(100, 100));

	window[2]->Create(L"Property", Vector2(100, 100));

	result = frame->Initialize(window,3, 1000000,1);
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
	currentScene.release();
	return kExit;
}