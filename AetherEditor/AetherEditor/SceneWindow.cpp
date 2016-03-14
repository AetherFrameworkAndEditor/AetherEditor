#include "SceneWindow.h"
#include"GameController.h"
#include <shobjidl.h> 
#include"WorldObjectManager.h"
#include"SceneWindowView.h"
#include<codecvt>


//modelIncluder
#include"Point.h"
#include"Line.h"
#include"Circle.h"
#include"Triangle.h"
#include"Rectangle3D.h"
#include"Cube.h"
#include"Cylinder.h"
#include"Sphere.h"
#include"Text.h"
#include"Rectangle2D.h"

using namespace aetherClass;
SceneWindow::SceneWindow()
{
}


SceneWindow::~SceneWindow()
{
}

enum eMenuItem
{
	eProject = 0,
	eImport,eExport,eReset,eResetCamera,eQuit,
	ePrimitive = 10,
	ePoint,eLine,eTriangle,eRectangle,eCircle,eCube,eCylinder,eSphere,eFbx,
	eUI = 20,
	eRectangle2D,eText,
	eSound = 30

};



LRESULT CALLBACK SceneWindow::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	UINT flg = LOWORD(wParam);
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (flg == WA_ACTIVE || flg == WA_CLICKACTIVE){
			GameController::GetKey().ChangeActiveWindow(m_hWnd);
			GameController::GetMouse().ChangeActiveWindow(m_hWnd);
		}
		break;
	case WM_COMMAND: 
	{
		int nId = LOWORD(wParam);
		UpdateCommand(nId);
		break;
	}
	case WM_CLOSE:
	{
		int result = MessageBox(NULL, L"保存はしましたか？", L"終了", MB_OKCANCEL | MB_ICONINFORMATION);
		if (result == 2)return S_OK;
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return S_OK;
}

void SceneWindow::OnCreate(){
	MENUITEMINFO mii;

	HMENU hMenu = CreateMenu();

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_SUBMENU | MIIM_TYPE;
	mii.fType = MFT_STRING;

	HMENU projectMenu = CreateMenu();
	mii.hSubMenu = projectMenu;
	mii.dwTypeData = L"Project";
	InsertMenuItem(hMenu, 0, FALSE, &mii);
	
	HMENU createMenu = CreateMenu();
	mii.hSubMenu = createMenu;
	mii.dwTypeData = L"Create";
	InsertMenuItem(hMenu, 1, FALSE, &mii);

	HMENU viewMenu = CreateMenu();
	mii.hSubMenu = viewMenu;
	mii.dwTypeData = L"View";
	InsertMenuItem(hMenu, 2, FALSE, &mii);


	HMENU primitiveMenu = CreateMenu();
	mii.hSubMenu = primitiveMenu;
	mii.dwTypeData = L"Primitive";
	InsertMenuItem(createMenu, 0, FALSE, &mii);

	HMENU uiMenu = CreateMenu();
	mii.hSubMenu = uiMenu;
	mii.dwTypeData = L"UI";
	InsertMenuItem(createMenu, 1, FALSE, &mii);


	//subMenuList
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.hSubMenu = NULL;
	
	mii.wID = eFbx;
	mii.dwTypeData = L"FbxImport";
	InsertMenuItem(createMenu, 0, FALSE, &mii);
	
	mii.wID = ePoint;
	mii.dwTypeData = L"Point";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eLine;
	mii.dwTypeData = L"Line";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eTriangle;
	mii.dwTypeData = L"Triangle";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eRectangle;
	mii.dwTypeData = L"Rectangle";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eCircle;
	mii.dwTypeData = L"Circle";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eCube;
	mii.dwTypeData = L"Cube";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eCylinder;
	mii.dwTypeData = L"Cylinder";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);
	mii.wID = eSphere;
	mii.dwTypeData = L"Sphere";
	InsertMenuItem(primitiveMenu, 0, FALSE, &mii);

	mii.wID = eRectangle2D;
	mii.dwTypeData = L"Rectangle";
	InsertMenuItem(uiMenu, 0, FALSE, &mii);
	mii.wID = eText;
	mii.dwTypeData = L"Text";
	InsertMenuItem(uiMenu, 0, FALSE, &mii);
	mii.wID = eSound;
	mii.dwTypeData = L"Sound";
	InsertMenuItem(createMenu, 1, FALSE, &mii);

	
	mii.wID = eImport;
	mii.dwTypeData = L"Import";
	InsertMenuItem(projectMenu, 0, FALSE, &mii);
	mii.wID = eExport;
	mii.dwTypeData = L"Export";
	InsertMenuItem(projectMenu, 0, FALSE, &mii);
	mii.wID = eReset;
	mii.dwTypeData = L"Reset";
	InsertMenuItem(projectMenu, 0, FALSE, &mii);
	mii.wID = eQuit;
	mii.dwTypeData = L"Quit";
	InsertMenuItem(projectMenu, 0, FALSE, &mii);

	mii.wID = eResetCamera;
	mii.dwTypeData = L"カメラを初期位置に戻す";
	InsertMenuItem(viewMenu, 0, FALSE, &mii);

	FontDesc fontDesc;
	fontDesc._fontSize = 30;
	fontDesc._charSet = SHIFTJIS_CHARSET;
	fontDesc._fontName = L"メイリオ";

	m_font = std::make_shared<Font>();
	m_font->Load(fontDesc);


	SetMenu(m_hWnd, hMenu);
}


std::string SceneWindow::OpenDialogBox(){
	HRESULT hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (FAILED(hResult)){
		return "";
	}

	IFileOpenDialog *pFileOpen;
	hResult = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
	if (FAILED(hResult)){
		return "";
	}
	
	hResult = pFileOpen->Show(NULL);
	if (FAILED(hResult)){
		return "";
	}
	
	IShellItem *pItem;
	hResult = pFileOpen->GetResult(&pItem);
	if (FAILED(hResult)){
		return "";
	}
	
	WCHAR* pszFilePath;
	hResult = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
	if (FAILED(hResult)){
		return "";
	}
	
	//*代入ぽじ*//
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>converter;
	std::string outputPath = converter.to_bytes(pszFilePath);

	CoTaskMemFree(pszFilePath);
	pItem->Release();
	pFileOpen->Release();
	CoUninitialize();

	return outputPath;
}


 std::wstring SaveDialog(HWND hWnd)
{
	static OPENFILENAME     ofn = { 0 };
	static TCHAR            szFile[MAX_PATH];
	static TCHAR			szFirst[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, szFirst);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = szFirst;       // 初期フォルダ位置
		ofn.lpstrFile = szFile;       // 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".aether");
		ofn.lpstrFilter = TEXT("AetherWorldData(*.aether)\0*.aether\0");
		ofn.lpstrTitle = TEXT("名前つけて保存");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

		GetSaveFileName(&ofn);
//		MessageBox(hWnd, szFile, TEXT("ファイル名を付けて保存"), MB_OK);
		
		std::wstring path = szFile;
	return path;
}


void SceneWindow::UpdateCommand(int nId){
	switch (nId)
	{
	case eImport:
	{
		std::string path = OpenDialogBox();
		if (path.empty()){
			return;
		}
		bool result = WorldObjectManager::Import(path);
		if (!result) MessageBox(NULL, L"読み込みに失敗しました", L"", MB_OK);
	}
		break;
	case eExport:
	{
		Vector2 position;
		Vector2 screen(200, 100);
		position._x = (::GetSystemMetrics(SM_CXSCREEN) - screen._x) / 2;
		position._y = (::GetSystemMetrics(SM_CYSCREEN) - screen._y) / 2;

		std::wstring path = SaveDialog(m_hWnd);
		if (path.empty()){
			return;
		}
		WorldObjectManager::Export(path);
	}
		break;
	case eReset:
	{
		int result = MessageBox(NULL, L"配置したオブジェクトをすべて破棄しますか？",L"リセット", MB_OKCANCEL);
		if (result == 2) return;
		WorldObjectManager::Reset();
	}
		break;

	case eResetCamera:
	{
		SceneWindowView::GetSceneWindowCamera().property._rotation = 0;
		SceneWindowView::GetSceneWindowCamera().property._translation = 0;
		SceneWindowView::GetSceneWindowCamera().property._lookUp = 0;
		SceneWindowView::GetSceneWindowCamera().property._lookAt = 0;

		SceneWindowView::GetSceneWindowCamera().property._translation._z = -10;
		SceneWindowView::GetSceneWindowCamera().property._lookUp._y = 1.0f;
		SceneWindowView::GetSceneWindowCamera().property._lookAt._z = 1.0f;
	}
	break;

	case eQuit:
	{
		int result = MessageBox(NULL, L"保存はしましたか？", L"終了", MB_OKCANCEL|MB_ICONINFORMATION);
		if (result == 2) return;
		PostQuitMessage(0);
	}
	break;

	case ePoint:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Point,&SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
		break;
	case eLine:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Line, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eTriangle:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Triangle, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eRectangle:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Rectangle3D, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eCircle:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Circle, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eCube:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Cube, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eCylinder:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Cylinder, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eSphere:
	{
		PrimitiveObject *obj = new PrimitiveObject;
		obj->Create(new Sphere(10, 10), &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddPrimitive(obj);
	}
	break;
	case eFbx:
	{
		std::string path = OpenDialogBox();
		FbxModelObject *obj = new FbxModelObject;
		obj->Create(path, &SceneWindowView::GetSceneWindowCamera());
		WorldObjectManager::AddFbxModel(obj);
	}
		break;
	case eRectangle2D:
	{
		SpriteObject *obj = new SpriteObject;
		obj->Create(new Rectangle2D);
		WorldObjectManager::AddSprite(obj);
	}
	break;
	case eText:
	{
		SpriteObject *obj = new SpriteObject;
		Text *text = new Text();
		text->SetFont(m_font.get());
		obj->Create(text);
		text->UpdateText(L"文字");
		WorldObjectManager::AddSprite(obj);
	}
	break;
	case eSound:
		break;

	}

}