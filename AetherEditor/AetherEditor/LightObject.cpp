#include "LightObject.h"
#include "Pivot3D.h"
#include <Cube.h>
using namespace aetherClass;
LightObject::LightObject()
{
}


LightObject::~LightObject()
{
	Destroy();
}

//
bool LightObject::Create(){
	if (!m_lightObject)
	{
		bool result;
		m_lightObject = std::make_unique<LightObjectInfo>();

		// コライダーの作成
		m_lightObject->_lightCollider = std::make_unique<Sphere>(10, 10);
		result = m_lightObject->_lightCollider->Initialize();
		if (!result){
			return false;
		}
		//ライトオブジェクト
		m_lightModel = std::make_unique<LightModel>();
		result = m_lightModel->Initialize();
		if (!result) return false;

		m_lightObject->_lightCollider->property._transform._scale = 1;

		auto centerPoint = m_lightObject->_light.property._translation;
		m_lightObject->_lightCollider->property._color = Color(1.0f, 0.0f, 0.0f, 0.4f);
		m_lightObject->_lightCollider->property._transform._translation = centerPoint;

		m_lightObject->_pivot = std::make_unique<Pivot3D>();
		m_lightObject->_pivot->Initialize(nullptr);
		m_lightObject->_pivot->MoveDirection(centerPoint);

		// 初期化
		m_lightObject->_isClick = false;
	}
	return true;
}

//
void LightObject::Destroy(){
	if (m_lightObject)
	{
		m_lightObject.release();
		m_lightObject = nullptr;
	}
	if (m_lightModel){
		m_lightModel->Finalize();
		m_lightModel.release();
		m_lightModel = nullptr;
	}
	return;
}

//
void LightObject::Render(aetherClass::ShaderBase* shader){
	if (m_lightObject->_isClick){
		m_lightObject->_pivot->Render(shader);
	}
	m_lightModel->Render(shader);
	if (m_lightObject->_isClick)
	{
		m_lightObject->_lightCollider->Render(shader);
	}
}

//
void LightObject::Update(){
	auto centerPoint = m_lightObject->_light.property._translation;

	// コライダーと擬似ライトオブジェクト
	m_lightModel->_property._transform._translation = centerPoint;
	m_lightObject->_lightCollider->property._transform._translation = centerPoint;
	m_lightObject->_pivot->MoveDirection(centerPoint);

	m_lightModel->Update();
}

//
Sphere* LightObject::GetCollider(){
	return m_lightObject->_lightCollider.get();
}

//
void LightObject::ChangePivotState(){
	m_lightObject->_isClick = !m_lightObject->_isClick;
}

//
std::unique_ptr<LightObject::LightObjectInfo>& LightObject::GetInfo(){

	return m_lightObject;
}

//
void LightObject::SetCamera(ViewCamera* camera){
	m_lightModel->SetCamera(camera);
	m_lightObject->_pivot->SetCamera(camera);
	m_lightObject->_lightCollider->SetCamera(camera);
}