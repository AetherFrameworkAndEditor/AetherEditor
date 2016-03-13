#ifndef _WORLD_H
#define _WORLD_H
#include <string>
#include <vector>
#include <Vector3.h>
#include "WorldAllObjectInfo.h"
struct World{
	std::vector<ObjectInfo*> _objectInfo;
	std::vector<aetherClass::Material*> _material;
	std::vector<aetherClass::Texture*> _texture;
	CameraValue _camera;
	aetherClass::Vector3 _lightPosition;
};
#endif