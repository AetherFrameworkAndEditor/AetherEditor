
#include <iostream>


#include "MainLoopScene.h"
#include "GameWindowView.h"
#include "SceneWindowView.h"
#include "PropertyWindowView.h"
using namespace aetherClass;

//
std::wstring MainLoopScene::m_windowNameArrya[] = {
	L"Scene",
	L"Game",
	L"Property"
};
//
MainLoopScene::MainLoopScene() :
GameScene("MainLoop", GetManager())
{
}


MainLoopScene::~MainLoopScene()
{
}

bool MainLoopScene::Initialize(){
	Finalize();
	// scene
	auto sceneWindowView = new SceneWindowView();
	if (!sceneWindowView)
	{
		return false;
	}
	m_viewWindow.push_back(sceneWindowView);

	// game
	auto gameWindowView = new GameWindowView();
	if (!gameWindowView)
	{
		return false;
	}
	m_viewWindow.push_back(gameWindowView);

	// property
	auto propertyWindowView = new PropertyWindowView();
	if (!propertyWindowView)
	{
		return false;
	}
	m_viewWindow.push_back(propertyWindowView);

	bool result = false;
	// ‘S‚Ä‚ÌƒV[ƒ“‚Ì‰Šú‰»
	for (auto& scene : m_viewWindow)
	{
		if (scene)
		{
			result = scene->Initialize();
			if (!result)
			{
				return false;
			}
		}
	}
	return true;
}

//
void MainLoopScene::Finalize(){
	for (auto& scene : m_viewWindow)
	{
		if (scene)
		{
			scene->Finalize();
			delete scene;
			scene = nullptr;
		}
	}
	m_viewWindow.clear();
}


//
bool MainLoopScene::Updater(){
	bool result = false;
	for (auto scene : m_viewWindow)
	{
		result = scene->Updater();
		if (!result)
		{
			return false;
		}
	}
	return true;
}

//
void MainLoopScene::Render(){
	DirectXEntity entity;
	int nameArrayNumber = 0;
	for (auto scene : m_viewWindow)
	{
		entity.GetDirect3DManager()->SetTargetWindow(m_windowNameArrya[nameArrayNumber]);
		if (scene)
		{
			scene->Render();
		}
	}
}

//
void MainLoopScene::UIRender(){
	DirectXEntity entity;
	int nameArrayNumber = 0;
	for (auto scene : m_viewWindow)
	{
		entity.GetDirect3DManager()->SetTargetWindow(m_windowNameArrya[nameArrayNumber]);
		if (scene)
		{
			scene->UIRender();
		}
	}
}


