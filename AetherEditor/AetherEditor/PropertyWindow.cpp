#define ErrorBox(x){MessageBox(NULL, x, L"Error", MB_OK);}
#include "PropertyWindow.h"


#include <Windows.h>
#include <iostream>
#include <codecvt>
#include <algorithm>
#include <cctype>
#include <Vector3.h>
#include <Color.h>
using namespace aetherClass;
namespace{
	const int kMaxSize = 3;
}
enum eButton{
	eName = 1000,
	ePosition,
	eRotation,
	eScale,
	eColor,
	eTexture,
	eMaterial,
	eDelete,
};

// vector削除関数
template<typename Type>
void Remove(std::vector<Type>& vector, unsigned int index)
{
	auto object = vector.begin() + index;
	if (*object)
	{
	//	delete (*object);
	}
	vector.erase(object);
}


PropertyWindow::PropertyWindow()
{
	m_prevSelectObject._objectType = eObjectType::eNull;
	m_prevSelectObject._number = NULL;
}


PropertyWindow::~PropertyWindow()
{
}
LRESULT CALLBACK  PropertyWindow::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (WorldObjectManager::GetCurrentSelectObject()._objectType != m_prevSelectObject._objectType||
		WorldObjectManager::GetCurrentSelectObject()._number != m_prevSelectObject._number)
	{

		m_prevSelectObject = WorldObjectManager::GetCurrentSelectObject();
		CheckWorldObject();
	}
	UINT flg = LOWORD(wParam);
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case eName:
			// 名前の設定
			SetName();
			return S_OK;

		case ePosition:
			SetPosition();
			return S_OK;

		case eRotation:
			SetRotation();
			return S_OK;

		case eScale:
			SetScale();
			return S_OK;

		case eColor:
			SetColor();
			return S_OK;

		case eTexture:
			MessageBox(NULL, L"テクスチャ", L"Error", MB_OK);
			return S_OK;

		case eMaterial:
			MessageBox(NULL, L"マテリアル", L"Error", MB_OK);
			return S_OK;

		case eDelete:
			DeleteObject();
			return S_OK;
		}
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return S_OK;
}

// 
void PropertyWindow::OnCreate(){
	CreateNameEdit();
	CreatePositionEdit();
	CreateRotationEdit();
	CreateScaleEdit();
	CreateColorEdit();
	CreateTextureEdit();
	CreateMaterialEdit();
	CreateDeleteEdit();

	// とりあえず、現在の状態をチェック
	CheckWorldObject();
	UpdateWindow(m_hWnd);
}

//
void PropertyWindow::DeleteObject(){
	auto current = m_prevSelectObject;
	// 削除したらNULLにしておく
	CurrentSelectObject currentObject;
	currentObject._objectType = eObjectType::eNull;
	currentObject._number = 0;
	WorldObjectManager::SetCurrentSelectObject(currentObject);
	CheckWorldObject();
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		
		Remove<PrimitiveObject*>(WorldObjectManager::GetPrimitive(), current._number);
		break;
	case eObjectType::eSprite:

		Remove<SpriteObject*>(WorldObjectManager::GetSprite(), current._number);
		break;

	case eObjectType::eFBX:

		Remove<FbxModelObject*>(WorldObjectManager::GetFbxModel(), current._number);
		break;
	default:
		break;
	}
	
	return;
}

//
void PropertyWindow::CheckWorldObject(){

	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:{
		// 名前
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string name = WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_name;
		std::wstring objectName = converter.from_bytes(name);
		SetWindowText(m_inputNameEdit, objectName.c_str());

		// テクスチャID
		const int textureID = WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputTextureEdit, std::to_wstring(textureID).c_str());

		//　マテリアルID
		const int materialID = WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputMaterialEdit, std::to_wstring(materialID).c_str());


		auto value = WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._transform;
		auto colorValue = WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._color;
		
		float positionArray[kMaxSize] = { value._translation._x, value._translation._y, value._translation._z };
		float rotationArray[kMaxSize] = { value._rotation._x, value._rotation._y, value._rotation._z };
		float scaleArray[kMaxSize] = { value._scale._x, value._scale._y, value._scale._z };
		float colorArray[kMaxSize] = { colorValue._red, colorValue._green, colorValue._blue };
		for (int i = 0; i < kMaxSize; ++i)
		{
			std::wstring position;
			position = std::to_wstring(positionArray[i]);
			SetWindowText(m_inputPositionEdit[i], position.c_str());

			std::wstring rotation;
			rotation = std::to_wstring(rotationArray[i]);
			SetWindowText(m_inputRotationEdit[i], rotation.c_str());

			std::wstring scale;
			scale = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputScaleEdit[i], scale.c_str());

			std::wstring color;
			color = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputColorEdit[i], color.c_str());
		}
	}
		break;


	case eObjectType::eSprite:{
		// 名前
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string name = WorldObjectManager::GetSprite()[current._number]->GetInfo()->_name;
		std::wstring objectName = converter.from_bytes(name);
		SetWindowText(m_inputNameEdit, objectName.c_str());

		// テクスチャID
		const int textureID = WorldObjectManager::GetSprite()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputTextureEdit, std::to_wstring(textureID).c_str());

		//　マテリアルID
		const int materialID = WorldObjectManager::GetSprite()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputMaterialEdit, std::to_wstring(materialID).c_str());


		auto value = WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._transform;
		auto colorValue = WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._color;

		float positionArray[kMaxSize] = { value._translation._x, value._translation._y, NULL };
		float rotationArray[kMaxSize] = { value._rotation._x, value._rotation._y, NULL };
		float scaleArray[kMaxSize] = { value._scale._x, value._scale._y, NULL };
		float colorArray[kMaxSize] = { colorValue._red, colorValue._green, colorValue._blue };
		for (int i = 0; i < kMaxSize; ++i)
		{
			std::wstring position;
			position = std::to_wstring(positionArray[i]);
			SetWindowText(m_inputPositionEdit[i], position.c_str());

			std::wstring rotation;
			rotation = std::to_wstring(rotationArray[i]);
			SetWindowText(m_inputRotationEdit[i], rotation.c_str());

			std::wstring scale;
			scale = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputScaleEdit[i], scale.c_str());

			std::wstring color;
			color = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputColorEdit[i], color.c_str());
		}
	}
		break;

	case eObjectType::eFBX:{
		// 名前
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string name = WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_name;
		std::wstring objectName = converter.from_bytes(name);
		SetWindowText(m_inputNameEdit, objectName.c_str());

		// テクスチャID
		const int textureID = WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputTextureEdit, std::to_wstring(textureID).c_str());

		//　マテリアルID
		const int materialID = WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_textureID;
		SetWindowText(m_inputMaterialEdit, std::to_wstring(materialID).c_str());


		auto value = WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_fbx->property._transform;
		
		float positionArray[kMaxSize] = { value._translation._x, value._translation._y, value._translation._z };
		float rotationArray[kMaxSize] = { value._rotation._x, value._rotation._y, value._rotation._z };
		float scaleArray[kMaxSize] = { value._scale._x, value._scale._y, value._scale._z };
		float colorArray[kMaxSize] = { NULL };
		for (int i = 0; i < kMaxSize; ++i)
		{
			std::wstring position;
			position = std::to_wstring(positionArray[i]);
			SetWindowText(m_inputPositionEdit[i], position.c_str());

			std::wstring rotation;
			rotation = std::to_wstring(rotationArray[i]);
			SetWindowText(m_inputRotationEdit[i], rotation.c_str());

			std::wstring scale;
			scale = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputScaleEdit[i], scale.c_str());

			std::wstring color;
			color = std::to_wstring(scaleArray[i]);
			SetWindowText(m_inputColorEdit[i], color.c_str());
		}
	}

		break;
	case eObjectType::eCamera:{
		SetWindowText(m_inputNameEdit, L"Camera");
		SetWindowText(m_inputTextureEdit, L"00");
		SetWindowText(m_inputMaterialEdit, L"00");
		auto position = WorldObjectManager::GetCameraValue();
		
		float positionArray[3] = { position._position._x, position._position._y, position._position._z };
		float rotationArray[3] = { position._rotation._x, position._rotation._y, position._rotation._z };
		for (int i = 0; i < 3; ++i)
		{
			std::wstring position;
			position = std::to_wstring(positionArray[i]);
			std::wstring rotation;
			rotation = std::to_wstring(rotationArray[i]);
			SetWindowText(m_inputPositionEdit[i], position.c_str());
			SetWindowText(m_inputRotationEdit[i], rotation.c_str());
			SetWindowText(m_inputScaleEdit[i], L"null");
			SetWindowText(m_inputColorEdit[i], L"null");
		}
	}
		break;

	case eObjectType::eLight:{
		SetWindowText(m_inputNameEdit, L"Light");
		SetWindowText(m_inputTextureEdit, L"00");
		SetWindowText(m_inputMaterialEdit, L"00");
		auto light = WorldObjectManager::GetLightValue();
		
		float positionArray[kMaxSize] = { light._x, light._y, light._z };
		for (int i = 0; i < 3; ++i)
		{
			std::wstring position;
			position = std::to_wstring(positionArray[i]);
			SetWindowText(m_inputPositionEdit[i], position.c_str());
			SetWindowText(m_inputRotationEdit[i], L"null");
			SetWindowText(m_inputScaleEdit[i], L"null");
			SetWindowText(m_inputColorEdit[i], L"null");
		}
	}
		break;

	case eObjectType::eNull:
		SetWindowText(m_inputNameEdit, L"Null");
		SetWindowText(m_inputTextureEdit, L"00");
		SetWindowText(m_inputMaterialEdit, L"00");

		for (int i = 0; i < 3; ++i)
		{
			SetWindowText(m_inputPositionEdit[i], L"null");
			SetWindowText(m_inputRotationEdit[i], L"null");
			SetWindowText(m_inputScaleEdit[i], L"null");
			SetWindowText(m_inputColorEdit[i], L"null");
		}
		break;
	}
	return;
}

//
void PropertyWindow::SetName(){
	TCHAR szBuf[256] = { NULL };
	// 入力されている文字の取得
	GetWindowText(m_inputNameEdit, szBuf, sizeof(szBuf) / sizeof(TCHAR));

	// コンバートオブジェクト
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
	std::string objectName = converter.to_bytes(szBuf);

	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_name = objectName;
		break;

	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[current._number]->GetInfo()->_name = objectName;
		break;

	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_name = objectName;
		break;

	default:
		break;
	}

	return;
}

//
void PropertyWindow::SetPosition(){
	TCHAR position[kMaxSize][256] = { NULL };
	float positionArrray[kMaxSize];
	// 入力されている文字の取得
	for (int i = 0; i < kMaxSize; ++i)
	{
		GetWindowText(m_inputPositionEdit[i], position[i], sizeof(position) / sizeof(TCHAR));
		// コンバートオブジェクト
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string objectPosition = converter.to_bytes(position[i]);

		if (std::any_of(objectPosition.cbegin(), objectPosition.cend(), std::isalpha))
		{
			ErrorBox(L"数値を入力してください");
			return;
		}

		positionArrray[i] = std::atof(objectPosition.c_str());
	}
	CameraValue camera;
	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._transform._translation = Vector3(positionArrray[0], positionArrray[1], positionArrray[2]);
		break;

	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._transform._translation = Vector3(positionArrray[0], positionArrray[1], 0);
		break;

	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_fbx->property._transform._translation = Vector3(positionArrray[0], positionArrray[1], positionArrray[2]);
		break;
	case eObjectType::eCamera:
		camera._rotation = WorldObjectManager::GetCameraValue()._rotation;
		camera._position = Vector3(positionArrray[0], positionArrray[1], positionArrray[2]);
		WorldObjectManager::RegisterCameraValue(camera);
		break;

	case eObjectType::eLight:
		WorldObjectManager::RegisterLightValue(Vector3(positionArrray[0], positionArrray[1], positionArrray[2]));
		break;
	default:
		break;
	}


	return;
}

//
void PropertyWindow::SetRotation(){
	TCHAR rotation[kMaxSize][256] = { NULL };
	float rotationArrray[kMaxSize];
	// 入力されている文字の取得
	for (int i = 0; i < kMaxSize; ++i)
	{
		GetWindowText(m_inputPositionEdit[i], rotation[i], sizeof(rotation) / sizeof(TCHAR));
		// コンバートオブジェクト
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string objectRotation = converter.to_bytes(rotation[i]);

		if (std::any_of(objectRotation.cbegin(), objectRotation.cend(), std::isalpha))
		{
			ErrorBox(L"数値を入力してください");
			return;
		}

		rotationArrray[i] = std::atof(objectRotation.c_str());
	}
	CameraValue camera;
	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._transform._rotation = Vector3(rotationArrray[0], rotationArrray[1], rotationArrray[2]);
		break;

	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._transform._rotation = Vector3(rotationArrray[0], rotationArrray[1], 0);
		break;

	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_fbx->property._transform._rotation = Vector3(rotationArrray[0], rotationArrray[1], rotationArrray[2]);
		break;
	case eObjectType::eCamera:
		camera._position= WorldObjectManager::GetCameraValue()._position;
		camera._rotation = Vector3(rotationArrray[0], rotationArrray[1], rotationArrray[2]);
		WorldObjectManager::RegisterCameraValue(camera);
		break;
	default:
		break;
	}

	return;
}

//
void PropertyWindow::SetScale(){
	TCHAR scale[kMaxSize][256] = { NULL };
	float scaleArrray[kMaxSize];
	// 入力されている文字の取得
	for (int i = 0; i < kMaxSize; ++i)
	{
		GetWindowText(m_inputPositionEdit[i], scale[i], sizeof(scale) / sizeof(TCHAR));
		// コンバートオブジェクト
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string objectScale= converter.to_bytes(scale[i]);

		if (std::any_of(objectScale.cbegin(), objectScale.cend(), std::isalpha))
		{
			ErrorBox(L"数値を入力してください");
			return;
		}

		scaleArrray[i] = std::atof(objectScale.c_str());
	}


	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._transform._scale = Vector3(scaleArrray[0], scaleArrray[1], scaleArrray[2]);
		break;

	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._transform._scale = Vector3(scaleArrray[0], scaleArrray[1], 0);
		break;

	case eObjectType::eFBX:
		WorldObjectManager::GetFbxModel()[current._number]->GetInfo()->_fbx->property._transform._scale = Vector3(scaleArrray[0], scaleArrray[1], scaleArrray[2]);
		
		break;
	default:
		break;
	}

	return;

}

//
void PropertyWindow::SetColor(){
	TCHAR color[kMaxSize][256] = { NULL };
	float colorArrray[kMaxSize];
	// 入力されている文字の取得
	for (int i = 0; i < kMaxSize; ++i)
	{
		GetWindowText(m_inputPositionEdit[i], color[i], sizeof(color) / sizeof(TCHAR));
		// コンバートオブジェクト
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
		std::string objectColor = converter.to_bytes(color[i]);

		if (std::any_of(objectColor.cbegin(), objectColor.cend(), std::isalpha))
		{
			ErrorBox(L"数値を入力してください");
			return;
		}

		colorArrray[i] = std::atof(objectColor.c_str());
	}


	auto current = m_prevSelectObject;
	switch (current._objectType)
	{
	case eObjectType::ePrimitive:
		WorldObjectManager::GetPrimitive()[current._number]->GetInfo()->_primitive->property._color = Color(colorArrray[0], colorArrray[1], colorArrray[2],1.0f);
		break;

	case eObjectType::eSprite:
		WorldObjectManager::GetSprite()[current._number]->GetInfo()->_sprite->property._color = Color(colorArrray[0], colorArrray[1], 0, 1.0f);
		break;
	default:
		break;
	}

	return;

}

//
void PropertyWindow::CreateNameEdit(){
	// name
	HWND m_hWndStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("NAME :"),
		WS_CHILD | SS_CENTER, 0, 0, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_hWndStatic, SW_SHOW);

	// name 入力
	m_inputNameEdit = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 60, 0, 150, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputNameEdit, SW_SHOW);

	// name 適用 ボタン
	HWND m_button = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 210, 0, 90, 20, m_hWnd, (HMENU)eName, GetModuleHandle(NULL), NULL);
	ShowWindow(m_button, SW_SHOW);

	return;
}

//
void PropertyWindow::CreatePositionEdit(){
	// position
	HWND position = CreateWindowEx(0, TEXT("STATIC"), TEXT("POSITION"),
		WS_CHILD | SS_CENTER, 0, 25, 300, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(position, SW_SHOW);

	// X
	HWND XStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("X :"),
		WS_CHILD, 0, 45, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(XStatic, SW_SHOW);

	// Xの入力
	m_inputPositionEdit[0] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 20, 45, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputPositionEdit[0], SW_SHOW);

	// Y
	HWND YStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Y :"),
		WS_CHILD, 80, 45, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(YStatic, SW_SHOW);

	// Yの入力
	m_inputPositionEdit[1] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 100, 45, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputPositionEdit[1], SW_SHOW);

	// Z
	HWND ZStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Z :"),
		WS_CHILD, 160, 45, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(ZStatic, SW_SHOW);

	// Zの入力
	m_inputPositionEdit[2] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 180, 45, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputPositionEdit[2], SW_SHOW);


	// ボタン
	HWND positionButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 240, 45, 60, 20, m_hWnd, (HMENU)ePosition, GetModuleHandle(NULL), NULL);
	ShowWindow(positionButton, SW_SHOW);

	return;
}

//
void PropertyWindow::CreateRotationEdit(){
	// rotation
	HWND rotation = CreateWindowEx(0, TEXT("STATIC"), TEXT("ROTATION"),
		WS_CHILD | SS_CENTER, 0, 70, 300, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(rotation, SW_SHOW);

	// X
	HWND XStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("X :"),
		WS_CHILD, 0, 90, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(XStatic, SW_SHOW);

	// Xの入力
	m_inputRotationEdit[0] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 20, 90, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputRotationEdit[0], SW_SHOW);

	// Y
	HWND YStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Y :"),
		WS_CHILD, 80, 90, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(YStatic, SW_SHOW);

	// Yの入力
	m_inputRotationEdit[1] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 100, 90, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputRotationEdit[1], SW_SHOW);

	// Z
	HWND ZStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Z :"),
		WS_CHILD, 160, 90, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(ZStatic, SW_SHOW);

	// Zの入力
	m_inputRotationEdit[2] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 180, 90, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputRotationEdit[2], SW_SHOW);


	// ボタン
	HWND rotationButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 240, 90, 60, 20, m_hWnd, (HMENU)eRotation, GetModuleHandle(NULL), NULL);
	ShowWindow(rotationButton, SW_SHOW);

	return;
}

//
void PropertyWindow::CreateScaleEdit(){
	// scale
	HWND scale = CreateWindowEx(0, TEXT("STATIC"), TEXT("SCALE"),
		WS_CHILD | SS_CENTER, 0, 115, 300, 20, m_hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);
	ShowWindow(scale, SW_SHOW);

	// X
	HWND XStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("X :"),
		WS_CHILD, 0, 135, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(XStatic, SW_SHOW);

	// Xの入力
	m_inputScaleEdit[0] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 20, 135, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputScaleEdit[0], SW_SHOW);

	// Y
	HWND YStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Y :"),
		WS_CHILD, 80, 135, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(YStatic, SW_SHOW);

	// Yの入力
	m_inputScaleEdit[1] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 100, 135, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputScaleEdit[1], SW_SHOW);

	// Z
	HWND ZStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Z :"),
		WS_CHILD, 160, 135, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(ZStatic, SW_SHOW);

	// Zの入力
	m_inputScaleEdit[2] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 180, 135, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputScaleEdit[2], SW_SHOW);


	// ボタン
	HWND scaleButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 240, 135, 60, 20, m_hWnd, (HMENU)eScale, GetModuleHandle(NULL), NULL);
	ShowWindow(scaleButton, SW_SHOW);

	return;
}

void PropertyWindow::CreateColorEdit(){
	// color
	HWND color = CreateWindowEx(0, TEXT("STATIC"), TEXT("COLOR"),
		WS_CHILD | SS_CENTER, 0, 160, 300, 20, m_hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);
	ShowWindow(color, SW_SHOW);

	// red
	HWND redStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("R :"),
		WS_CHILD, 0, 180, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(redStatic, SW_SHOW);

	// redの入力
	m_inputColorEdit[0] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 20, 180, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputColorEdit[0], SW_SHOW);

	// green
	HWND greenStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("G:"),
		WS_CHILD, 80, 180, 25, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(greenStatic, SW_SHOW);

	// greenの入力
	m_inputColorEdit[1] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 100, 180, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputColorEdit[1], SW_SHOW);

	// blue
	HWND blueStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("B:"),
		WS_CHILD, 160, 180, 25, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(blueStatic, SW_SHOW);

	// blueの入力
	m_inputColorEdit[2] = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD, 180, 180, 60, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputColorEdit[2], SW_SHOW);


	// ボタン
	HWND scaleButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 240, 180, 60, 20, m_hWnd, (HMENU)eColor, GetModuleHandle(NULL), NULL);
	ShowWindow(scaleButton, SW_SHOW);
}

//
void PropertyWindow::CreateTextureEdit(){

	// texture
	HWND texture = CreateWindowEx(0, TEXT("STATIC"), TEXT("TEXTURE_ID :"),
		WS_CHILD | SS_RIGHT, 0, 205, 150, 20, m_hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);
	ShowWindow(texture, SW_SHOW);

	m_inputTextureEdit = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD | ES_RIGHT, 152.5, 205, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputTextureEdit, SW_SHOW);

	// ボタン
	HWND textureButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 175, 205, 130, 20, m_hWnd, (HMENU)eTexture, GetModuleHandle(NULL), NULL);
	ShowWindow(textureButton, SW_SHOW);
	return;
}

//
void PropertyWindow::CreateMaterialEdit(){
	// material
	HWND material = CreateWindowEx(0, TEXT("STATIC"), TEXT("MATERIAL_ID :"),
		WS_CHILD | SS_RIGHT, 0, 230, 150, 20, m_hWnd, (HMENU)101, GetModuleHandle(NULL), NULL);
	ShowWindow(material, SW_SHOW);

	m_inputMaterialEdit = CreateWindowEx(0, TEXT("EDIT"), TEXT(""),
		WS_CHILD | ES_RIGHT, 152.5, 230, 20, 20, m_hWnd, (HMENU)100, GetModuleHandle(NULL), NULL);
	ShowWindow(m_inputMaterialEdit, SW_SHOW);

	// ボタン
	HWND textureButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Apply"),
		WS_CHILD, 175, 230, 130, 20, m_hWnd, (HMENU)eMaterial, GetModuleHandle(NULL), NULL);
	ShowWindow(textureButton, SW_SHOW);
	return;
}

//
void PropertyWindow::CreateDeleteEdit(){
	// ボタン
	HWND deleteButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Remove Object"),
		WS_CHILD, 0, 255, 300, 40, m_hWnd, (HMENU)eDelete, GetModuleHandle(NULL), NULL);
	ShowWindow(deleteButton, SW_SHOW);
	return;
}
