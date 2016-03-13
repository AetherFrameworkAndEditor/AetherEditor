#ifndef _WORLDOBJECTMANAGER_H
#define _WORLDOBJECTMANAGER_H
#include "PrimitiveObject.h"
#include "SpriteObject.h"
#include "FbxModelObject.h"
#include "WorldReader.h"
#include "WorldAllObjectInfo.h"
#include <ViewCamera.h>
#include <Light.h>
class WorldObjectManager
{
public:
	static void AddPrimitive(PrimitiveObject*);
	static void AddSprite(SpriteObject*);
	static void AddFbxModel(FbxModelObject*);
	static void RegisterCamera(CameraValue);
	static void RegisterLight(aetherClass::Vector3);

	static std::vector<PrimitiveObject*> GetPrimitive();
	static std::vector<SpriteObject*> GetSprite();
	static std::vector<FbxModelObject*> GetFbxModel();
	static CameraValue GetCamera();
	static aetherClass::Vector3 GetLight();

	static bool Import(std::string);
	static bool Export(std::string);
	static void Reset();

private:
	WorldObjectManager();
	~WorldObjectManager();
private:
	static std::vector<PrimitiveObject*> m_primitive;
	static std::vector<SpriteObject*> m_sprite;
	static std::vector<FbxModelObject*> m_fbx;
	static std::vector<aetherClass::Material*> m_material;
	static std::vector<aetherClass::Texture*> m_texture;

	static aetherClass::Vector3 m_light;
	static CameraValue m_camera;
};

#endif