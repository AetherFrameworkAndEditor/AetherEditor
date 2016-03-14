/*
	SceneWindowÇÃèàóùÇãLèq
*/
#ifndef _SCENEWINDOWVIEW_H
#define _SCENEWINDOWVIEW_H
#include <GameScene.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include "Pivot.h"
#include "PrimitiveObject.h"
#include"Line.h"
#include"Light.h"
#include"WorldObjectManager.h"
class SceneWindowView :
	public aetherClass::GameScene
{
	const int kCameraRoteVal = 3;
	const int kCameraTransVal = 8;
	const int kCameraZoomVal = 100;
public:
	SceneWindowView();
	~SceneWindowView();
	bool Initialize()override;
	void Finalize()override;
	void Render()override;
	void UIRender()override;
	bool Updater()override;

	std::unique_ptr<aetherClass::ShaderBase>m_colorShader;

	static aetherClass::ViewCamera& GetSceneWindowCamera();
private:
	bool PlayingProcess();
	bool NotPlayingProcess();

	void UpdateCamera();
	void TransformInitialize(aetherClass::Transform transform);
	void SceneWindowView::LockMouseCursor(HWND hWnd);
	void UpdateCurrentObject();
	CurrentSelectObject SelectObject(aetherClass::RayVector);
private:
	static aetherClass::ViewCamera m_viewCamera;
	aetherClass::ViewCamera m_gameCamera;
	aetherClass::Light m_gamelight;
	aetherClass::DirectXEntity m_directX;

	bool m_IsPlay;
	bool m_controllCamera;
	int m_cursorShowFlg;
	aetherClass::Transform m_objectTransform;
};

#endif