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
#include"ModelUtility.h"
#include"MathUtility.h"

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
	desc._pixel._srcFile = L"Shader/Transparent.hlsl";
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	m_colorShader = std::make_unique<PixelShader>();
	m_colorShader->Initialize(desc, eVertex | ePixel);
	
	m_viewCamera.property._translation._z = -10;
	m_gameCamera.property._translation._z = -10;

	m_IsPlay = false;
	m_controllCamera= false;

	m_objectTransform._translation = 0;
	m_objectTransform._rotation = 0;
	m_objectTransform._scale= 0;
	m_cursorShowFlg = 0;
	return true;
}

//
void SceneWindowView::Finalize(){
	m_colorShader->Finalize();
	return;
}

void SceneWindowView::TransformInitialize(Transform transform){
	transform._translation = 0;
	transform._rotation = 0;
	transform._scale = 0;
}

void SceneWindowView::UpdateCurrentObject(){
	CurrentSelectObject *obj = &WorldObjectManager::GetCurrentSelectObject();
	switch (obj->_objectType){
		case eObjectType::ePrimitive:
			WorldObjectManager::GetPrimitive()[obj->_number]->GetInfo()->_primitive->property._transform._translation += m_objectTransform._translation;
			WorldObjectManager::GetPrimitive()[obj->_number]->GetInfo()->_primitive->property._transform._rotation += m_objectTransform._rotation;
			WorldObjectManager::GetPrimitive()[obj->_number]->GetInfo()->_primitive->property._transform._scale += m_objectTransform._scale;

			WorldObjectManager::GetPrimitive()[obj->_number]->Update();
			break;
		case eObjectType::eFBX:
			WorldObjectManager::GetFbxModel()[obj->_number]->GetInfo()->_fbx->property._transform._translation += m_objectTransform._translation;
			WorldObjectManager::GetFbxModel()[obj->_number]->GetInfo()->_fbx->property._transform._rotation += m_objectTransform._rotation;
			WorldObjectManager::GetFbxModel()[obj->_number]->GetInfo()->_fbx->property._transform._scale += m_objectTransform._scale;

			WorldObjectManager::GetFbxModel()[obj->_number]->Update();
			break;
		case eObjectType::eSprite:
			WorldObjectManager::GetSprite()[obj->_number]->GetInfo()->_sprite->property._transform._translation += m_objectTransform._translation;
			WorldObjectManager::GetSprite()[obj->_number]->GetInfo()->_sprite->property._transform._rotation += m_objectTransform._rotation;
			WorldObjectManager::GetSprite()[obj->_number]->GetInfo()->_sprite->property._transform._scale += m_objectTransform._scale;

			WorldObjectManager::GetSprite()[obj->_number]->Update();
			break;
		case eObjectType::eCamera:
			//		WorldObjectManager::GetCamera()->Update();
		case eObjectType::eLight:
			//		WorldObjectManager::GetLight()->Update();
			break;
	}

}

//
bool SceneWindowView::Updater(){
	bool result;
	if (!m_IsPlay){
		result = NotPlayingProcess();
	}
	else{
		result = PlayingProcess();
	}
	if (!result) return false;
	return true;
}

void SceneWindowView::LockMouseCursor(HWND hWnd){
	RECT screen;
	GetWindowRect(hWnd, &screen);
	POINT screenCenter = { (screen.left + screen.right) / 2, (screen.top + screen.bottom) / 2 };
	POINT mousePos(screenCenter);

	SetCursorPos(screenCenter.x, screenCenter.y);
	
	if (m_cursorShowFlg >= 0){
		m_cursorShowFlg = ShowCursor(false);
	}

	return;
}

void SceneWindowView::UpdateCamera(){
	if (GameController::GetMouse().IsRightButtonDown()){
		Vector2 vec = GameController::GetMouse().GetMouseMovement();
		LockMouseCursor(m_directX.GetWindowHandle(L"Scene"));
		if (!vec._x && !vec._y)return;

		vec /= kCameraRoteVal;
		m_viewCamera.property._rotation._x += vec._y;
		m_viewCamera.property._rotation._y += vec._x;

		m_controllCamera = true;
		return;
	}else if (GameController::GetMouse().IsWheelDown()){
		Vector2 vec = GameController::GetMouse().GetMouseMovement();
		LockMouseCursor(m_directX.GetWindowHandle(L"Scene"));
		if (!vec._x && !vec._y)return;

		vec /= kCameraTransVal;
	

		Vector3 translation(vec._x, vec._y, 0);
		Matrix4x4 roteMat;
		Vector3 rote = m_viewCamera.property._rotation;
		rote *= kAetherRadian;

		roteMat.PitchYawRoll(rote);
		translation = translation.TransformCoordNormal(roteMat);

		m_viewCamera.property._translation._x += translation._x;
		m_viewCamera.property._translation._y += translation._y;
		m_viewCamera.property._translation._z += translation._z;

		m_controllCamera = true;
		return;
	}
	else if (GameController::GetMouse().GetWheelMovement()){
		float vec = GameController::GetMouse().GetWheelMovement();
		vec /= kCameraZoomVal;

		Vector3 translation(0, 0, vec);
		Matrix4x4 roteMat;
		Vector3 rote = m_viewCamera.property._rotation;
		rote *= kAetherRadian;

		roteMat.PitchYawRoll(rote);
		translation = translation.TransformCoordNormal(roteMat);

		m_viewCamera.property._translation._x += translation._x;
		m_viewCamera.property._translation._y += translation._y;
		m_viewCamera.property._translation._z += translation._z;
		m_controllCamera = true;
		return;
	}


}


bool SceneWindowView::NotPlayingProcess(){

	static CameraValue value;
	m_controllCamera = false;

	if (m_cursorShowFlg < 0){
		m_cursorShowFlg = ShowCursor(true);
	}
	if (GameController::GetKey().IsKeyDown(DIK_F5)){
		m_IsPlay = true;
	}

	//m_objectTransform._translation._x = 0.1;
	
	UpdateCamera();
	if (!m_controllCamera){
		/*if (GameController::GetMouse().IsLeftButtonDown()){
			Vector2 mouse = GameController::GetMouse().GetMousePosition();
		}*/
		//オブジェクトセレクト
		if (GameController::GetMouse().IsLeftButtonTrigger()){
			RayVector ray = GameController::GetMouse().Intersection(m_viewCamera);
			ray._scaler = 100;
			CurrentSelectObject currentSelected = SelectObject(ray);
			WorldObjectManager::SetCurrentSelectObject(currentSelected);
		}
	}

	//移動対象オブジェクトアップデート(位置更新とか)
	UpdateCurrentObject();
	
	//カメラ更新
	value._position = m_gameCamera.property._translation;
	value._rotation = m_gameCamera.property._rotation;
	WorldObjectManager::RegisterCameraValue(value);
	
//	m_viewCamera.Controller();
	return true;
}

CurrentSelectObject SceneWindowView::SelectObject(RayVector ray){
	int index = 0;
	CurrentSelectObject currentSelected;

	currentSelected._objectType = eObjectType::ePrimitive;
	for (auto itr : WorldObjectManager::GetPrimitive()){
		if (aetherFunction::RaySphereIntersect(*itr->GetCollider(), ray)){
			itr->ChangePivotState();
			currentSelected._number = index;
			return currentSelected;
		}
		index++;
	}
	
	index = 0;
	currentSelected._objectType = eObjectType::eFBX;
	for (auto itr : WorldObjectManager::GetFbxModel()){
		itr->Update();
		if (aetherFunction::RaySphereIntersect(*itr->GetCollider(), ray)){
			itr->ChangePivotState();
			currentSelected._number = index;
			return currentSelected;
		}
		index++;
	}
	
	currentSelected._objectType = eObjectType::eSprite;
	index = 0;
	for (auto itr : WorldObjectManager::GetSprite()){
		//currentSelected._number = index;
		//WorldObjectManager::SetCurrentSelectObject(currentSelected);
		//return;
	}
//	index++;
	currentSelected._objectType = eObjectType::eNull;
	index = 0;
	return currentSelected;

}

bool SceneWindowView::PlayingProcess(){
	if (GameController::GetKey().IsKeyDown(DIK_F5)){
		m_IsPlay = false;
	}
	return true;
}


void SceneWindowView::Render(){
	if (m_IsPlay)return;
	m_viewCamera.Render();
	
	for (auto itr : WorldObjectManager::GetPrimitive()){
		itr->SetCamera(&m_viewCamera);
		itr->Render(m_colorShader.get());
	}
	for (auto itr : WorldObjectManager::GetFbxModel()){
		itr->SetCamera(&m_viewCamera);
		itr->Render(m_colorShader.get());
	}


	return;
}

void SceneWindowView::UIRender(){
	if (m_IsPlay)return;
	for (auto itr : WorldObjectManager::GetSprite()){
		itr->Render(m_colorShader.get());
	}

	return;
}


ViewCamera& SceneWindowView::GetSceneWindowCamera(){
	return m_viewCamera;
}