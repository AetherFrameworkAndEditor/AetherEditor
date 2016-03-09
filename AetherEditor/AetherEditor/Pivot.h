#ifndef _PIVOT_H
#define _PIVOT_H
#include <vector>

#include <Cylinder.h>
#include <ViewCamera.h>
#include <Color.h>
#include <Vector3.h>
class Pivot
{
private:
	struct PivotProperty
	{
		aetherClass::Color _color;
		aetherClass::Vector3 _direction;
		aetherClass::Vector3 _position;
	};
public:
	Pivot();
	~Pivot();
	bool Initialize(aetherClass::ViewCamera*);
	void Render(aetherClass::ShaderBase*);
	void Finalize();
	void MoveDirection(aetherClass::Vector3);
private:
	std::vector<std::shared_ptr<aetherClass::ModelBase>> m_pivot;
	static PivotProperty m_pivotPropertyArray[];
	bool m_isInitialize;
};

#endif