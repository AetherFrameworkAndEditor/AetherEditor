#include "Pivot2D.h"
#include <iostream>
#include <Rectangle2D.h>

// Pivotにしか使わない定数
namespace{
	const int kMaxCount = 2;
	const float kDirectionSize = 1.0f;
	const float kOtherSize = kDirectionSize / 10;
}
using namespace aetherClass;

// Pivotの詳細情報
Pivot2D::Pivot2DProperty Pivot2D::m_pivotPropertyArray[kMaxCount] = {
	{ Color(1.0f, 0.0f, 0.0f, 1.0f), Vector3(kDirectionSize, kOtherSize, 0.0f), Vector3(0, 0.0f, 0.0f), },
	{ Color(0.0f, 1.0f, 0.0f, 1.0f), Vector3(kOtherSize, -kDirectionSize, 0.0f), Vector3(0.0f, 0, 0.0f), }
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
		index = std::make_shared<Rectangle2D>();
		result = index->Initialize();
		if (!result)
		{
			return false;
		}
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
	/*	2D用の移動処理 引数ではVector2を受け取るがVector2に変換する*/
	Vector2 move2d = Vector2(move._x, move._y);
	int id = 0;
	for (auto index : m_pivot)
	{
		index->property._transform._translation._x = move2d._x + m_pivotPropertyArray[id]._position._x;
		index->property._transform._translation._y = move2d._y + m_pivotPropertyArray[id]._position._y;
		id += 1;
	}

	return;
}

//
void Pivot2D::SetScale(float length){
	
	for (auto& index : m_pivot)
	{
		Vector3 nomalize = index->property._transform._scale.Normalize();
		index->property._transform._scale = nomalize*length;
	}
}