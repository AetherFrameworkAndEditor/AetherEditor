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
	WindowBase*window[3] = { new ObjectWindow, new ObjectWindow, new WindowBase };

	WindowBase::WindowStyleDesc desc;
	desc._windowStyle = WS_OVERLAPPEDWINDOW;

	window[0]->SetWindowStyles(desc);
	window[0]->Create(L"Scene", screenSize);
	window[1]->Create(L"Game", screenSize);
	window[2]->Create(L"Property", Vector2(100,100));

	result = frame->Initialize(window,3, 1000000,1);
	if (!result)
	{
		return kError;
	}
	auto currentScene = std::make_unique<MainLoopScene>();
	GameSceneManager::SetCurrentScene(currentScene.get());

	//WorldReader red;
	//red.Load("text\\test.aether");
	//red.UnLoad();
	frame->GameRun();

	ConsoleWindow::Close();

	// 破棄	
	currentScene.release();
	delete[] window;
	return kExit;
}