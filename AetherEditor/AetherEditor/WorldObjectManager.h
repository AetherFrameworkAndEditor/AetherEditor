#ifndef _WORLDOBJECTMANAGER_H
#define _WORLDOBJECTMANAGER_H
#include "PrimitiveObject.h"
#include "SpriteObject.h"
#include "FbxModelObject.h"
#include "ViewCamera.h"
#include "Light.h"
#include "WorldReader.h"
class WorldObjectManager
{
public:
	static void AddPrimitive(PrimitiveObject*);
	static void AddSprite(SpriteObject*);
	static void AddFbxModel(FbxModelObject*);
	static void RegisterCamera(aetherClass::Light*);
	static void RegisterLight(aetherClass::ViewCamera*);

	static void Reset();

	static bool Import(std::string);
	static bool Export(std::string);
private:
	WorldObjectManager();
	~WorldObjectManager();
private:
	static std::vector<PrimitiveObject*> m_primitive;
	static std::vector<SpriteObject*> m_sprite;
	static std::vector<FbxModelObject*> m_fbx;
	static std::vector<aetherClass::Material*> m_material;
	static std::vector<aetherClass::Texture*> m_texture;

	static aetherClass::Light* m_light;
	static aetherClass::ViewCamera* m_camera;
};

#endif