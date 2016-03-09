#include "Pivot.h"
#include <iostream>

// PivotÇ…ÇµÇ©égÇÌÇ»Ç¢íËêî
namespace{
	const int kMaxCount = 3;
	const float kDirectionSize = 5.0f;
	const float kOtherSize = kDirectionSize / 10;
}
using namespace aetherClass;

// PivotÇÃè⁄ç◊èÓïÒ
Pivot::PivotProperty Pivot::m_pivotPropertyArray[kMaxCount] = {
	{ Color(1.0f, 0.0f, 0.0f, 1.0f), Vector3(kDirectionSize, kOtherSize, kOtherSize), Vector3(kDirectionSize, 0.0f, 0.0f), },
	{ Color(0.0f, 1.0f, 0.0f, 1.0f), Vector3(kOtherSize, kDirectionSize, kOtherSize), Vector3(0.0f, kDirectionSize - kOtherSize, 0.0f), },
	{ Color(0.0f, 0.0f, 1.0f, 1.0f), Vector3(kOtherSize, kOtherSize, kDirectionSize), Vector3(0.0f, 0.0f, kDirectionSize) }
};

Pivot::Pivot()
{
}


Pivot::~Pivot()
{
}

//
bool Pivot::Initialize(ViewCamera* camera){
	Finalize();
	m_pivot.resize(kMaxCount);
	int arrayNumber = 0;
	for (auto& index : m_pivot)
	{
		if (index) continue;
		index = std::make_shared<Cylinder>();
		index->Initialize();
		index->SetCamera(camera);
		index->property._transform._scale = m_pivotPropertyArray[arrayNumber]._direction;
		index->property._transform._translation = m_pivotPropertyArray[arrayNumber]._position;
		index->property._color= m_pivotPropertyArray[arrayNumber]._color;

		arrayNumber += 1;
	}
	return true;
}

//
void Pivot::Finalize(){
	for (auto index : m_pivot)
	{
		if (index)
		{
			index->Finalize();
			index.reset();
		}
	}

	m_pivot.clear();
}

//
void Pivot::Render(ShaderBase* shader){
	for (auto index : m_pivot)
	{
		if (index)
		{	
			index->Render(shader);
		}
	}
}