#include "FbxModelObject.h"
#include "Pivot3D.h"
using namespace aetherClass;

FbxModelObject::FbxModelObject()
{
}


FbxModelObject::~FbxModelObject()
{
}

bool FbxModelObject::Create(std::string path, ViewCamera* camera){
	if (!m_fbxObject)
	{

		m_fbxObject = std::make_unique<FbxObjectInfo>();

		m_fbxObject->_fbx = std::make_unique<FbxModel>();
		// オブジェクトの作成
		m_fbxObject->_fbx->LoadFBX(path, eAxisSystem::eAxisOpenGL);
		m_fbxObject->_fbx->SetModelMaterialColor(Color(1.0f, 1.0f, 0.0f, 1.0f),eMatrerialType::eDiffuse);
		m_fbxObject->_fbx->SetCamera(camera);

		// コライダーの作成
		m_fbxObject->_fbxCollider = std::make_unique<Sphere>(10, 10);
		bool result = m_fbxObject->_fbxCollider->Initialize();
		if (!result)
		{
			return false;
		}
		m_fbxObject->_fbxCollider->property._transform._scale = 1;

		auto centerPoint = m_fbxObject->_fbx->property._transform._translation;
		m_fbxObject->_fbxCollider->SetCamera(camera);
		m_fbxObject->_fbxCollider->property._color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_fbxObject->_fbxCollider->property._transform._translation = centerPoint;

		// Pivotの作成
		m_fbxObject->_pivot = std::make_unique<Pivot3D>();
		m_fbxObject->_pivot->Initialize(camera);
		m_fbxObject->_pivot->MoveDirection(centerPoint);

		// 初期化
		m_fbxObject->_isClick = false;
	}
}

//
void FbxModelObject::Destroy(){
	if (m_fbxObject)
	{
		m_fbxObject->Destory();
		m_fbxObject.release();
		m_fbxObject = nullptr;
	}

}

//
void FbxModelObject::Render(aetherClass::ShaderBase* shader){

	if (m_fbxObject->_isClick)
	{

		m_fbxObject->_pivot->Render(shader);
		m_fbxObject->_fbxCollider->Render(shader);
		m_fbxObject->_fbx->SetModelMaterialColor(Color(1.0f, 1.0f, 0.0f, 0.3f), eMatrerialType::eDiffuse);
	}
	else
	{
		m_fbxObject->_fbx->SetModelMaterialColor(Color(1.0f, 1.0f, 0.0f, 1.0f), eMatrerialType::eDiffuse);
	}

	m_fbxObject->_fbx->Render(shader);
}

//
void FbxModelObject::Update(){
	auto centerPoint = m_fbxObject->_fbx->property._transform._translation;

	// コライダーとピポット
	m_fbxObject->_fbxCollider->property._transform._translation = centerPoint;
	m_fbxObject->_pivot->MoveDirection(centerPoint);
}

//
Sphere* FbxModelObject::GetCollider(){
	return m_fbxObject->_fbxCollider.get();
}

//
void FbxModelObject::ChangePivotState(){
	m_fbxObject->_isClick = !m_fbxObject->_isClick;
}
