#include "Pivot3D.h"
#include <iostream>

// PivotÇ…ÇµÇ©égÇÌÇ»Ç¢íËêî
namespace{
	const int kMaxCount = 3;
	const float kDirectionSize = 1.0f;
	const float kOtherSize = kDirectionSize / 10;
}
using namespace aetherClass;

// PivotÇÃè⁄ç◊èÓïÒ
Pivot3D::Pivot3DProperty Pivot3D::m_pivotPropertyArray[kMaxCount] = {
	{ Color(1.0f, 0.0f, 0.0f, 1.0f), Vector3(kDirectionSize, kOtherSize, kOtherSize), Vector3(kDirectionSize, 0.0f, 0.0f), },
	{ Color(0.0f, 1.0f, 0.0f, 1.0f), Vector3(kOtherSize, kDirectionSize, kOtherSize), Vector3(0.0f, kDirectionSize - kOtherSize, 0.0f), },
	{ Color(0.0f, 0.0f, 1.0f, 1.0f), Vector3(kOtherSize, kOtherSize, kDirectionSize), Vector3(0.0f, 0.0f, kDirectionSize) }
};

Pivot3D::Pivot3D() :
m_isInitialize(false)
{
}


Pivot3D::~Pivot3D()
{
	Finalize();
}

//
bool Pivot3D::Initialize(ViewCamera* camera){
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
		index->property._color= m_pivotPropertyArray[arrayNumber]._color;

		arrayNumber += 1;
	}
	m_isInitialize = true;
	return true;
}

//
void Pivot3D::Finalize(){
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
void Pivot3D::Render(ShaderBase* shader){
	if (!m_isInitialize)return;

	for (auto index : m_pivot)
	{	
		index->Render(shader);	
	}
}

void Pivot3D::MoveDirection(Vector3 move){
	if (!m_isInitialize)return;

	for (auto index : m_pivot)
	{
		index->property._transform._translation += move;
	}
}

//
void Pivot3D::SetScale(float length){
	for (auto& index : m_pivot)
	{
		Vector3 nomalize = index->property._transform._scale.Normalize();
		index->property._transform._scale = nomalize*length;
		index->property._transform._translation = nomalize*length;
	}
}
