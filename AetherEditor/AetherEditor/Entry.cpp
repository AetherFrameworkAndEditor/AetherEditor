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
using namespace aetherClass;

#include "WorldReader.h"

namespace{
	const int kExit = 0;
	const int kError = -1;
}
INT WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE, LPSTR, INT){
	ConsoleWindow::Create();
	auto entry = std::make_unique<GameEntry>();

	auto frame = std::make_unique<GameFrame>();

	bool result = entry->Initialize(false, L"Aether Editor");
	if (!result)
	{
		return kError;
	}
	
	result = frame->Initialize(*entry->GetWindowHandle(), POINT{ 800, 600 }, false, 1000000);
	if (!result)
	{
		return kError;
	}

	WorldReader red;
	red.Load("text\\test.aether");

	entry->SetGameFrame(frame.get());

	entry->GameRun();
	red.UnLoad();
	ConsoleWindow::Close();

	return kExit;
}