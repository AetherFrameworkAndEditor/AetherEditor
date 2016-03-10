#ifndef _SPRITEOBJECT_H
#define _SPRITEOBJECT_H
#include <Sphere.h>
#include <SpriteBase.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include <memory>
#include "Pivot.h"
class SpriteObject
{
private:
	struct SpriteObjectInfo{
		SpriteObjectInfo(){
			_sprite = nullptr;
			_spriteCollider = nullptr;
			_pivot = nullptr;
			_isClick = false;
		}

		~SpriteObjectInfo(){
			Destory();
		}

		//
		void Destory(){
			if (_sprite)
			{
				_sprite->Finalize();
				_sprite.release();
				_sprite = nullptr;
			}

			//
			if (_spriteCollider)
			{
				_spriteCollider->Finalize();
				_spriteCollider.release();
				_spriteCollider = nullptr;
			}

			//
			if (_pivot)
			{
				_pivot.release();
				_pivot = nullptr;
			}
			_isClick = false;
		}
		std::unique_ptr<aetherClass::SpriteBase> _sprite;
		std::unique_ptr<aetherClass::Sphere> _spriteCollider;
		std::unique_ptr<Pivot> _pivot;
		bool _isClick;
	};

public:
	SpriteObject();
	~SpriteObject();
	bool Create(aetherClass::SpriteBase*, aetherClass::ViewCamera*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	aetherClass::Sphere* GetCollider();
	void ChangePivotState();

private:
	void Destroy();
private:
	std::unique_ptr<SpriteObjectInfo> m_spriteObject;
};

#endif