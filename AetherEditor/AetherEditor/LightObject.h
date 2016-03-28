#ifndef _LIGHTOBJECT_H
#define _LIGHTOBJECT_H
#include <Light.h>
#include<Sphere.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include <memory>
#include "Pivot.h"
#include"LightModel.h"
class LightObject
{
private:
	struct LightObjectInfo{
		LightObjectInfo(){
			_lightCollider = nullptr;

			_isClick = false;
			_name = "null_name";
		}

		~LightObjectInfo(){
			Destory();
		}

		aetherClass::Light _light;
		std::unique_ptr<Pivot> _pivot;
		std::unique_ptr<aetherClass::Sphere> _lightCollider;
		bool _isClick;
		std::string _name;

	private:
		//
		void Destory(){
			_light.property._translation = 0;
			if (_lightCollider){
				_lightCollider->Finalize();
				_lightCollider.release();
				_lightCollider = nullptr;
			}
			if (_pivot)
			{
				_pivot.release();
				_pivot = nullptr;
			}
			_isClick = false;
		}
	};

public:
	LightObject();
	~LightObject();

	bool Create();
	void Update();
	void Render(aetherClass::ShaderBase*);

	void ChangePivotState();
	void SetCamera(aetherClass::ViewCamera*);

	aetherClass::Sphere* GetCollider();
	std::unique_ptr<LightObjectInfo>& GetInfo();

private:
	void Destroy();
private:
	std::unique_ptr<LightModel> m_lightModel;
	std::unique_ptr<LightObjectInfo> m_lightObject;
};

#endif