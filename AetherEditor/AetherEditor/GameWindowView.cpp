#include "GameWindowView.h"
#include"WorldObjectManager.h"
#include"GameController.h"
using namespace aetherClass;

GameWindowView::GameWindowView():
GameScene("Game", GetManager())
{
}


GameWindowView::~GameWindowView()
{
}


bool GameWindowView::Initialize(){
	ShaderDesc desc;
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader/DiffuseLightPS.hlsl";
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader/DiffuseLightVS.hlsl";

	m_colorShader = std::make_unique<LightShader>();
	m_colorShader->Initialize(desc, eVertex | ePixel);
	m_IsPlay = false;

	m_gameCamera = std::make_shared<ViewCamera>();
	m_gameLight = std::make_shared<Light>();
	return true;
}


void GameWindowView::Finalize(){
	m_colorShader->Finalize();
	return;
}


void GameWindowView::Render(){
	if (!m_IsPlay)return;

	m_gameCamera->Render();
	for (auto itr : WorldObjectManager::GetPrimitive()){
		itr->SetCamera(m_gameCamera.get());
		itr->Render(m_colorShader.get());
	}
	for (auto itr : WorldObjectManager::GetFbxModel()){
		itr->SetCamera(m_gameCamera.get());
		itr->Render(m_colorShader.get());
	}


	return;
}

void GameWindowView::UIRender(){
	if (!m_IsPlay)return;
	for (auto itr : WorldObjectManager::GetSprite()){
		itr->Render(m_colorShader.get());
	}

	return;
}

bool GameWindowView::Updater(){
	if (m_IsPlay == false){
		if (GameController::GetKey().KeyDownTrigger(VK_F5)){
			m_IsPlay = true;
			DirectXEntity entity;
			HWND hWnd = entity.GetWindowHandle(L"Game");
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			ShowWindow(hWnd, SW_SHOWDEFAULT);

			SetWindowText(hWnd,L"Game (Play)");
			m_gameCamera->property._translation = WorldObjectManager::GetCameraValue()._position;
			m_gameCamera->property._rotation = WorldObjectManager::GetCameraValue()._rotation;
			m_gameLight->property._translation = WorldObjectManager::GetLightValue()._position;

			static_cast<LightShader*>(m_colorShader.get())->SetLight(m_gameLight.get());
		}
	}
	else{
		if (GameController::GetKey().KeyDownTrigger(VK_F5)){
			m_IsPlay = false;
			DirectXEntity entity;
			HWND hWnd = entity.GetWindowHandle(L"Game");
			SetWindowText(hWnd, L"Game");
			ShowWindow(hWnd, SW_HIDE);

			hWnd = entity.GetWindowHandle(L"Scene");
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);

		}
		m_gameCamera->Controller();

	}

	return true;
}