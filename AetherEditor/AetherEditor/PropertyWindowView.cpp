#include "PropertyWindowView.h"

using namespace aetherClass;
PropertyWindowView::PropertyWindowView():
GameScene("Property", GetManager())
{
}


PropertyWindowView::~PropertyWindowView()
{
}

bool PropertyWindowView::Initialize(){

	return true;
}


void PropertyWindowView::Finalize(){

}


void PropertyWindowView::Render(){

}


void PropertyWindowView::UIRender(){

}


bool PropertyWindowView::Updater(){
	return true;
}