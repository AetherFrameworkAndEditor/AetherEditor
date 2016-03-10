#ifndef _AETHEROBJECTINFO_H
#define _AETHEROBJECTINFO_H
#include <Transform.h>
#include <Vector3.h>
#include <Color.h>
#include <Material.h>
#include <string>
struct ObjectInfo
{
	ObjectInfo(){
		Reset();
	}

	~ObjectInfo(){
		Reset();
	}
	aetherClass::Transform _transform;
	int _materialID;
	std::string _modelType;
	aetherClass::Color _color;
	int _textureID;

private:
	void Reset(){
		_materialID = 0;
		_modelType = "none";
		_color = aetherClass::Color(0, 0, 0, 0);
		_textureID = 0;
	}
};

// 
struct CameraInfo
{
	aetherClass::Vector3 _rotation;
	aetherClass::Vector3 _translation;
};

#endif