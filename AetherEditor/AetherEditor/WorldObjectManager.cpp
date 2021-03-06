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

LightValue WorldObjectManager::m_lightValue;

CameraValue WorldObjectManager::m_cameraValue;

std::string WorldObjectManager::m_modelType = "none";
CurrentSelectObject WorldObjectManager::m_currnetSelectObject;
bool WorldObjectManager::m_play = false;
// リセット時に呼び出す
template<typename Type>
static void ResetVector(std::vector<Type>& vector)
{
	if (!vector.empty())
	{
		//
		for (auto object : vector)
		{
			if (object)
			{
				delete object;
				object = nullptr;
			}
		}
	}
	vector.clear();
}
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

bool WorldObjectManager::IsPlay(){
	return m_play;
}

void WorldObjectManager::IsPlay(bool flg){
	m_play = flg;
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
	LightValue readLightValue;
	readLightValue._position = reader->GetInputWorldInfo()._lightPosition;
	readLightValue._isClick = false;
	RegisterLightValue(readLightValue);

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
	writer.WriteLight(exportObject, m_lightValue._position);

	exportObject.close();
	return true;
}

//
void WorldObjectManager::Reset(){

	//
	ResetVector<PrimitiveObject*>(m_primitive);

	//
	ResetVector<SpriteObject*>(m_sprite);

	//
	ResetVector<FbxModelObject*>(m_fbx);

	//
	m_cameraValue._position = NULL;
	m_cameraValue._rotation = NULL;
	m_cameraValue._isClick = NULL;
	//
	m_lightValue._position = NULL;
	m_lightValue._isClick = NULL;

	//
	m_currnetSelectObject._objectType = eObjectType::eNull;
	return;
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

	return;
}

//
void WorldObjectManager::RegisterLightValue(LightValue lightValue){
	m_lightValue = lightValue;

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
CameraValue& WorldObjectManager::GetCameraValue(){
	return m_cameraValue;
}


//
LightValue& WorldObjectManager::GetLightValue(){
	return m_lightValue;
}


//
void WorldObjectManager::CreateFBX(ObjectInfo* object){
	if (object->_modelType != "FbxModel") return;

	FbxModelObject* fbx = new FbxModelObject();
	bool result = fbx->Create(object->_path, nullptr);
	if (!result)
	{
		MessageBox(NULL, L"Do not Load fbx", L"Error", MB_OK);
		return;
	}
	fbx->GetInfo()->_name = object->_name;
	fbx->GetInfo()->_fbx->property._transform = object->_transform;
	// 登録
	AddFbxModel(fbx);
	return;
}

//
void WorldObjectManager::CreatePrimitive(ObjectInfo* object){
	if (object->_modelType == "FbxModel" || object->_modelType == "Text" ||
		object->_modelType == "Rectangle2D") return;

	PrimitiveObject* primitive = new PrimitiveObject();

	bool result = primitive->Create(GetPrimitiveModel(object->_modelType), nullptr);

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

//
void WorldObjectManager::SetCurrentSelectObject(CurrentSelectObject currnet){
	if (m_currnetSelectObject._number == currnet._number && m_currnetSelectObject._objectType == currnet._objectType)return;
	SelectOff();
	m_currnetSelectObject = currnet;
}

//
CurrentSelectObject WorldObjectManager::GetCurrentSelectObject(){
	return m_currnetSelectObject;

}

void WorldObjectManager::SelectOff(){
	const int id = m_currnetSelectObject._number;
	switch (m_currnetSelectObject._objectType)
	{
	case eObjectType::eCamera:
		//カメラのフラグオフ
		m_cameraValue._isClick = false;
		break;
	case eObjectType::eLight:
		// ライトのフラグをオフ
		m_lightValue._isClick = false;
		break;

	case eObjectType::eFBX:
		m_fbx[id]->GetInfo()->_isClick = false;
		break;

	case eObjectType::ePrimitive:
		m_primitive[id]->GetInfo()->_isClick = false;
		break;

	case eObjectType::eSprite:
		m_sprite[id]->GetInfo()->_isClick = false;
		break;

	case eObjectType::eNull:
		break;
	default:
		break;
	}
}