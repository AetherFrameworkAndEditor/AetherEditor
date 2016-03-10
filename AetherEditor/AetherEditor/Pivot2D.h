#ifndef _PIVOT2D_H
#define _PIVOT2D_H
#include <memory>
#include <ModelBase.h>
#include <Vector2.h>
#include <Color.h>
#include "Pivot.h"
class Pivot2D :
	public Pivot
{
private:
	struct Pivot2DProperty
	{
		aetherClass::Color _color;
		aetherClass::Vector3 _directionScale;
		aetherClass::Vector3 _position;
	};
public:
	Pivot2D();
	~Pivot2D();
	bool Initialize(aetherClass::ViewCamera*)override;
	void Render(aetherClass::ShaderBase*)override;
	void MoveDirection(aetherClass::Vector3)override;

private:
	void Finalize()override;
private:
	std::vector<std::shared_ptr<aetherClass::ModelBase>> m_pivot;
	static Pivot2DProperty m_pivotPropertyArray[];
	bool m_isInitialize;
};

#endif