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
			Destory();
		}

		//
		void Destory(){
			if (_primitive)
			{
				_primitive->Finalize();
				_primitive.release();
				_primitive = nullptr;
			}

			//
			if (_primitiveCollider)
			{
				_primitiveCollider->Finalize();
				_primitiveCollider.release();
				_primitiveCollider = nullptr;
			}

			//
			if (_pivot)
			{
				_pivot.release();
				_pivot = nullptr;
			}
			_isClick = false;
			_materialID = 0;
			_textureID = 0;
		}
		std::unique_ptr<aetherClass::ModelBase> _primitive;
		std::unique_ptr<aetherClass::Sphere> _primitiveCollider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;
		int _materialID;
		int _textureID;
	};
public:
	PrimitiveObject();
	~PrimitiveObject();

	bool Create(aetherClass::ModelBase*,aetherClass::ViewCamera*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	aetherClass::Sphere* GetCollider();
	void ChangePivotState();
	std::unique_ptr<PrimitiveObjectInfo> GetInfo();
private:
	void Destroy();
private:
	std::unique_ptr<PrimitiveObjectInfo>& m_primitiveObject;
};

#endif