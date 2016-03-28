#ifndef _LIGHTMODEL_H
#define _LIGHTMODEL_H
#include"Sphere.h"
#include"Cone.h"
class LightModel
{
public:
	LightModel();
	~LightModel();
	bool Initialize();
	void Finalize();
	void Update();
	void Render(aetherClass::ShaderBase*);

	void SetCamera(aetherClass::ViewCamera*);
	aetherClass::Property _property;
private:
	std::unique_ptr<aetherClass::Sphere>m_sphere;
	std::unique_ptr<Cone>m_cone;
	aetherClass::Vector3 m_spherePosition, m_conePosition;

};

#endif