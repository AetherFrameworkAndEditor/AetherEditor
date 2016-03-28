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

bool SpriteObject::Create(SpriteBase* sprite){
	if (!m_spriteObject)
	{
		m_spriteObject = std::make_unique<SpriteObjectInfo>();

		// オブジェクトの作成
		m_spriteObject->_sprite.reset(sprite);
		bool result = m_spriteObject->_sprite->Initialize();
		if (!result)
		{
			return false;
		}
		m_spriteObject->_sprite->property._transform._scale = Vector3(50.0f, 50.0f,0.0f);
		m_spriteObject->_sprite->property._color = Color(1.0f, 1.0f, 0.0f, 1.0f);
		
		auto center = m_spriteObject->_sprite->property._transform._scale;
		auto position = m_spriteObject->_sprite->property._transform._translation;
		auto centerPoint = (position + center)/2;

		// Pivotの作成
		m_spriteObject->_pivot = std::make_unique<Pivot2D>();
		m_spriteObject->_pivot->Initialize(nullptr);
		m_spriteObject->_pivot->SetScale(100);

		m_spriteObject->_isClick = false;
	}
}

//
void SpriteObject::Destroy(){
	if (m_spriteObject)
	{
		m_spriteObject.release();
		m_spriteObject = nullptr;
	}

}

//
void SpriteObject::Render(aetherClass::ShaderBase* shader){

	m_spriteObject->_sprite->Render(shader);
	
	if (m_spriteObject->_isClick)
	{
		m_spriteObject->_pivot->Render(shader);
	}
	return;
}

//
void SpriteObject::Update(){

	auto scale = m_spriteObject->_sprite->property._transform._scale;
	auto position = m_spriteObject->_sprite->property._transform._translation;
	auto center = (position + (scale / 2));

	m_spriteObject->_pivot->MoveDirection(center);
}

//
void SpriteObject::ChangePivotState(){
	m_spriteObject->_isClick = !m_spriteObject->_isClick;
}


std::unique_ptr<SpriteObject::SpriteObjectInfo>& SpriteObject::GetInfo(){
	return m_spriteObject;
}