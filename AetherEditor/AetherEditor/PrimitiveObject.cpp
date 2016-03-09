#include "PrimitiveObject.h"
#include <Cube.h>
using namespace aetherClass;
PrimitiveObject::PrimitiveObject()
{
}


PrimitiveObject::~PrimitiveObject()
{
	Destroy();
}

bool PrimitiveObject::Create(ViewCamera* camera){
	if (!m_primitiveObject)
	{
		m_primitiveObject = std::make_unique<PrimitiveObjectInfo>();
		m_primitiveObject->_primitive = std::make_unique<Cube>();

		bool result = m_primitiveObject->_primitive->Initialize();
		if (!result)
		{
			return false;
		}
		m_primitiveObject->_primitive->property._color = Color(1.0f, 1.0f, 0.0f, 1.0f);
		m_primitiveObject->_primitive->SetCamera(camera);

		
		m_primitiveObject->_primitiveCollider = std::make_unique<Sphere>(10, 10);
		m_primitiveObject->_primitiveCollider->property._transform._scale = 1;
		result = m_primitiveObject->_primitiveCollider->Initialize();
		if (!result)
		{
			return false;
		}
		m_primitiveObject->_primitiveCollider->SetCamera(camera);
		m_primitiveObject->_primitiveCollider->property._color = Color(1.0f, 0.0f, 0.0f, 0.3f);
		m_primitiveObject->_primitiveCollider->property._transform._translation = m_primitiveObject->_primitive->property._transform._translation;

		m_primitiveObject->_pivot = std::make_unique<Pivot>();
		m_primitiveObject->_pivot->Initialize(camera);
		m_primitiveObject->_pivot->MoveDirection(m_primitiveObject->_primitive->property._transform._translation);

		m_primitiveObject->_isClick = false;
	}
}

//
void PrimitiveObject::Destroy(){
	if (m_primitiveObject)
	{
		m_primitiveObject->_primitive->Finalize();
		m_primitiveObject->_primitiveCollider->Finalize();
		m_primitiveObject->_pivot->Finalize();
	}
	m_primitiveObject.release();
}

//
void PrimitiveObject::Render(aetherClass::ShaderBase* shader){
	
	if (m_primitiveObject->_isClick)
	{
		
		m_primitiveObject->_pivot->Render(shader);
		//m_primitiveObject->_primitiveCollider->Render(shader);
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

	// コライダーとピポット
	m_primitiveObject->_primitiveCollider->property._transform._translation = m_primitiveObject->_primitive->property._transform._translation;
	m_primitiveObject->_pivot->MoveDirection(m_primitiveObject->_primitive->property._transform._translation);
}

//
Sphere* PrimitiveObject::GetCollider(){
	return m_primitiveObject->_primitiveCollider.get();
}

//
void PrimitiveObject::ChangePivotState(){
	m_primitiveObject->_isClick = !m_primitiveObject->_isClick;
}
