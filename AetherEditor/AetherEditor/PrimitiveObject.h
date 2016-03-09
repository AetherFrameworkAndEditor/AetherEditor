#ifndef _PROMITOVEOBJECT_H
#define _PROMITOVEOBJECT_H
#include <Sphere.h>
#include <ModelBase.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include <memory>
#include "Pivot.h"
class PrimitiveObject
{
private:
	struct PrimitiveObjectInfo{
		PrimitiveObjectInfo(){
			_primitive = nullptr;
			_primitiveCollider = nullptr;
			_pivot = nullptr;
			_isClick = false;
		}

		~PrimitiveObjectInfo(){
			_primitive = nullptr;
			_primitiveCollider = nullptr;
			_pivot = nullptr;
			_isClick = false;
		}

		std::unique_ptr<aetherClass::ModelBase> _primitive;
		std::unique_ptr<aetherClass::Sphere> _primitiveCollider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;
	};
public:
	PrimitiveObject();
	~PrimitiveObject();

	bool Create(aetherClass::ViewCamera*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	aetherClass::Sphere* GetCollider();
	void ChangePivotState();

private:
	void Destroy();
private:
	std::unique_ptr<PrimitiveObjectInfo> m_primitiveObject;
};

#endif