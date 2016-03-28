#include "CameraObject.h"
#include "Pivot3D.h"
using namespace aetherClass;
CameraObject::CameraObject()
{
}


CameraObject::~CameraObject()
{
}

//
bool CameraObject::Initialize(aetherClass::ViewCamera* camera){

	//
	m_objectInfo._base = std::make_unique<Cube>();
	if (!m_objectInfo._base)
	{
		return false;
	}
	m_objectInfo._base->Initialize();
	m_objectInfo._base->property._color._red = 1.0f;
	m_objectInfo._base->property._transform._scale = 0.5f;
	m_objectInfo._base->SetCamera(camera);

	//
	m_objectInfo._lense = std::make_unique<Cylinder>();
	if (!m_objectInfo._lense)
	{
		return false;
	}
	m_objectInfo._lense->Initialize();
	m_objectInfo._lense->property._color._red = 1.0f;
	m_objectInfo._lense->property._transform._scale = 0.5f;
	m_objectInfo._lense->property._transform._rotation._x = 90;
	m_objectInfo._lense->SetCamera(camera);
	

	//
	m_objectInfo._collider = std::make_unique<Sphere>(10,10);
	if (!m_objectInfo._collider)
	{
		return false;
	}
	m_objectInfo._collider->Initialize();
	m_objectInfo._collider->SetCamera(camera);

	m_objectInfo._pivot = std::make_unique<Pivot3D>();
	if (!m_objectInfo._pivot)
	{
		return false;
	}
	m_objectInfo._pivot->Initialize(camera);

	m_objectInfo._isClick = false;
	return true;
}

//
void CameraObject::Render(aetherClass::ShaderBase* shader){
	m_objectInfo._base->Render(shader);
	m_objectInfo._lense->Render(shader);

	if (m_objectInfo._isClick)
	{
		m_objectInfo._pivot->Render(shader);
		m_objectInfo._collider->Render(shader);
		m_objectInfo._base->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
		m_objectInfo._base->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);

	}
	else
	{
		m_objectInfo._base->property._color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_objectInfo._base->property._color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	}

	return;
}

//
void CameraObject::Update(){
	auto translation = m_objectInfo._base->property._transform._translation;
	
	m_objectInfo._lense->property._transform._translation = translation+Vector3(0,0,1);
	m_objectInfo._collider->property._transform._translation = translation;
	m_objectInfo._pivot->MoveDirection(translation);
	return;
}

//
void CameraObject::SetTranslation(aetherClass::Vector3 translation){
	m_objectInfo._base->property._transform._translation = translation;
}

//
Vector3 CameraObject::GetTranslation(){
	return m_objectInfo._base->property._transform._translation;
}

//
Vector3 CameraObject::GetRotation(){
	return m_objectInfo._base->property._transform._rotation;
}
//
CameraObject::CameraObjectInfo& CameraObject::GetInfo(){
	return m_objectInfo;
}