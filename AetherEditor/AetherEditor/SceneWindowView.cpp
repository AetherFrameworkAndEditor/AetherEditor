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

//������
bool SceneWindowView::Initialize(){
	
	ShaderDesc desc;
	desc._pixel._entryName = "ps_main";
	desc._pixel._srcFile = L"Shader/BasicColor.hlsl";
	desc._vertex._entryName = "vs_main";
	desc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";

	m_colorShader = std::make_unique<PixelShader>();
	m_colorShader->Initialize(desc, eVertex | ePixel);
	
	m_viewCamera.property._translation._z = -20;
	m_viewCamera.property._translation._y = 10;

	m_viewCamera.property._rotation._x = 30;

	m_controllCamera = false;
	m_clickFlg = false;

	m_objectTransform._translation = 0;
	m_objectTransform._rotation = 0;
	m_objectTransform._scale= 0;
	m_cursorShowFlg = 0;
	
	m_gamelight.Create();
	m_gamelight.GetInfo()->_light.property._translation._y = WorldObjectManager::GetLightValue()._position._y = 10;
	m_gamelight.Update();

	m_gameCamera.Initialize(&m_viewCamera);
	m_gameCamera.GetInfo()._base->property._transform._translation._z = WorldObjectManager::GetCameraValue()._position._z = -10;
	m_gameCamera.Update();

	return true;
}

//�I��
void SceneWindowView::Finalize(){
	m_colorShader->Finalize();

	return;
}
//�S�I�u�W�F�N�g�̃A�b�v�f�[�g
void SceneWindowView::InitialUpdate(){
	for (auto itr : WorldObjectManager::GetPrimitive()){
		itr->Update();
	}
	for (auto itr : WorldObjectManager::GetFbxModel()){
		itr->Update();
	}
	for (auto itr : WorldObjectManager::GetSprite()){
		itr->Update();
	}
}

//�g�����X�t�H�[��������
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
		transform._translation = WorldObjectManager::GetCameraValue()._position;
		transform._rotation = WorldObjectManager::GetCameraValue()._rotation;
		break;
	case eObjectType::eLight:
		transform._translation = WorldObjectManager::GetLightValue()._position;
		break;
	default:
		SecureZeroMemory(&transform, sizeof(Transform));
		transform._scale = 1;
		return;
	}
}

//�X�V�U�蕪��
bool SceneWindowView::Updater(){
	bool result;
	if (GameController::GetKey().KeyDownTrigger(VK_F5)){
		const bool play = !WorldObjectManager::IsPlay();
		WorldObjectManager::IsPlay(play);
	}

	if (!WorldObjectManager::IsPlay()){
		result = NotPlayingProcess();
	}
	else{
		result = PlayingProcess();
	}
	if (!result) return false;
	return true;
}
//�v���C���̏���
bool SceneWindowView::PlayingProcess(){
	
	return true;
}

//�v���C������Ȃ����̏���
bool SceneWindowView::NotPlayingProcess(){
	TransformInitialize(m_objectTransform);
	//���C�g�ƃJ�����̏����擾�X�V
	GetWorldObjectValue();

	//�S�I�u�W�F�N�g�̃A�b�v�f�[�g���Ă�
	InitialUpdate();

	m_controllCamera = false;
	m_viewCamera.Render();

	if (m_cursorShowFlg < 0){
		m_cursorShowFlg = ShowCursor(true);
	}


	//�V�[���J�����X�V
	UpdateCamera();
	UpdateViewObject();

	if (!m_controllCamera){
		//��ʊO����Ȃ����Ƃ𔻒�
		if (InnerWindowMousePos())return true;
		//�}�E�X�ړ�
		if (GameController::GetMouse().IsLeftButtonDown()){
			DragCurrentObject();
		}
		//�I�u�W�F�N�g�Z���N�g
		if (GameController::GetMouse().IsLeftButtonTrigger()){
			RayVector ray = GameController::GetMouse().Intersection(m_viewCamera);
			ray._scaler = 100;
			CurrentSelectObject currentSelected = SelectObject(ray);
			WorldObjectManager::SetCurrentSelectObject(currentSelected);
			if (currentSelected._objectType != eObjectType::eNull){
				m_clickFlg = false;
				return true;
			}
		}
	}

	//�ړ��Ώۂ̈ʒu�X�V
	UpdateCurrentObject();

	//���C�g�ƃJ�����̏����X�V�o�^
	RegistWorldObjectValue();

//	m_viewCamera.Controller();
	return true;
}

//�őO�ʑI�����邽�߂ɃV�[����̃I�u�W�F�N�g��z��֓o�^
void SceneWindowView::UpdateViewObject(){
	m_sceneObjectList.clear();
	int index = 0;

	Matrix4x4 viewMatrix = m_viewCamera.GetViewMatrix();
	SceneObject appendObj;
	
	//�v���~�e�B�u
	appendObj.type = eObjectType::ePrimitive;
	for (auto itr : WorldObjectManager::GetPrimitive()){
		Transform transform = itr->GetInfo()->_primitive->property._transform;
		Vector3 pos = transform._translation.TransformCoord(viewMatrix);

		appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	index = 0;
	appendObj.type = eObjectType::eFBX;
	for (auto itr : WorldObjectManager::GetFbxModel()){
		Transform transform = itr->GetInfo()->_fbx->property._transform;
		Vector3 pos = transform._translation.TransformCoord(viewMatrix);

		appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	index = 0;
	appendObj.type = eObjectType::eLight;
	{
		Vector3 pos = m_gamelight.GetInfo()->_light.property._translation.TransformCoord(viewMatrix);

		//�J�����̗����ɂ�����	��-
		if (pos._z > 0){
			appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
			appendObj.index = index;

			m_sceneObjectList.push_back(appendObj);
		}
	}
	index = 0;
	appendObj.type = eObjectType::eCamera;
	{
		Vector3 pos = m_gameCamera.GetInfo()._base->property._transform._translation.TransformCoord(viewMatrix);

		//�J�����̗����ɂ�����	��-
		if (pos._z > 0){
			appendObj.distance = (pos._x * pos._x) + (pos._y * pos._y) + (pos._z * pos._z);
			appendObj.index = index;
			m_sceneObjectList.push_back(appendObj);
		}
	}


	index = 0;
	appendObj.type = eObjectType::eSprite;
	for (auto itr : WorldObjectManager::GetSprite()){
		Transform transform = itr->GetInfo()->_sprite->property._transform;

		appendObj.distance = transform._translation._z;
		appendObj.index = index++;

		m_sceneObjectList.push_back(appendObj);
	}

	if (!m_sceneObjectList.size() > 0)return;
	std::sort(m_sceneObjectList.begin(), m_sceneObjectList.end());
	auto itr = m_sceneObjectList.begin();

}

//�I�u�W�F�N�g�̑I��
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
			result = aetherFunction::RaySphereIntersect(*m_gamelight.GetCollider(), ray);
			if (result){
				WorldObjectManager::GetLightValue()._isClick = true;
			}
			break;
		case eObjectType::eCamera:
			result = aetherFunction::RaySphereIntersect(*m_gameCamera.GetInfo()._collider.get(), ray);
			if (result){
				WorldObjectManager::GetCameraValue()._isClick = true;
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

//�I�𒆃I�u�W�F�N�g���}�E�X�Ńh���b�O����
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
		objTransform._translation = WorldObjectManager::GetCameraValue()._position;
		objTransform._rotation = WorldObjectManager::GetCameraValue()._rotation;
		break;
	case eObjectType::eLight:
		objTransform._translation = WorldObjectManager::GetLightValue()._position;
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


		if (!m_clickFlg){
			Vector3 viewObjPos = objTransform._translation;
			viewObjPos = viewObjPos.TransformCoord(viewMat);
			viewObjPos = viewObjPos.TransformCoord(m_directX.GetDirect3DManager()->GetProjectionMatrix());
			m_prevMouseOffset = Vector2(viewObjPos._x, viewObjPos._y);
			m_prevMouseOffset = Vector2(mousePos._x - viewObjPos._x, mousePos._y - viewObjPos._y);
			printf("%f,%f\n", viewObjPos._x, viewObjPos._y);
			m_clickFlg = true;
		}

		mousePos = mousePos - m_prevMouseOffset;


		//��ʔ䗦�ɔ�Ⴗ��̂łȂ�Ƃ�������
		mousePos._x = (length / 1.8)* mousePos._x;
		mousePos._y = (length / 2.4)* mousePos._y;

		Vector3 translation(mousePos._x, mousePos._y, 0);
		translation = translation.TransformCoordNormal(roteMat);

		Vector3 cameraZoffset(0, 0, length);
		cameraZoffset = cameraZoffset.TransformCoordNormal(roteMat);
		cameraTrans += cameraZoffset;
		objTransform._translation = translation + cameraTrans;

		m_objectTransform = objTransform;
	}
	else{	//�X�v���C�g�̎�
		if (!m_clickFlg){
			Vector3 viewObjPos = objTransform._translation;

			m_prevMouseOffset = Vector2(mousePos._x - viewObjPos._x, mousePos._y - viewObjPos._y);
			//printf("%f,%f\n", viewObjPos._x, viewObjPos._y);
			m_clickFlg = true;
		}
		mousePos = mousePos - m_prevMouseOffset;


		Vector3 translate = Vector3(mousePos._x, mousePos._y, objTransform._translation._z);
		objTransform._translation = translate;
		m_objectTransform = objTransform;
	}
}

//�}�E�X�J�[�\���̃��b�N
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

//�J�����̑���
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


		Vector3 translation(vec._x, -vec._y, 0);
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

//�I�𒆃I�u�W�F�N�g�̍X�V
void SceneWindowView::UpdateCurrentObject(){
	CurrentSelectObject *obj = &WorldObjectManager::GetCurrentSelectObject();
	if (!obj)return;
	switch (obj->_objectType){
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive().at(obj->_number)->GetInfo()->_primitive->property._transform = m_objectTransform;
		WorldObjectManager::GetPrimitive().at(obj->_number)->Update();
		break;
	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel().at(obj->_number)->GetInfo()->_fbx->property._transform = m_objectTransform;
		WorldObjectManager::GetFbxModel().at(obj->_number)->Update();
		break;
	case eObjectType::eSprite:
		WorldObjectManager::GetSprite().at(obj->_number)->GetInfo()->_sprite->property._transform = m_objectTransform;
		WorldObjectManager::GetSprite().at(obj->_number)->Update();
		break;
	case eObjectType::eCamera:
		WorldObjectManager::GetCameraValue()._position = m_objectTransform._translation;
		WorldObjectManager::GetCameraValue()._rotation = m_objectTransform._rotation;
		break;
	case eObjectType::eLight:
		WorldObjectManager::GetLightValue()._position = m_objectTransform._translation;
		break;
	}

}




//�ȉ��`��
void SceneWindowView::Render(){
	if (WorldObjectManager::IsPlay())return;
	auto inverseVec = m_sceneObjectList;
	std::reverse(inverseVec.begin(),inverseVec.end());
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
			m_gamelight.SetCamera(&m_viewCamera);
			m_gamelight.Render(m_colorShader.get());
			break;
		case eObjectType::eCamera:
			m_gameCamera.Render(m_colorShader.get());
			break;
		default:
			break;
		}
	}
	return;
}

void SceneWindowView::UIRender(){
	if (WorldObjectManager::IsPlay())return;
	auto inverseVec = m_sceneObjectList;
	std::reverse(inverseVec.begin(), inverseVec.end());
	for (auto itr : inverseVec){
		switch (itr.type)
		{
		case eObjectType::eSprite:
			WorldObjectManager::GetSprite().at(itr.index)->Render(m_colorShader.get());
			break;
		default:
			break;
		}
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

//�f�[�^�̎擾�@�X�V
void SceneWindowView::GetWorldObjectValue(){
	auto cameraValue = WorldObjectManager::GetCameraValue();
	auto lightValue = WorldObjectManager::GetLightValue();

	m_gameCamera.SetTranslation(cameraValue._position);
	m_gameCamera.SetRotation(cameraValue._rotation);
	m_gameCamera.GetInfo()._isClick = cameraValue._isClick;

	m_gamelight.GetInfo()->_light.property._translation = lightValue._position;
	m_gamelight.GetInfo()->_isClick = lightValue._isClick;

	m_gameCamera.Update();
	m_gamelight.Update();
}

//�J�������̓o�^
void SceneWindowView::RegistWorldObjectValue(){
	//CameraValue cameraValue;
	//LightValue lightValue;

	//cameraValue._position = m_gameCamera.GetTranslation();
	//cameraValue._rotation = m_gameCamera.GetRotation();
	//cameraValue._isClick = WorldObjectManager::GetCameraValue()._isClick;

	//lightValue._position = m_gamelight.GetInfo()->_light.property._translation;
	//lightValue._isClick = WorldObjectManager::GetLightValue()._isClick;

	//WorldObjectManager::RegisterLightValue(lightValue);
	//WorldObjectManager::RegisterCameraValue(cameraValue);
}

bool SceneWindowView::InnerWindowMousePos(){
	//��ʊO���ǂ���
	Vector2 mousePos = GameController::GetMouse().GetMousePosition();
	Vector2 screen = aetherFunction::GetWindowSize(m_directX.GetWindowHandle(L"Scene"));
	mousePos._x = (mousePos._x / (screen._x - GetSystemMetrics(SM_CXDLGFRAME) * 2))* screen._x;
	mousePos._y = (mousePos._y / (screen._y - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENUSIZE) - GetSystemMetrics(SM_CXDLGFRAME)))* screen._y;
	if (mousePos._x < 0 || mousePos._y < 0 || mousePos._x > screen._x || mousePos._y > screen._y)return true;
	return false;
}