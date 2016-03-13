#include "WorldObjectManager.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include "WorldWriter.h"

//primitive
#include"Point.h"
#include"Line.h"
#include"Circle.h"
#include"Triangle.h"
#include"Rectangle3D.h"
#include"Cube.h"
#include"Cylinder.h"
#include"Sphere.h"

// sprite
#include"Text.h"
#include"Rectangle2D.h"
using namespace aetherClass;
std::vector<PrimitiveObject*> WorldObjectManager::m_primitive;
std::vector<SpriteObject*>    WorldObjectManager::m_sprite;
std::vector<FbxModelObject*>  WorldObjectManager::m_fbx;
std::vector<Material*> WorldObjectManager::m_material;
std::vector<Texture*> WorldObjectManager::m_texture;

Vector3 WorldObjectManager::m_lightValue = Vector3(NULL,NULL,NULL);
Light WorldObjectManager::m_light;
CameraValue WorldObjectManager::m_cameraValue;
ViewCamera WorldObjectManager::m_camera;
std::string WorldObjectManager::m_modelType = "none";

//
static ModelBase* GetPrimitiveModel(std::string type){
	if (type == "Point"){
		return new Point();
	}
	else if (type == "Line"){
		return new Line();
	}
	else if (type == "Circle"){
		return new Circle();
	}
	else if (type == "Triangle"){
		return new Triangle();
	}
	else if (type == "Rectangle3D"){
		return new Rectangle3D();
	}
	else if (type == "Cube"){
		return new Cube();
	}
	else if (type == "Cylinder"){
		return new Cylinder();
	}
	else if (type == "Sphere"){
		return new Sphere(10,10);
	}
	else{
		assert(!"不正な値を検出しました.開発者に連絡してください");
	}
}


WorldObjectManager::WorldObjectManager()
{
}


WorldObjectManager::~WorldObjectManager()
{
}

//
bool WorldObjectManager::Import(std::string path){

	Reset();
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

	// カメラ
	RegisterCameraValue(reader->GetInputWorldInfo()._camera);

	// ライト
	RegisterLightValue(reader->GetInputWorldInfo()._lightPosition);

	for (auto object : reader->GetInputWorldInfo()._object)
	{
		// モデルタイプの取得
		m_modelType = object->_modelType;

		CreateFBX(object);
		CreateSprite(object);
		CreatePrimitive(object);
	}
	reader->UnLoad();
	reader.release();
	reader = nullptr;
	return true;
}

//
bool WorldObjectManager::Export(std::wstring fileName){

	// 相対パスを含めと拡張子を含めたファイル名にする
	std::wstring exprotFileName =  fileName;

	// ファイルに書き込むオブジェクト
	std::ofstream exportObject(exprotFileName, std::ios::out);

	// ワールド情報を書き出すオブジェクト
	WorldWriter writer;

	// Objectタグの設定と書き出し
	exportObject << "[Object]" << std::endl;

	//
	for (auto primitive : m_primitive)
	{
		if (primitive)
		{
			writer.WritePrimitive(exportObject, primitive);
		}
	}

	//
	for (auto sprite : m_sprite)
	{
		if (sprite)
		{
			writer.WriteSprite(exportObject, sprite);
		}
	}
	
	//
	for (auto fbx : m_fbx)
	{
		if (fbx)
		{
			writer.WriteFbxModel(exportObject, fbx);
		}
	}

	// Materialタグの設定と書き出し
	exportObject << "[Material]" << std::endl;

	//
	for (auto material : m_material)
	{
		if (material)
		{
			writer.WriteMaterial(exportObject, material);
		}
	}

	// Textureタグの設定と書き出し
	exportObject << "[Texture]" << std::endl;
	writer.WriteTexture(exportObject, nullptr);
	for (auto texture : m_texture)
	{
		if (texture)
		{
			writer.WriteTexture(exportObject, texture);
		}
	}
	// Cameraタグの設定と書き出し
	exportObject << "[Camera]" << std::endl;
	writer.WriteCamera(exportObject, m_cameraValue);

	// Lightタグの設定と書き出し
	exportObject << "[Light]" << std::endl;
	writer.WriteLight(exportObject, m_lightValue);

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
	m_cameraValue._position = NULL;
	m_cameraValue._rotation = NULL;

	//
	m_lightValue = NULL;
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
void WorldObjectManager::RegisterCameraValue(CameraValue camera){
	m_cameraValue = camera;
	m_camera.property._translation = camera._position;
	m_camera.property._rotation = camera._rotation;
	return;
}

//
void WorldObjectManager::RegisterLightValue(Vector3 lightValue){
	m_lightValue = lightValue;
	m_light.property._translation = m_lightValue;
	return;
}

//
std::vector<PrimitiveObject*>& WorldObjectManager::GetPrimitive(){
	return m_primitive;
}

//
std::vector<SpriteObject*>& WorldObjectManager::GetSprite(){
	return m_sprite;
}

//
std::vector<FbxModelObject*>& WorldObjectManager::GetFbxModel(){
	return m_fbx;
}

//
CameraValue WorldObjectManager::GetCameraValue(){
	return m_cameraValue;
}

//
ViewCamera WorldObjectManager::GetCamera(){
	return m_camera;
}

//
Vector3 WorldObjectManager::GetLightValue(){
	return m_lightValue;
}

//
Light WorldObjectManager::GetLight(){
	return m_light;
}

//
void WorldObjectManager::CreateFBX(ObjectInfo* object){
	if (object->_modelType != "FbxModel") return;

	FbxModelObject* fbx = new FbxModelObject();
	bool result = fbx->Create(object->_name, &m_camera);
	if (!result)
	{
		return;
	}

	fbx->GetInfo()->_fbx->property._transform = object->_transform;
	fbx->GetInfo()->_path = object->_name;

	// 登録
	AddFbxModel(fbx);
	return;
}

//
void WorldObjectManager::CreatePrimitive(ObjectInfo* object){
	if (object->_modelType == "FbxModel" || object->_modelType == "Text" ||
		object->_modelType == "Rectangle2D") return;

	PrimitiveObject* primitive = new PrimitiveObject();

	bool result = primitive->Create(GetPrimitiveModel(object->_modelType), &m_camera);

	if (!result)
	{
		return;
	}
	primitive->GetInfo()->_name = object->_name;
	primitive->GetInfo()->_primitive->property._transform = object->_transform;
	primitive->GetInfo()->_primitive->property._color = object->_color;
	// 登録
	AddPrimitive(primitive);
	return;
}

//
void WorldObjectManager::CreateSprite(ObjectInfo* object){
	if (object->_modelType == "Text" || object->_modelType == "Rectangle2D"){
		auto* sprite = new SpriteObject();

		bool result = false;
		if (object->_modelType == "Text"){
			auto text = new Text();

			FontDesc fontDesc;
			fontDesc._fontSize = 30;
			fontDesc._charSet = SHIFTJIS_CHARSET;
			fontDesc._fontName = L"メイリオ";

			Font* font = new Font();
			font->Load(fontDesc);
			text->SetFont(font);
			sprite->Create(text);
			text->UpdateText(L"文字");
			
			AddSprite(sprite);
		}
		else if (object->_modelType == "Rectangle2D"){
			result = sprite->Create(new Rectangle2D());


			if (!result)
			{
				return;
			}

			sprite->GetInfo()->_name = object->_name;
			sprite->GetInfo()->_sprite->property._transform = object->_transform;
			sprite->GetInfo()->_sprite->property._color = object->_color;

			// 登録
			AddSprite(sprite);
		}
	}
	return;
}