/*
	GameWindow‚Ìˆ—‚ğ‹Lq
*/
#ifndef _GAMEWINDOWVIEW_H
#define _GAMEWINDOWVIEW_H
#include <GameScene.h>
class GameWindowView :
	public aetherClass::GameScene
{
public:
	GameWindowView();
	~GameWindowView();
	bool Initialize()override;
	void Finalize()override;
	void Render()override;
	void UIRender()override;
	bool Updater()override;
};

#endif