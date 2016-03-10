#include "PrimitiveObject.h"
#include "Pivot3D.h"
#include <Cube.h>
using namespace aetherClass;
PrimitiveObject::PrimitiveObject()
{
}


PrimitiveObject::~PrimitiveObject()
{
	Destroy();
}

bool PrimitiveObject::Create(ModelBase* model,ViewCamera* camera){
	if (!m_primitiveObject)
	{
		
		m_primitiveObject = std::make_unique<PrimitiveObjectInfo>();

		// �I�u�W�F�N�g�̍쐬
		m_primitiveObject->_primitive.reset(model);

		bool result = m_primitiveObject->_primitive->Initialize();
		if (!result)
		{
			return false;
		}
		m_primitiveObject->_primitive->property._color = Color(1.0f, 1.0f, 0.0f, 1.0f);
		m_primitiveObject->_primitive->SetCamera(camera);

		// �R���C�_�[�̍쐬
		m_primitiveObject->_primitiveCollider = std::make_unique<Sphere>(10, 10);
		m_primitiveObject->_primitiveCollider->property._transform._scale = 1;
		result = m_primitiveObject->_primitiveCollider->Initialize();
		if (!result)
		{
			return false;
		}
		auto centerPoint = m_primitiveObject->_primitive->property._transform._translation;
		m_primitiveObject->_primitiveCollider->SetCamera(camera);
		m_primitiveObject->_primitiveCollider->property._color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_primitiveObject->_primitiveCollider->property._transform._translation = centerPoint;

		// Pivot�̍쐬
		m_primitiveObject->_pivot = std::make_unique<Pivot3D>();
		m_primitiveObject->_pivot->Initialize(camera);
		m_primitiveObject->_pivot->MoveDirection(centerPoint);

		// ������
		m_primitiveObject->_isClick = false;
	}
}

//
void PrimitiveObject::Destroy(){
	if (m_primitiveObject)
	{
		m_primitiveObject->Destory();
		m_primitiveObject.release();
		m_primitiveObject = nullptr;
	}

}

//
void PrimitiveObject::Render(aetherClass::ShaderBase* shader){
	
	if (m_primitiveObject->_isClick)
	{
		
		m_primitiveObject->_pivot->Render(shader);
		m_primitiveObject->_primitiveCollider->Render(shader);
		m_primitiveObject->_primitive->property._color._alpha = 0.3;
	}
	else
	{
		m_primitiveObject->_primitive->property._color._alpha =1.0;
	}
	
	m_primitiveObject->_primitive->Render(shader);
}

//
void PrimitiveObject::Update(){
	auto centerPoint = m_primitiveObject->_primitive->property._transform._translation;

	// �R���C�_�[�ƃs�|�b�g
	m_primitiveObject->_primitiveCollider->property._transform._translation = centerPoint;
	m_primitiveObject->_pivot->MoveDirection(centerPoint);
}

//
Sphere* PrimitiveObject::GetCollider(){
	return m_primitiveObject->_primitiveCollider.get();
}

//
void PrimitiveObject::ChangePivotState(){
	m_primitiveObject->_isClick = !m_primitiveObject->_isClick;
}