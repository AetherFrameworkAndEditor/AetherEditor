#include "Pivot2D.h"
#include <iostream>
#include <Cylinder.h>

// PivotÇ…ÇµÇ©égÇÌÇ»Ç¢íËêî
namespace{
	const int kMaxCount = 2;
	const float kDirectionSize = 1.0f;
	const float kOtherSize = kDirectionSize / 10;
}
using namespace aetherClass;

// PivotÇÃè⁄ç◊èÓïÒ
Pivot2D::Pivot2DProperty Pivot2D::m_pivotPropertyArray[kMaxCount] = {
	{ Color(1.0f, 0.0f, 0.0f, 1.0f), Vector3(kDirectionSize, kOtherSize, 0.0f), Vector3(kDirectionSize, 0.0f, 0.0f), },
	{ Color(0.0f, 1.0f, 0.0f, 1.0f), Vector3(kOtherSize, kDirectionSize, 0.0f), Vector3(0.0f, kDirectionSize - kOtherSize, 0.0f), }
};

Pivot2D::Pivot2D() :
m_isInitialize(false)
{
}


Pivot2D::~Pivot2D()
{
	Finalize();
}

//
bool Pivot2D::Initialize(ViewCamera* camera){
	Finalize();

	m_pivot.resize(kMaxCount);
	int arrayNumber = 0;
	bool result;
	for (auto& index : m_pivot)
	{
		index = std::make_shared<Cylinder>();
		result = index->Initialize();
		if (!result)
		{
			return false;
		}
		index->SetCamera(camera);
		index->property._transform._scale = m_pivotPropertyArray[arrayNumber]._directionScale;
		index->property._transform._translation = m_pivotPropertyArray[arrayNumber]._position;
		index->property._color = m_pivotPropertyArray[arrayNumber]._color;

		arrayNumber += 1;
	}
	m_isInitialize = true;
	return true;
}

//
void Pivot2D::Finalize(){
	if (!m_isInitialize)return;

	for (auto index : m_pivot)
	{

		index->Finalize();
		index.reset();
	}

	m_pivot.clear();
	m_isInitialize = false;
	return;
}

//
void Pivot2D::Render(ShaderBase* shader){
	if (!m_isInitialize)return;

	for (auto index : m_pivot)
	{
		index->Render(shader);
	}

	return;
}

//
void Pivot2D::MoveDirection(Vector3 move){
	if (!m_isInitialize)return;
	/*	2DópÇÃà⁄ìÆèàóù à¯êîÇ≈ÇÕVector2ÇéÛÇØéÊÇÈÇ™Vector2Ç…ïœä∑Ç∑ÇÈ*/
	Vector2 move2d = Vector2(move._x, move._y);
	for (auto index : m_pivot)
	{
		index->property._transform._translation._x += move2d._x;
		index->property._transform._translation._y += move2d._y;
	}

	return;
}

//
void Pivot2D::SetScale(float length){
	
	for (auto& index : m_pivot)
	{
		Vector3 nomalize = index->property._transform._scale.Normalize();
		index->property._transform._scale = nomalize*length;
		index->property._transform._translation = nomalize*length;
	}
}
