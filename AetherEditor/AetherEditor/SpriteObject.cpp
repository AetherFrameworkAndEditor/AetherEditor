#include "SpriteObject.h"
#include "Pivot2D.h"
#include <Cube.h>
using namespace aetherClass;
SpriteObject::SpriteObject()
{
}


SpriteObject::~SpriteObject()
{
	Destroy();
}

bool SpriteObject::Create(SpriteBase* sprite, ViewCamera* camera){
	if (!m_spriteObject)
	{
		m_spriteObject = std::make_unique<SpriteObjectInfo>();

		// �I�u�W�F�N�g�̍쐬
		m_spriteObject->_sprite.reset(sprite);
		bool result = m_spriteObject->_sprite->Initialize();
		if (!result)
		{
			return false;
		}
		m_spriteObject->_sprite->property._color = Color(1.0f, 1.0f, 0.0f, 1.0f);

		// �R���C�_�[�̍쐬
		m_spriteObject->_spriteCollider = std::make_unique<Sphere>(10, 10);
		m_spriteObject->_spriteCollider->property._transform._scale = 1;
		result = m_spriteObject->_spriteCollider->Initialize();
		if (!result)
		{
			return false;
		}
		auto center = m_spriteObject->_sprite->property._transform._scale / 2;
		auto position = m_spriteObject->_sprite->property._transform._translation;
		auto centerPoint = position + center;
		m_spriteObject->_spriteCollider->SetCamera(camera);
		m_spriteObject->_spriteCollider->property._color = Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_spriteObject->_spriteCollider->property._transform._translation = centerPoint;

		// Pivot�̍쐬
		m_spriteObject->_pivot = std::make_unique<Pivot2D>();
		m_spriteObject->_pivot->Initialize(camera);
		m_spriteObject->_pivot->MoveDirection(centerPoint);

		m_spriteObject->_isClick = false;
	}
}

//
void SpriteObject::Destroy(){
	if (m_spriteObject)
	{
		m_spriteObject->Destory();
		m_spriteObject.release();
		m_spriteObject = nullptr;
	}

}

//
void SpriteObject::Render(aetherClass::ShaderBase* shader){

	if (m_spriteObject->_isClick)
	{

		m_spriteObject->_pivot->Render(shader);
		m_spriteObject->_spriteCollider->Render(shader);
		m_spriteObject->_sprite->property._color._alpha = 0.3;
	}
	else
	{
		m_spriteObject->_sprite->property._color._alpha = 1.0;
	}

	m_spriteObject->_sprite->Render(shader);
}

//
void SpriteObject::Update(){

	auto center = m_spriteObject->_sprite->property._transform._scale / 2;
	auto position = m_spriteObject->_sprite->property._transform._translation;
	auto centerPoint = position + center;
	// �R���C�_�[�ƃs�|�b�g
	m_spriteObject->_spriteCollider->property._transform._translation = centerPoint;
	m_spriteObject->_pivot->MoveDirection(centerPoint);
}

//
Sphere* SpriteObject::GetCollider(){
	return m_spriteObject->_spriteCollider.get();
}

//
void SpriteObject::ChangePivotState(){
	m_spriteObject->_isClick = !m_spriteObject->_isClick;
}