#include "SceneWindowView.h"
#include <PixelShader.h>

using namespace aetherClass;
SceneWindowView::SceneWindowView():
GameScene("Scene", GetManager())
{
}


SceneWindowView::~SceneWindowView()
{
}

bool SceneWindowView::Initialize(){
	// シェーダーの初期化用
	ShaderDesc shaderDesc;
	bool result = false;
	if (!m_pivot)
	{
		m_pivot = std::make_shared<Pivot>();
		result = m_pivot->Initialize(&m_viewCamera);
		if (!result)
		{
			return false;
		}
	}

	if (!m_colorShader)
	{
		m_colorShader = std::make_unique<PixelShader>();

		shaderDesc._vertex._entryName = "vs_main";
		shaderDesc._pixel._entryName = "ps_main";

		shaderDesc._vertex._srcFile = L"Shader/VertexShaderBase.hlsl";
		shaderDesc._pixel._srcFile = L"Shader/BasicColor.ps";

		m_colorShader->Initialize(shaderDesc, ShaderType::eVertex | ShaderType::ePixel);
	}
	
	return true;
}

void SceneWindowView::Finalize(){

	if (m_pivot)
	{
		m_pivot->Finalize();
		m_pivot.reset();
	}

	if (m_colorShader)
	{
		m_colorShader->Finalize();
		m_colorShader.release();
	}
	return;
}

void SceneWindowView::Render(){
	m_viewCamera.Render();
	m_pivot->Render(m_colorShader.get());

	return;
}

void SceneWindowView::UIRender(){

}

bool SceneWindowView::Updater(){


	m_viewCamera.Controller();
	return true;
}