#include "LightModel.h"

using namespace aetherClass;
LightModel::LightModel()
{
}


LightModel::~LightModel()
{
}

bool LightModel::Initialize(){
	bool result;
	m_sphere = std::make_unique<Sphere>(10,10);
	result = m_sphere->Initialize();
	if (!result)return false;

	m_cone = std::make_unique<Cone>(4);
	result = m_cone->Initialize();
	if (!result)return false;

	Color color(0.0f, 1.0f, 1.0f, 1.0f);

	m_sphere->property._color = color;
	m_cone->property._color = color;

	m_sphere->property._transform._scale = 0.5;
	m_cone->property._transform._scale = 0.7;
	m_sphere->property._transform._translation._y = m_spherePosition._y = m_cone->property._transform._scale._y;
	
	return true;
}
void LightModel::Finalize(){
	if (m_sphere){
		m_sphere->Finalize();
		m_sphere.release();
		m_sphere = nullptr;
	}
	if (m_cone){
		m_cone->Finalize();
		m_cone.release();
		m_cone = nullptr;
	}
}
void LightModel::Update(){
	m_sphere->property._transform._translation = _property._transform._translation + m_spherePosition;
	m_cone->property._transform._translation = _property._transform._translation + m_conePosition;
}
void LightModel::Render(aetherClass::ShaderBase *shader){
	m_sphere->Render(shader);
	m_cone->Render(shader);

}

void LightModel::SetCamera(aetherClass::ViewCamera *camera){
	m_sphere->SetCamera(camera);
	m_cone->SetCamera(camera);
}

