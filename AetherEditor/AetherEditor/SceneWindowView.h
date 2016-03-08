/*
	SceneWindow‚Ìˆ—‚ğ‹Lq
*/
#ifndef _SCENEWINDOWVIEW_H
#define _SCENEWINDOWVIEW_H
#include <GameScene.h>
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
};

#endif