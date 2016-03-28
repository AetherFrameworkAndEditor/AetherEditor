#ifndef _AETHEROBJECTINFO_H
#define _AETHEROBJECTINFO_H
#include <Transform.h>
#include <Vector3.h>
#include <Color.h>
#include <Material.h>
#include <string>

struct CameraValue{
	CameraValue(){
		_position = 0;
		_rotation = 0;
		_isClick = false;
	}
	aetherClass::Vector3 _position;
	aetherClass::Vector3 _rotation;
	bool _isClick;
};

struct LightValue{
	LightValue(){
		_position = 0;
		_isClick = false;
	}

	aetherClass::Vector3 _position;
	bool _isClick;
};

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
	std::string _name;
	std::string _path;

private:
	void Reset(){
		_materialID = 0;
		_modelType = "none";
		_color = aetherClass::Color(0, 0, 0, 1);
		_textureID = 0;
		_name = "none";
		_path = "null";
	}
};

#endif