#ifndef _WORLD_H
#define _WORLD_H
#include <string>
#include <vector>
#include <Vector3.h>
#include "AetherObjectInfo.h"
struct World{
	std::vector<ObjectInfo> _objectInfo;
	std::vector<aetherClass::Material> _material;
	std::vector<aetherClass::Texture*> _texture;
	CameraInfo _camera;
	aetherClass::Vector3 _lightPosition;
};
#endif