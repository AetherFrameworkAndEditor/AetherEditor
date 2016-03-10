/*
	SceneWindow�̏������L�q
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

private:
	std::unique_ptr<PrimitiveObject> m_primitiveObject;
	std::unique_ptr<aetherClass::ShaderBase> m_colorShader;
	std::unique_ptr<Pivot> m_testPivot;
	aetherClass::ViewCamera m_viewCamera;
};

#endif