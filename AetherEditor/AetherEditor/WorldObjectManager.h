#ifndef _WORLDOBJECTMANAGER_H
#define _WORLDOBJECTMANAGER_H
#include "PrimitiveObject.h"
#include "SpriteObject.h"
#include "FbxModelObject.h"
#include "WorldReader.h"
#include "WorldAllObjectInfo.h"

#include <unordered_map>
#include <ViewCamera.h>
#include <Light.h>
class WorldObjectManager
{
public:
	static void AddPrimitive(PrimitiveObject*);
	static void AddSprite(SpriteObject*);
	static void AddFbxModel(FbxModelObject*);
	static void RegisterCameraValue(CameraValue);
	static void RegisterLightValue(aetherClass::Vector3);

	static std::vector<PrimitiveObject*>& GetPrimitive();
	static std::vector<SpriteObject*>& GetSprite();
	static std::vector<FbxModelObject*>& GetFbxModel();
	static CameraValue GetCameraValue();
	static aetherClass::Vector3 GetLightValue();
	static aetherClass::ViewCamera GetCamera();
	static aetherClass::Light GetLight();
	static bool Import(std::string);
	static bool Export(std::wstring);
	static void Reset();
private:
	WorldObjectManager();
	~WorldObjectManager();

	static void CreateFBX(ObjectInfo*);
	static void CreateSprite(ObjectInfo*);
	static void CreatePrimitive(ObjectInfo*);
private:
	static std::vector<PrimitiveObject*> m_primitive;
	static std::vector<SpriteObject*> m_sprite;
	static std::vector<FbxModelObject*> m_fbx;
	static std::vector<aetherClass::Material*> m_material;
	static std::vector<aetherClass::Texture*> m_texture;

	static aetherClass::Vector3 m_lightValue;
	static aetherClass::Light m_light;
	static CameraValue m_cameraValue;
	static aetherClass::ViewCamera m_camera;
	static std::string m_modelType;

};



#endif