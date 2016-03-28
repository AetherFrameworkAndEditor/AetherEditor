#ifndef _CAMERAOBJECT_H
#define _CAMERAOBJECT_H
#include <memory>
#include <Cube.h>
#include <Cylinder.h>
#include <Sphere.h>
#include "Pivot.h"
class CameraObject
{
private:
	struct CameraObjectInfo{
		CameraObjectInfo(){
			Destory();
		}

		~CameraObjectInfo(){
			Destory();
		}

		std::unique_ptr<aetherClass::Cube> _base;
		std::unique_ptr<aetherClass::Cylinder> _lense;
		std::unique_ptr<aetherClass::Sphere> _collider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;

	private:
		//
		void Destory(){
			if (_base)
			{
				_base->Finalize();
				_base.release();
				_base = nullptr;
			}

			//
			if (_lense)
			{
				_lense->Finalize();
				_lense.release();
				_lense = nullptr;
			}

			if (_collider)
			{
				_collider->Finalize();
				_collider.release();
				_collider = nullptr;
			}


			//
			if (_pivot)
			{
				_pivot.release();
				_pivot = nullptr;
			}
			_isClick = false;

		}

	};
public:
	CameraObject();
	~CameraObject();
	bool Initialize(aetherClass::ViewCamera*);
	void Update();
	void Render(aetherClass::ShaderBase*);
	CameraObjectInfo& GetInfo();
private:

	CameraObjectInfo m_objectInfo;

};

#endif