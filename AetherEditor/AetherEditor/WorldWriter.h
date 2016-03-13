#ifndef _WORLDWRITER_H
#define _WORLDWRITER_H
#include <fstream>
#include "PrimitiveObject.h"
#include "FbxModelObject.h"
#include "SpriteObject.h"
#include "WorldAllObjectInfo.h"
#include <Light.h>
#include <ViewCamera.h>
class WorldWriter
{
public:
	WorldWriter();
	~WorldWriter();

	void WritePrimitive(std::ofstream&, PrimitiveObject*);
	void WriteSprite(std::ofstream&, SpriteObject*);
	void WriteFbxModel(std::ofstream&, FbxModelObject*);

	void WriteMaterial(std::ofstream&, aetherClass::Material*);
	void WriteTexture(std::ofstream&, aetherClass::Texture*);

	void WriteCamera(std::ofstream&, CameraValue);
	void WriteLight(std::ofstream&, aetherClass::Vector3);
};

#endif