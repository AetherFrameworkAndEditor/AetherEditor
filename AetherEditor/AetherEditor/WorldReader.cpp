#include "WorldReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace aetherClass;
WorldReader::WorldReader(){
	m_tag = "none";
	m_input.clear();
}


WorldReader::~WorldReader(){

}


bool WorldReader::Load(std::string filePath){
	// 独自ファイル以外は読み込まない
	if (GetExtension(filePath) != "aether")
	{
		return false;
	}
	std::ifstream reader;
	reader.open(filePath,std::ios::in);

	while (!reader.eof())
	{
		std::string line;
		reader >> line;
		m_input.push_back(line);
	}

	for (auto index : m_input)
	{
		// タグの行かを確認
		if (index == "Object" || index == "Material" ||
			index == "Texture"||index == "Camera" || 
			index == "Light")
		{
			// 取り込んでいるタグを設定
			m_tag = index;
			continue;
		}

		// それぞれの現在のタグに合わせた読み込み
		GetObjectInfo(index);
		GetMaterialInfo(index);
		GetCameraInfo(index);
		GetLightInfo(index);
	}

	reader.close();

	return true;
}

// 情報破棄
void WorldReader::UnLoad(){
	for (auto texture : m_world._texture)
	{
		// ポインタの破棄
		if (!texture)continue;
		delete texture;
		texture = nullptr;
	}
}
// オブジェクト情報を読み取る
void WorldReader::GetObjectInfo(std::string data){

	// タグが違っていたら何もしない
	if (m_tag != "Object")return;
	ObjectInfo info;
	std::string split;
	std::vector<std::string> spliteArray;
	std::stringstream stream(data);
	while (std::getline(stream,split,','))
	{
		spliteArray.push_back(split);
	}

	// 位置
	info._transform._translation._x = std::atof(spliteArray[0].c_str());
	info._transform._translation._y = std::atof(spliteArray[1].c_str());
	info._transform._translation._z = std::atof(spliteArray[2].c_str());

	// 回転
	info._transform._rotation._x = std::atof(spliteArray[3].c_str());
	info._transform._rotation._y = std::atof(spliteArray[4].c_str());
	info._transform._rotation._z = std::atof(spliteArray[5].c_str());

	// 拡大縮小
	info._transform._scale._x = std::atof(spliteArray[6].c_str());
	info._transform._scale._y = std::atof(spliteArray[7].c_str());
	info._transform._scale._z = std::atof(spliteArray[8].c_str());

	// 適用するマテリアル番号
	info._materialID = std::atoi(spliteArray[9].c_str());
	
	// モデルの種類
	info._modelType = spliteArray[10];

	// 色
	info._color._red = std::atof(spliteArray[11].c_str());
	info._color._green = std::atof(spliteArray[12].c_str());
	info._color._blue = std::atof(spliteArray[13].c_str());
	info._color._alpha = std::atof(spliteArray[14].c_str());

	// 適用するテクスチャ番号
	info._textureID = std::atoi(spliteArray[15].c_str());

	// ワールドに取り込む
	m_world._objectInfo.push_back(info);
	return;
}

// マテリアルを取り込む
void WorldReader::GetMaterialInfo(std::string data){

	// タグが違っていたら何もしない
	if (m_tag != "Material")return;
	Material info;
	std::string split;
	std::vector<std::string> spliteArray;
	std::stringstream stream(data);
	while (std::getline(stream, split, ','))
	{
		spliteArray.push_back(split);
	}

	// diffuse
	info._diffuse._color._red = std::atof(spliteArray[0].c_str());
	info._diffuse._color._green = std::atof(spliteArray[1].c_str());
	info._diffuse._color._blue = std::atof(spliteArray[2].c_str());
	info._diffuse._color._alpha = std::atof(spliteArray[3].c_str());
	info._diffuse._texturePath.push_back(spliteArray[4]);

	// ambient
	info._ambient._color._red = std::atof(spliteArray[0].c_str());
	info._ambient._color._green = std::atof(spliteArray[1].c_str());
	info._ambient._color._blue = std::atof(spliteArray[2].c_str());
	info._ambient._color._alpha = std::atof(spliteArray[3].c_str());
	info._ambient._texturePath.push_back(spliteArray[4]);

	// specular
	info._specular._color._red = std::atof(spliteArray[0].c_str());
	info._specular._color._green = std::atof(spliteArray[1].c_str());
	info._specular._color._blue = std::atof(spliteArray[2].c_str());
	info._specular._color._alpha = std::atof(spliteArray[3].c_str());
	info._specular._texturePath.push_back(spliteArray[4]);

	// emissive
	info._emissive._color._red = std::atof(spliteArray[0].c_str());
	info._emissive._color._green = std::atof(spliteArray[1].c_str());
	info._emissive._color._blue = std::atof(spliteArray[2].c_str());
	info._emissive._color._alpha = std::atof(spliteArray[3].c_str());
	info._emissive._texturePath.push_back(spliteArray[4]);

	m_world._material.push_back(info);

	return;
}

// テクスチャの取りこみ
void WorldReader::GetTextureInfo(std::string data){
	// タグが違っていたら何もしない
	if (m_tag != "Texture")return;
	
	Texture* texture = new Texture();
	texture->Load(data);

	m_world._texture.push_back(texture);

	return;
}

// カメラの情報を取り込む
void WorldReader::GetCameraInfo(std::string data){

	// タグが違っていたら何もしない
	if (m_tag != "Camera")return;
	CameraInfo info;
	std::string split;
	std::vector<std::string> spliteArray;
	std::stringstream stream(data);
	while (std::getline(stream, split, ','))
	{
		spliteArray.push_back(split);
	}

	// 位置情報
	info._translation._x = std::atof(spliteArray[0].c_str());
	info._translation._y = std::atof(spliteArray[1].c_str());
	info._translation._z = std::atof(spliteArray[2].c_str());

	// 回転情報
	info._rotation._x = std::atof(spliteArray[3].c_str());
	info._rotation._y = std::atof(spliteArray[4].c_str());
	info._rotation._z = std::atof(spliteArray[5].c_str());

	m_world._camera = info;
	return;
}


void WorldReader::GetLightInfo(std::string data){

	// タグが違っていたら何もしない
	if (m_tag != "Light")return;
	Vector3 info;
	std::string split;
	std::vector<std::string> spliteArray;
	std::stringstream stream(data);
	while (std::getline(stream, split, ','))
	{
		spliteArray.push_back(split);
	}

	// 位置情報
	info._x = std::atof(spliteArray[0].c_str());
	info._y = std::atof(spliteArray[1].c_str());
	info._z = std::atof(spliteArray[2].c_str());

	m_world._lightPosition = info;
	return;
}

// フルパスから拡張子名を小文字で取り込む
std::string WorldReader::GetExtension(const std::string path){
	std::string extension;
	size_t pos = path.rfind('.');
	if (pos != std::string::npos)
	{
		extension = path.substr(pos + 1, path.size() - pos);
		std::string::iterator stringItr = extension.begin();

		while (stringItr != extension.end())
		{
			*stringItr = tolower(*stringItr);
			stringItr++;
		}

		stringItr = extension.end() - 1;

		while (stringItr != extension.begin())
		{
			if (*stringItr == 0 || *stringItr == 32)
			{
				extension.erase(stringItr--);
			}
			else
			{
				stringItr--;
			}
		}
	}
	return extension;
}