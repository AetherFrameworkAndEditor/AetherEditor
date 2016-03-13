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

Vector3 WorldObjectManager::m_light = Vector3(NULL,NULL,NULL);
CameraValue WorldObjectManager::m_camera;

WorldObjectManager::WorldObjectManager()
{
}


WorldObjectManager::~WorldObjectManager()
{
}

//
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
	// ロードした後の処理

	reader->UnLoad();
	reader.release();
	reader = nullptr;
	return true;
}

//
bool WorldObjectManager::Export(std::string fileName){

	// ディレクトリがないときはディレクトリを作成
	std::string directoryName = "WorldData";
	_mkdir(directoryName.c_str());

	// 相対パスを含めと拡張子を含めたファイル名にする
	std::string exprotFileName = directoryName + "\\" + fileName + ".aether";

	// ファイルに書き込むオブジェクト
	std::ofstream exportObject(exprotFileName, std::ios::out);

	// ワールド情報を書き出すオブジェクト
	WorldWriter writer;

	// Objectタグの設定と書き出し
	exportObject << "[Object]" << std::endl;
	writer.WritePrimitive(exportObject,nullptr);
	writer.WriteSprite(exportObject, nullptr);
	writer.WriteFbxModel(exportObject, nullptr);
	
	// Materialタグの設定と書き出し
	exportObject << "[Material]" << std::endl;
	writer.WriteMaterial(exportObject, nullptr);

	// Textureタグの設定と書き出し
	exportObject << "[Texture]" << std::endl;
	writer.WriteTexture(exportObject, nullptr);

	// Cameraタグの設定と書き出し
	exportObject << "[Camera]" << std::endl;
	writer.WriteCamera(exportObject, m_camera);

	// Lightタグの設定と書き出し
	exportObject << "[Light]" << std::endl;
	writer.WriteLight(exportObject, m_light);

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
	m_camera._position = NULL;
	m_camera._rotation = NULL;

	//
	m_light = NULL;
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
void WorldObjectManager::RegisterCamera(Vector3 light){
	m_light = light;
	return;
}

//
void WorldObjectManager::RegisterLight(CameraValue camera){
	m_camera = camera;
	return;
}

//
std::vector<PrimitiveObject*> WorldObjectManager::GetPrimitive(){
	return m_primitive;
}

//
std::vector<SpriteObject*> WorldObjectManager::GetSprite(){
	return m_sprite;
}

//
std::vector<FbxModelObject*> WorldObjectManager::GetFbxModel(){
	return m_fbx;
}

//
CameraValue WorldObjectManager::GetCamera(){
	return m_camera;
}

//
Vector3 WorldObjectManager::GetLight(){
	return m_light;
}