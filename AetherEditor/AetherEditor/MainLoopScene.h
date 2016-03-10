/*
	ÀÛ‚ÌƒƒCƒ“ƒ‹[ƒv‚É‚È‚Á‚Ä‚à‚ç‚¤‚Æ‚±
*/
#ifndef _MAINLOOPSCEME_H
#define _MAINLOOPSCEME_H
#include <GameScene.h>
#include <DirectXEntity.h>
#include <vector>
class MainLoopScene :public aetherClass::GameScene
{
public:
	MainLoopScene();
	~MainLoopScene();
	bool Initialize()override;
	void Finalize()override;
	void Render()override;
	void UIRender()override;
	bool Updater()override;
private:
	std::vector<aetherClass::GameScene*> m_viewWindow;
	static std::wstring m_windowNameArray[];
};

#endif