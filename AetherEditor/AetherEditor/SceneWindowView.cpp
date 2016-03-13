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

	return true;
}

//
void SceneWindowView::Finalize(){

	return;
}

//
bool SceneWindowView::Updater(){
	m_viewCamera.Controller();
	
	return true;
}

void SceneWindowView::Render(){
	m_viewCamera.Render();

	return;
}

void SceneWindowView::UIRender(){
	return;
}


ViewCamera& SceneWindowView::GetSceneWindowCamera(){
	return m_viewCamera;
}