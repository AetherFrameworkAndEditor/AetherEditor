#include "WorldObjectManager.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include "WorldWriter.h"
using namespace aetherClass;
std::vector<PrimitiveObject*> WorldObjectManager::m_primitive;
std::vector<SpriteObject*>    WorldObjectManager::m_sprite;
std::vector<FbxModelObject*>  WorldObjectManager::m_fbx;
std::vector<Material*> m_material;
std::vector<Texture*> m_texture;

Light* WorldObjectManager::m_light = nullptr;
ViewCamera* WorldObjectManager::m_camera = nullptr;

WorldObjectManager::WorldObjectManager()
{
}


WorldObjectManager::~WorldObjectManager()
{
}


bool WorldObjectManager::Import(std::string path){

	std::unique_ptr<WorldReader> reader;
	if (reader)
	{
		reader->UnLoad();
		reader.release();
		reader = nullptr;
	}

	reader = std::make_unique<WorldReader>();
	bool result = reader->Load(path);
	if (!result)
	{
		return false;
	}
	// ���[�h������̏���

	reader->UnLoad();
	reader.release();
	reader = nullptr;
	return true;
}

//
bool WorldObjectManager::Export(std::string fileName){

	// �f�B���N�g�����Ȃ��Ƃ��̓f�B���N�g�����쐬
	std::string directoryName = "WorldData";
	_mkdir(directoryName.c_str());

	// ���΃p�X���܂߂Ɗg���q���܂߂��t�@�C�����ɂ���
	std::string exprotFileName = directoryName + "\\" + fileName + ".aether";

	// �t�@�C���ɏ������ރI�u�W�F�N�g
	std::ofstream exportObject(exprotFileName, std::ios::out);

	// ���[���h���������o���I�u�W�F�N�g
	WorldWriter writer;

	// Object�^�O�̐ݒ�
	exportObject << "Object" << std::endl;
	writer.WritePrimitive(exportObject,nullptr);
	writer.WriteSprite(exportObject, nullptr);
	writer.WriteFbxModel(exportObject, nullptr);
	
	// Material�^�O�̐ݒ�
	exportObject << "Material" << std::endl;
	writer.WriteMaterial(exportObject, nullptr);

	// Texture�^�O�̐ݒ�
	exportObject << "Texture" << std::endl;
	writer.WriteTexture(exportObject, nullptr);

	// Camera�^�O�̐ݒ�
	exportObject << "Camera" << std::endl;
	writer.WriteCamera(exportObject, nullptr);

	// Light�^�O�̐ݒ�
	exportObject << "Light" << std::endl;
	writer.WriteLight(exportObject, nullptr);

	exportObject.close();
	return true;
}

//
void WorldObjectManager::Reset(){

	//
	for (auto primitive : m_primitive)
	{
		if (primitive)
		{
			delete primitive;
			primitive = nullptr;
		}
	}
	m_primitive.clear();

	//
	for (auto sprite : m_sprite)
	{
		if (sprite)
		{
			delete sprite;
			sprite = nullptr;
		}
	}
	m_sprite.clear();

	//
	for (auto fbx : m_fbx)
	{
		if (fbx)
		{
			delete fbx;
			fbx = nullptr;
		}
	}
	m_fbx.clear();

	//
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
	m_camera = nullptr;

	//
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}
	m_light = nullptr;
}
//
void WorldObjectManager::AddPrimitive(PrimitiveObject* primitive){
	m_primitive.push_back(primitive);

	return;
}

//
void WorldObjectManager::AddSprite(SpriteObject* sprite){
	m_sprite.push_back(sprite);

	return;
}

//
void WorldObjectManager::AddFbxModel(FbxModelObject* fbx){
	m_fbx.push_back(fbx);

	return;
}

//
void WorldObjectManager::RegisterCamera(aetherClass::Light* light){
	m_light = light;
	return;
}

//
void WorldObjectManager::RegisterLight(aetherClass::ViewCamera* camera){
	m_camera = camera;
	return;
}
