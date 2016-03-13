/*
	SceneWindow‚Ìˆ—‚ğ‹Lq
*/
#ifndef _SCENEWINDOWVIEW_H
#define _SCENEWINDOWVIEW_H
#include <GameScene.h>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include "Pivot.h"
#include "PrimitiveObject.h"
class SceneWindowView :
	public aetherClass::GameScene
{
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
	static aetherClass::ViewCamera m_viewCamera;
};

#endif