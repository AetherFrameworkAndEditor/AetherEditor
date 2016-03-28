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
#include<algorithm>

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

//初期化
bool SceneWindowView::Initialize(){
	
	ShaderDesc desc;
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader/Transparent.hlsl";
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	m_colorShader = std::make_unique<PixelShader>();
	m_colorShader->Initialize(desc, eVertex | ePixel);
	
	m_viewCamera.property._translation._z = -20;
	m_gameCamera.Initialize(&m_viewCamera);
	m_gameCamera.SetTranslation(Vector3(0, 0, -10));

	m_IsPlay = false;
	m_controllCamera= false;

	m_objectTransform._translation = 0;
	m_objectTransform._rotation = 0;
	m_objectTransform._scale= 0;
	m_cursorShowFlg = 0;

	return true;
}

//終了
void SceneWindowView::Finalize(){
	m_colorShader->Finalize();
	return;
}
//トランスフォーム初期化
void SceneWindowView::TransformInitialize(Transform &transform){
	CurrentSelectObject *obj = &WorldObjectManager::GetCurrentSelectObject();
	if (!obj)return;
	switch (obj->_objectType){
	case eObjectType::ePrimitive:
		transform = WorldObjectManager::GetPrimitive().at(obj->_number)->GetInfo()->_primitive->property._transform;
		break;
	case eObjectType::eFBX:
		transform = WorldObjectManager::GetFbxModel().at(obj->_number)->GetInfo()->_fbx->property._transform;
		break;
	case eObjectType::eSprite:
		transform = WorldObjectManager::GetSprite().at(obj->_number)->GetInfo()->_sprite->property._transform;
		break;
	case eObjectType::eCamera:
		//		WorldObjectManager::GetCamera()->Update();
	case eObjectType::eLight:
		//		WorldObjectManager::GetLight()->Update();
		break;
	default:
		SecureZeroMemory(&transform, sizeof(Transform));
		transform._scale = 1;
		return;
	}
}

//更新振り分け
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
//プレイ中の処理
bool SceneWindowView::PlayingProcess(){
	if (GameController::GetKey().IsKeyDown(DIK_F5)){
		m_IsPlay = false;
	}
	return true;
}

//プレイ中じゃない時の処理
bool SceneWindowView::NotPlayingProcess(){
	TransformInitialize(m_objectTransform);
	static CameraValue value;
	m_controllCamera = false;
	m_viewCamera.Render();

	if (m_cursorShowFlg < 0){
		m_cursorShowFlg = ShowCursor(true);
	}
	if (GameController::GetKey().IsKeyDown(DIK_F5)){
		m_IsPlay = true;
		return true;
	}

	//シーンカメラ更新
	UpdateCamera();
	UpdateViewObject();

	if (!m_controllCamera){
		//マウス移動
		if (GameController::GetMouse().IsLeftButtonDown()){
			DragCurrentObject();
		}
		//オブジェクトセレクト
		if (GameController::GetMouse().IsLeftButtonTrigger()){
			RayVector ray = GameController::GetMouse().Intersection(m_viewCamera);
			ray._scaler = 100;
			CurrentSelectObject currentSelected = SelectObject(ray);
			WorldObjectManager::SetCurrentSelectObject(currentSelected);
			if (currentSelected._objectType != eObjectType::eNull){
				return true;
			}
		}
	}

	//移動対象オブジェクトアップデート(位置更新とか)
	UpdateCurrentObject();
	m_gameCamera.Update();
	//ゲームカメラ更新
	value._position = m_gameCamera.GetTranslation();
	value._rotation = m_gameCamera.GetRotation();
	WorldObjectManager::RegisterCameraValue(value);
	
//	m_viewCamera.Controller();
	return true;
}

//最前面選択するためにシーン上のオブジェクトを配列へ登録
void SceneWindowView::UpdateViewObject(){
	m_sceneObjectList.clear();
	int index = 0;

	Matrix4x4 viewMatrix = m_viewCamera.GetViewMatrix();
	SceneObject appendObj;
	
	//プリミティブ
	appendObj.type = eObjectType::ePrimitive;
	for (auto itr : WorldObjectManager::GetPrimitive()){
		Transform transform = itr->GetInfo()->_primitive->property._transform;
		Vector3 pos = transform._translation.TransformCoord(viewMatrix);
		
		//カメラの裏側にあるやつ	ｚ-
		if (pos._z < 0)continue;
		appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	index = 0;
	appendObj.type = eObjectType::eFBX;
	for (auto itr : WorldObjectManager::GetFbxModel()){
		Transform transform = itr->GetInfo()->_fbx->property._transform;
		Vector3 pos = transform._translation.TransformCoord(viewMatrix);

		//カメラの裏側にあるやつ	ｚ-
		if (pos._z < 0)continue;
		appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	index = 0;
	appendObj.type = eObjectType::eSprite;
	for (auto itr : WorldObjectManager::GetSprite()){
		Transform transform = itr->GetInfo()->_sprite->property._transform;

		appendObj.distance = transform._translation._z;
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	appendObj.type = eObjectType::eCamera;
	appendObj.distance = 0.0f;
	m_sceneObjectList.push_back(appendObj);

	if (!m_sceneObjectList.size() > 0)return;
	std::sort(m_sceneObjectList.begin(), m_sceneObjectList.end());
	auto itr = m_sceneObjectList.begin();

}

//オブジェクトの選択
CurrentSelectObject SceneWindowView::SelectObject(RayVector ray){

	int index = 0;
	bool result = false;
	CurrentSelectObject currentSelected;

	for (auto itr : m_sceneObjectList){
		switch (itr.type)
		{
		case eObjectType::ePrimitive:
			result = aetherFunction::RaySphereIntersect(*WorldObjectManager::GetPrimitive().at(itr.index)->GetCollider(), ray);
			if (result){
				WorldObjectManager::GetPrimitive().at(itr.index)->GetInfo()->_isClick = true;
			}
			break;
		case eObjectType::eFBX:
			result = aetherFunction::RaySphereIntersect(*WorldObjectManager::GetFbxModel().at(itr.index)->GetCollider(), ray);
			if (result){
				WorldObjectManager::GetFbxModel().at(itr.index)->GetInfo()->_isClick = true;
			}
			break;
		case eObjectType::eSprite:
			result = HitSprite(WorldObjectManager::GetSprite().at(itr.index)->GetInfo()->_sprite.get());
			if (result){
				WorldObjectManager::GetSprite().at(itr.index)->GetInfo()->_isClick = true;
			}
			break;
		case eObjectType::eLight:
			//			result = aetherFunction::RaySphereIntersect(*WorldObjectManager::GetPrimitive().at(itr.index)->GetCollider(), ray);
			break;
		case eObjectType::eCamera:
				result = aetherFunction::RaySphereIntersect(*m_gameCamera.GetInfo()._collider.get(), ray);
				if (result){
					m_gameCamera.GetInfo()._isClick = true;
				}
			break;
		default:
			break;
		}
		if (result){
			currentSelected._number = itr.index;
			currentSelected._objectType = itr.type;
			return currentSelected;
		}
	}//forLoop

	currentSelected._objectType = eObjectType::eNull;
	index = 0;
	return currentSelected;
}

//選択中オブジェクトをマウスでドラッグする
void SceneWindowView::DragCurrentObject(){
	Transform objTransform;

	CurrentSelectObject *obj = &WorldObjectManager::GetCurrentSelectObject();
	if (!obj)return;
	switch (obj->_objectType){
	case eObjectType::ePrimitive:
		objTransform = WorldObjectManager::GetPrimitive().at(obj->_number)->GetInfo()->_primitive->property._transform;
		break;
	case eObjectType::eFBX:
		objTransform = WorldObjectManager::GetFbxModel().at(obj->_number)->GetInfo()->_fbx->property._transform;
		break;
	case eObjectType::eSprite:
		objTransform = WorldObjectManager::GetSprite().at(obj->_number)->GetInfo()->_sprite->property._transform;
		break;
	case eObjectType::eCamera:
		//		WorldObjectManager::GetCamera()->Update();
	case eObjectType::eLight:
		//		WorldObjectManager::GetLight()->Update();
		break;
	default:
		return;
	}
	

	Vector2 mousePos = GameController::GetMouse().GetMousePosition();
	Vector2 screen = aetherFunction::GetWindowSize(m_directX.GetWindowHandle(L"Scene"));
	mousePos._x = (mousePos._x / (screen._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screen._x;
	mousePos._y = (mousePos._y / (screen._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENUSIZE) - GetSystemMetrics(SM_CXDLGFRAME)))* screen._y;

	if (obj->_objectType != eObjectType::eSprite){
		Matrix4x4 roteMat, viewMat;
		Vector3 rote = m_viewCamera.property._rotation;
		Vector3 cameraTrans = m_viewCamera.property._translation;

		rote *= kAetherRadian;
		roteMat.PitchYawRoll(rote);
		viewMat = m_viewCamera.GetViewMatrix();


		float length = objTransform._translation.TransformCoord(viewMat)._z;

		//	printf("%.2f,%.2f,%.2f\n", vec1._x, vec1._y, vec1._z);

		mousePos._x = ((2.0f*mousePos._x) / screen._x) - 1.0f;
		mousePos._y = (((2.0f*mousePos._y) / screen._y) - 1.0f)*-1.0f;

		mousePos._x = (length / 2)* mousePos._x;
		mousePos._y = (length / 2)* mousePos._y;



		Vector3 translation(mousePos._x, mousePos._y, 0);
		translation = translation.TransformCoordNormal(roteMat);
		Vector3 cameraZoffset(0, 0, length);
		cameraZoffset = cameraZoffset.TransformCoordNormal(roteMat);
		cameraTrans += cameraZoffset;
		objTransform._translation = translation + cameraTrans;

		m_objectTransform = objTransform;
	}
	else{	//スプライトの時
		Vector3 translate = Vector3(mousePos._x, mousePos._y, objTransform._translation._z);
		objTransform._translation = translate;
		m_objectTransform = objTransform;
	}
}

//マウスカーソルのロック
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

//カメラの操作
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
	}
	else if (GameController::GetMouse().IsWheelDown()){
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

//選択中オブジェクトの更新
void SceneWindowView::UpdateCurrentObject(){
	CurrentSelectObject *obj = &WorldObjectManager::GetCurrentSelectObject();
	if (!obj)return;
	switch (obj->_objectType){
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[obj->_number]->GetInfo()->_primitive->property._transform = m_objectTransform;
		WorldObjectManager::GetPrimitive()[obj->_number]->Update();
		break;
	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel()[obj->_number]->GetInfo()->_fbx->property._transform = m_objectTransform;
		WorldObjectManager::GetFbxModel()[obj->_number]->Update();
		break;
	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[obj->_number]->GetInfo()->_sprite->property._transform = m_objectTransform;
		WorldObjectManager::GetSprite()[obj->_number]->Update();
		break;
	case eObjectType::eCamera:
		//		WorldObjectManager::GetCamera()->Update();
	case eObjectType::eLight:
		//		WorldObjectManager::GetLight()->Update();
		break;
	}

}




//以下描画
void SceneWindowView::Render(){
	if (m_IsPlay)return;
	auto inverseVec = m_sceneObjectList;
	std::reverse(inverseVec.begin(),inverseVec.end());
	m_gameCamera.Render(m_colorShader.get());

	for (auto itr : inverseVec){
		switch (itr.type)
		{
		case eObjectType::ePrimitive:
			WorldObjectManager::GetPrimitive().at(itr.index)->SetCamera(&m_viewCamera);
			WorldObjectManager::GetPrimitive().at(itr.index)->Render(m_colorShader.get());
			break;
		case eObjectType::eFBX:
			WorldObjectManager::GetFbxModel().at(itr.index)->SetCamera(&m_viewCamera);
			WorldObjectManager::GetFbxModel().at(itr.index)->Render(m_colorShader.get());
			break;
		case eObjectType::eLight:
			break;
		case eObjectType::eCamera:
			
			break;
		default:
			break;
		}
	}
	
	return;
}

void SceneWindowView::UIRender(){
	if (m_IsPlay)return;
	for (auto itr : WorldObjectManager::GetSprite()){
//		Color col = itr->GetInfo()->_sprite->property._color;
//		printf("%.2f,%.2f,%.2f,%.2f\n", col._red, col._green, col._blue, col._alpha);

		itr->Render(m_colorShader.get());
	}

	return;
}


ViewCamera& SceneWindowView::GetSceneWindowCamera(){
	return m_viewCamera;
}

bool SceneWindowView::HitSprite(SpriteBase* sprite){
	Vector3 position = sprite->property._transform._translation;
	Vector3 size = sprite->property._transform._scale;
	Vector2 mousePos = GameController::GetMouse().GetMousePosition();
	Vector2 screen = aetherFunction::GetWindowSize(m_directX.GetWindowHandle(L"Scene"));
	mousePos._x = (mousePos._x / (screen._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screen._x;
	mousePos._y = (mousePos._y / (screen._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENUSIZE) - GetSystemMetrics(SM_CXDLGFRAME)))* screen._y;

	if (mousePos._x > position._x && mousePos._y > position._y &&
		mousePos._x < position._x + size._x && mousePos._y < position._y + size._y){
		return true;
	}
	return false;
}