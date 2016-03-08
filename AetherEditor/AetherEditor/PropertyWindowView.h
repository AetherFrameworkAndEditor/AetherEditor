/*
	PropertyWindow‚Ìˆ—‚ğ‹Lq
*/
#ifndef _PROPERTYWINDOWVIEW_H
#define _PROPERTYWINDOWVIEW_H
#include <GameScene.h>
class PropertyWindowView :
	public aetherClass::GameScene
{
public:
	PropertyWindowView();
	~PropertyWindowView();
	bool Initialize()override;
	void Finalize()override;
	void Render()override;
	void UIRender()override;
	bool Updater()override;

};

#endif