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
			_pivot = nullptr;
			_isClick = false;
			_name = "null_name";
			_materialID = 0;
			_textureID = 0;
		}

		~SpriteObjectInfo(){
			Destory();
		}
		
		std::unique_ptr<aetherClass::SpriteBase> _sprite;
		std::unique_ptr<Pivot> _pivot;
		int _materialID;
		int _textureID;
		bool _isClick;
		std::string _name;

	private:
		//
		void Destory(){
			if (_sprite)
			{
				_sprite->Finalize();
				_sprite.release();
				_sprite = nullptr;
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
	};

public:
	SpriteObject();
	~SpriteObject();
	bool Create(aetherClass::SpriteBase*);

	void Render(aetherClass::ShaderBase*);
	void Update();

	void ChangePivotState();

	std::unique_ptr<SpriteObjectInfo>& GetInfo();
private:
	void Destroy();
private:
	std::unique_ptr<SpriteObjectInfo> m_spriteObject;
};

#endif