#include "GameWindowView.h"

using namespace aetherClass;

GameWindowView::GameWindowView():
GameScene("Game", GetManager())
{
}


GameWindowView::~GameWindowView()
{
}


bool GameWindowView::Initialize(){

	return true;
}


void GameWindowView::Finalize(){

	return;
}


void GameWindowView::Render(){

	return;
}

void GameWindowView::UIRender(){

	return;
}

bool GameWindowView::Updater(){

	return true;
}