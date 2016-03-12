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
			_name = "none";
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
			_materialID = 0;
			_textureID = 0;
		}
		std::unique_ptr<aetherClass::FbxModel> _fbx;
		std::unique_ptr<aetherClass::Sphere> _fbxCollider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;
		int _materialID;
		int _textureID;
		std::string _name;
	};
public:
	FbxModelObject();
	~FbxModelObject();

	bool Create(std::string, aetherClass::ViewCamera*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	aetherClass::Sphere* GetCollider();
	void ChangePivotState();
	std::unique_ptr<FbxObjectInfo>& GetInfo();

private:
	void Destroy();
private:
	std::unique_ptr<FbxObjectInfo> m_fbxObject;
};

#endif