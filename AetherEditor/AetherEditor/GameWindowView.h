/*
	GameWindow‚Ìˆ—‚ğ‹Lq
*/
#ifndef _GAMEWINDOWVIEW_H
#define _GAMEWINDOWVIEW_H
#include <GameScene.h>
#include<memory>
#include"PixelShader.h"
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

private:
	std::unique_ptr<aetherClass::ShaderBase>m_colorShader;
	bool m_IsPlay;
	std::shared_ptr<aetherClass::ViewCamera>m_gameCamera;
};

#endif