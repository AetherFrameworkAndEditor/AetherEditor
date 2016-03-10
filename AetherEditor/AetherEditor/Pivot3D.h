#ifndef _PIVOT3D_H
#define _PIVOT3D_H
#include <vector>

#include <Cylinder.h>
#include <ViewCamera.h>
#include <Color.h>
#include <Vector3.h>

#include "Pivot.h"
class Pivot3D:
	public Pivot
{
private:
	struct Pivot3DProperty
	{
		aetherClass::Color _color;
		aetherClass::Vector3 _directionScale;
		aetherClass::Vector3 _position;
	};

public:
	Pivot3D();
	~Pivot3D();
	bool Initialize(aetherClass::ViewCamera*)override;
	void Render(aetherClass::ShaderBase*)override;
	void MoveDirection(aetherClass::Vector3)override;
	void SetLength(float)override;

private:
	void Finalize()override;

private:
	std::vector<std::shared_ptr<aetherClass::ModelBase>> m_pivot;
	static Pivot3DProperty m_pivotPropertyArray[];
	bool m_isInitialize;
};

#endif