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

}


void GameWindowView::Render(){

}

void GameWindowView::UIRender(){

}

bool GameWindowView::Updater(){
	return true;
}