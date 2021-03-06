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
enum class eObjectType{
	eNull,
	ePrimitive,
	eSprite,
	eFBX,
	eCamera,
	eLight,
};

struct CurrentSelectObject{
	CurrentSelectObject(){
		_objectType = eObjectType::eNull;
		_number = NULL;
	}
	eObjectType _objectType;
	int _number;
};

class WorldObjectManager
{
public:
	static void AddPrimitive(PrimitiveObject*);
	static void AddSprite(SpriteObject*);
	static void AddFbxModel(FbxModelObject*);
	static void RegisterCameraValue(CameraValue);
	static void RegisterLightValue(LightValue);

	static std::vector<PrimitiveObject*>& GetPrimitive();
	static std::vector<SpriteObject*>& GetSprite();
	static std::vector<FbxModelObject*>& GetFbxModel();
	static CameraValue& GetCameraValue();
	static LightValue& GetLightValue();

	static bool Import(std::string);
	static bool Export(std::wstring);
	static void Reset();

	static void SetCurrentSelectObject(CurrentSelectObject);
	static CurrentSelectObject GetCurrentSelectObject();
	static bool IsPlay();
	static void IsPlay(bool);
	
private:
	WorldObjectManager();
	~WorldObjectManager();

	static void CreateFBX(ObjectInfo*);
	static void CreateSprite(ObjectInfo*);
	static void CreatePrimitive(ObjectInfo*);

	static void SelectOff();
private:
	static std::vector<PrimitiveObject*> m_primitive;
	static std::vector<SpriteObject*> m_sprite;
	static std::vector<FbxModelObject*> m_fbx;
	static std::vector<aetherClass::Material*> m_material;
	static std::vector<aetherClass::Texture*> m_texture;

	static LightValue m_lightValue;
	static CameraValue m_cameraValue;

	static std::string m_modelType;
	static CurrentSelectObject m_currnetSelectObject;
	static bool m_play;

};



#endif