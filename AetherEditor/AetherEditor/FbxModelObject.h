#ifndef _FBXMODELOBJECT_H
#define _FBXMODELOBJECT_H
#include <Sphere.h>
#include <FbxModel.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include <string>
#include <memory>
#include "Pivot.h"
class FbxModelObject
{
private:
	struct FbxObjectInfo{
		FbxObjectInfo(){
			_fbx = nullptr;
			_fbxCollider = nullptr;
			_pivot = nullptr;
			_isClick = false;
		}

		~FbxObjectInfo(){
			Destory();
		}

		//
		void Destory(){
			if (_fbx)
			{
				_fbx->Finalize();
				_fbx.release();
				_fbx = nullptr;
			}

			//
			if (_fbxCollider)
			{
				_fbxCollider->Finalize();
				_fbxCollider.release();
				_fbxCollider = nullptr;
			}

			//
			if (_pivot)
			{
				_pivot.release();
				_pivot = nullptr;
			}
			_isClick = false;
		}
		std::unique_ptr<aetherClass::FbxModel> _fbx;
		std::unique_ptr<aetherClass::Sphere> _fbxCollider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;
	};
public:
	FbxModelObject();
	~FbxModelObject();

	bool Create(std::string, aetherClass::ViewCamera*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	aetherClass::Sphere* GetCollider();
	void ChangePivotState();

private:
	void Destroy();
private:
	std::unique_ptr<FbxObjectInfo> m_fbxObject;
};

#endif