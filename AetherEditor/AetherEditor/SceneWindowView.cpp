#include "SceneWindowView.h"
#include <PixelShader.h>
#include <Physics.h>
#include <Cube.h>
#include <GameController.h>
#include <GameClock.h>
#include "Pivot3D.h"
#include "Pivot2D.h"
#include "SpriteObject.h"
#include "Rectangle2D.h"
#include <iostream>
#include"WorldObjectManager.h"
using namespace aetherClass;
using namespace aetherFunction;
ViewCamera SceneWindowView::m_viewCamera;

SceneWindowView::SceneWindowView():
GameScene("Scene", GetManager())
{
}


SceneWindowView::~SceneWindowView()
{
}

//
bool SceneWindowView::Initialize(){
	ShaderDesc desc;
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader/BasicColor.hlsl";
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	m_colorShader = std::make_unique<PixelShader>();
	m_colorShader->Initialize(desc, eVertex | ePixel);
	m_viewCamera.property._translation._z = -10;
	return true;
}

//
void SceneWindowView::Finalize(){
	m_colorShader->Finalize();
	return;
}

//
bool SceneWindowView::Updater(){
	m_viewCamera.Controller();
	
	return true;
}

void SceneWindowView::Render(){
	m_viewCamera.Render();
	
	for (auto itr : WorldObjectManager::GetPrimitive()){
		itr->Render(m_colorShader.get());
	}
	for (auto itr : WorldObjectManager::GetFbxModel()){
		itr->Render(m_colorShader.get());
	}



	return;
}

void SceneWindowView::UIRender(){
	for (auto itr : WorldObjectManager::GetSprite()){
		itr->Render(m_colorShader.get());
	}

	return;
}


ViewCamera& SceneWindowView::GetSceneWindowCamera(){
	return m_viewCamera;
}