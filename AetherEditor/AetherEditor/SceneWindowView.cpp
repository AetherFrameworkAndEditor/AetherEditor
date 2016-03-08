#include "SceneWindowView.h"


SceneWindowView::SceneWindowView():
GameScene("Scene", GetManager())
{
}


SceneWindowView::~SceneWindowView()
{
}

bool SceneWindowView::Initialize(){
	
	return true;
}

void SceneWindowView::Finalize(){

}

void SceneWindowView::Render(){

}

void SceneWindowView::UIRender(){

}

bool SceneWindowView::Updater(){
	return true;
}