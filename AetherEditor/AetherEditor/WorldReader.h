#ifndef _WORLDREADER_H
#define _WORLDREADER_H
#include <string>
#include "World.h"
class WorldReader
{
public:
	WorldReader();
	~WorldReader();

	bool Load(std::string);
	World GetInputWorldInfo();
	void UnLoad();

	void UnLoadTexture();
	void UnLoadObject();
	void UnLoadMaterial();
private:
	World m_world;
	std::vector<std::string> m_input;
	std::string m_tag;
private:
	std::string GetExtension(const std::string path);
	void GetObjectInfo(std::string);
	void GetMaterialInfo(std::string);
	void GetCameraInfo(std::string);
	void GetTextureInfo(std::string);
	void GetLightInfo(std::string);
};

#endif