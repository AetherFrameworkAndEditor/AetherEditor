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
	// �ʒu
	exportObject << object->GetInfo()->_primitive->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._translation._z;
	exportObject << ',';

	// ��]
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._rotation._z;
	exportObject << ',';

	// �X�P�[��
	exportObject << object->GetInfo()->_primitive->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_primitive->property._transform._scale._z;
	exportObject << ',';

	// �}�e���A��ID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// ���f���^�C�v
	exportObject << object->GetInfo()->_primitive->GetName();
	exportObject << ',';

	// �J���[
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

	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteSprite(std::ofstream& exportObject, SpriteObject* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// �ʒu
	exportObject << object->GetInfo()->_sprite->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._translation._z;
	exportObject << ',';

	// ��]
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._rotation._z;
	exportObject << ',';

	// �X�P�[��
	exportObject << object->GetInfo()->_sprite->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_sprite->property._transform._scale._z;
	exportObject << ',';

	// �}�e���A��ID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// ���f���^�C�v
	exportObject << object->GetInfo()->_sprite->GetName();
	exportObject << ',';

	// �J���[
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

	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteFbxModel(std::ofstream& exportObject, FbxModelObject* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// �ʒu
	exportObject << object->GetInfo()->_fbx->property._transform._translation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._translation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._translation._z;
	exportObject << ',';

	// ��]
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._rotation._z;
	exportObject << ',';

	// �X�P�[��
	exportObject << object->GetInfo()->_fbx->property._transform._scale._x;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._scale._y;
	exportObject << ',';
	exportObject << object->GetInfo()->_fbx->property._transform._scale._z;
	exportObject << ',';

	// �}�e���A��ID
	exportObject << object->GetInfo()->_materialID;
	exportObject << ',';

	// ���f���^�C�v
	// �Ƃ肠����FBX�������f���̃p�X��f���o���悤�ɂ���
	exportObject << object->GetInfo()->_fbx->GetModelPath(); 
	exportObject << ',';

	// �J���[
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

	exportObject << std::endl;
}

//
void WorldWriter::WriteMaterial(std::ofstream& exportObject, Material* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}

	exportObject << std::endl;
}

//
void WorldWriter::WriteTexture(std::ofstream& exportObject, Texture* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}

	exportObject << std::endl;
}

//
void WorldWriter::WriteCamera(std::ofstream& exportObject, aetherClass::ViewCamera* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// �ʒu
	exportObject << object->property._translation._x;
	exportObject << ',';
	exportObject << object->property._translation._y;
	exportObject << ',';
	exportObject << object->property._translation._z;
	exportObject << ',';

	// ��]
	exportObject << object->property._rotation._x;
	exportObject << ',';
	exportObject << object->property._rotation._y;
	exportObject << ',';
	exportObject << object->property._rotation._z;
	exportObject << std::endl;
	return;
}

//
void WorldWriter::WriteLight(std::ofstream& exportObject, aetherClass::Light* object){
	if (!object) {
		exportObject << "none" << std::endl;
		return;
	}
	// �ʒu
	exportObject << object->property._translation._x;
	exportObject << ',';
	exportObject << object->property._translation._y;
	exportObject << ',';
	exportObject << object->property._translation._z;
	exportObject << std::endl;
	return;
}