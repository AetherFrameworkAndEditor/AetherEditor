#include "WorldWriter.h"


WorldWriter::WorldWriter()
{
}


WorldWriter::~WorldWriter()
{
}

//
void WorldWriter::WritePrimitive(std::ofstream& exportObject, PrimitiveObject* primitive){
	exportObject << "primitive" << std::endl;
	return;
}

//
void WorldWriter::WriteSprite(std::ofstream& exportObject, SpriteObject* sprite){
	exportObject << "sprite" << std::endl;
	return;
}

//
void WorldWriter::WriteFbxModel(std::ofstream& exportObject, FbxModelObject* fbx){
	exportObject << "fbx" << std::endl;
	return;
}

//
void WorldWriter::WriteCamera(std::ofstream& exportObject, aetherClass::ViewCamera* camera){
	exportObject << "camera" << std::endl;
	return;
}

//
void WorldWriter::WriteLight(std::ofstream& exportObject, aetherClass::Light* light){
	exportObject << "light" << std::endl;
	return;
}