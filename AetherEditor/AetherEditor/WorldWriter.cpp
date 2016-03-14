#include "WorldWriter.h"
#include <Cube.h>
using namespace aetherClass;
WorldWriter::WorldWriter()
{
}


WorldWriter::~WorldWriter()
{
}

//
void WorldWriter::WritePrimitive(std::ofstream& exportObject, PrimitiveObject* object){
	
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// 位置
	exportObject << object->GetInfo()->_primitive->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._translation._z;
	exportObject << ',';

	// 回転
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._z;
	exportObject << ',';

	// スケール
	exportObject << object->GetInfo()->_primitive->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._scale._z;
	exportObject << ',';

	// マテリアルID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// モデルタイプ
	exportObject << object->GetInfo()->_primitive->GetName();
	exportObject << ',';

	// カラー
	exportObject << object->GetInfo()->_primitive->property._color._red;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._color._green;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._color._blue;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._color._alpha;
	exportObject << ',';

	// TextureID
	exportObject << object->GetInfo()->_textureID;
	exportObject << ',';

	// それぞれの名前
	exportObject << object->GetInfo()->_name;
	exportObject << ',';

	// オブジェクトのパス
	exportObject << "null_path";

	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteSprite(std::ofstream& exportObject, SpriteObject* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// 位置
	exportObject << object->GetInfo()->_sprite->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._translation._z;
	exportObject << ',';

	// 回転
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._z;
	exportObject << ',';

	// スケール
	exportObject << object->GetInfo()->_sprite->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._scale._z;
	exportObject << ',';

	// マテリアルID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// モデルタイプ
	exportObject << object->GetInfo()->_sprite->GetName();
	exportObject << ',';

	// カラー
	exportObject << object->GetInfo()->_sprite->property._color._red;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._color._green;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._color._blue;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._color._alpha;
	exportObject << ',';

	// TextureID
	exportObject << object->GetInfo()->_textureID;
	exportObject << ',';

	// それぞれの名前
	exportObject << object->GetInfo()->_name;
	exportObject << ',';

	// オブジェクトのパス
	exportObject << "null_path";

	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteFbxModel(std::ofstream& exportObject, FbxModelObject* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// 位置
	exportObject << object->GetInfo()->_fbx->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._translation._z;
	exportObject << ',';

	// 回転
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._z;
	exportObject << ',';

	// スケール
	exportObject << object->GetInfo()->_fbx->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._scale._z;
	exportObject << ',';

	// マテリアルID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// モデルタイプ
	// とりあえずFBXだけモデルのパスを吐き出すようにする
	exportObject << "FbxModel";
	exportObject << ',';

	// カラー
	exportObject << NULL;
	exportObject << ',';
	exportObject << NULL;
	exportObject << ',';
	exportObject << NULL;
	exportObject << ',';
	exportObject << NULL;
	exportObject << ',';

	// TextureID
	exportObject << object->GetInfo()->_textureID;
	exportObject << ',';

	// それぞれの名前
	exportObject << object->GetInfo()->_name;
	exportObject << ',';

	// オブジェクトのパス
	exportObject << object->GetInfo()->_fbx->GetModelPath();

	exportObject << std::endl;
}

//
void WorldWriter::WriteMaterial(std::ofstream& exportObject, Material* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// diffuse
	exportObject << object->_diffuse._color._red;
	exportObject << ',';
	exportObject << object->_diffuse._color._green;
	exportObject << ',';
	exportObject << object->_diffuse._color._blue;
	exportObject << ',';
	exportObject << object->_diffuse._color._alpha;
	exportObject << ',';
	exportObject << object->_diffuse._texturePath[0];
	exportObject << ',';

	// ambient
	exportObject << object->_ambient._color._red;
	exportObject << ',';
	exportObject << object->_ambient._color._green;
	exportObject << ',';
	exportObject << object->_ambient._color._blue;
	exportObject << ',';
	exportObject << object->_ambient._color._alpha;
	exportObject << ',';
	exportObject << object->_ambient._texturePath[0];
	exportObject << ',';

	// specular
	exportObject << object->_specular._color._red;
	exportObject << ',';
	exportObject << object->_specular._color._green;
	exportObject << ',';
	exportObject << object->_specular._color._blue;
	exportObject << ',';
	exportObject << object->_specular._color._alpha;
	exportObject << ',';
	exportObject << object->_specular._texturePath[0];
	exportObject << ',';

	// emissive
	exportObject << object->_emissive._color._red;
	exportObject << ',';
	exportObject << object->_emissive._color._green;
	exportObject << ',';
	exportObject << object->_emissive._color._blue;
	exportObject << ',';
	exportObject << object->_emissive._color._alpha;
	exportObject << ',';
	exportObject << object->_emissive._texturePath[0];

	exportObject << std::endl;
}

//
void WorldWriter::WriteTexture(std::ofstream& exportObject, Texture* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	exportObject << object->GetPath();
	exportObject << std::endl;
}



//
void WorldWriter::WriteCamera(std::ofstream& exportObject, CameraValue object){
	
	// 位置
	exportObject << object._position._x;
	exportObject << ',';
	exportObject << object._position._y;
	exportObject << ',';
	exportObject << object._position._z;
	exportObject << ',';

	// 回転
	exportObject << object._rotation._x;
	exportObject << ',';
	exportObject << object._rotation._y;
	exportObject << ',';
	exportObject << object._rotation._z;
	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteLight(std::ofstream& exportObject, Vector3 object){
	
	// 位置
	exportObject << object._x;
	exportObject << ',';
	exportObject << object._y;
	exportObject << ',';
	exportObject << object._z;
	return;
}