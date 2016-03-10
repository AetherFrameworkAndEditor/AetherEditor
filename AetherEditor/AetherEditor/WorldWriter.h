#ifndef _WORLDWRITER_H
#define _WORLDWRITER_H
#include <fstream>
#include "PrimitiveObject.h"
#include "FbxModelObject.h"
#include "SpriteObject.h"
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

	void WriteCamera(std::ofstream&, aetherClass::ViewCamera*);
	void WriteLight(std::ofstream&, aetherClass::Light*);
};

#endif