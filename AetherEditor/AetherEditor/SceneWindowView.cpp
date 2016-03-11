#include "SceneWindowView.h"
#include <PixelShader.h>
#include <Physics.h>
#include <Cube.h>
#include <GameController.h>
#include <GameClock.h>
#include "Pivot3D.h"
using namespace aetherClass;
using namespace aetherFunction;
SceneWindowView::SceneWindowView():
GameScene("Scene", GetManager())
{
}


SceneWindowView::~SceneWindowView()
{
}


//
bool SceneWindowView::Initialize(){
	// シェーダーの初期化用
	ShaderDesc shaderDesc;
	bool result = false;
	if (!m_primitiveObject)
	{
		m_primitiveObject = std::make_unique<PrimitiveObject>();
		result = m_primitiveObject->Create(new Cube,&m_viewCamera);
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

	if (!m_testPivot)
	{
		m_testPivot = std::make_unique<Pivot3D>();
		m_testPivot->Initialize(&m_viewCamera);
	}
	
	m_viewCamera.property._translation._z -= 10;
	m_testPivot->SetScale(0.2);
	return true;
}

//
void SceneWindowView::Finalize(){

	if (m_testPivot)
	{
		m_testPivot.release();
		m_testPivot = nullptr;
	}

	if (m_primitiveObject)
	{
		m_primitiveObject.release();
	}

	if (m_colorShader)
	{
		m_colorShader->Finalize();
		m_colorShader.release();
	}
	return;
}

//
bool SceneWindowView::Updater(){
	m_viewCamera.Controller();
	if (GameController::GetKey().IsKeyDown(DIK_UP))
	{
		Vector3 move(1, 0, 0);
		m_testPivot->MoveDirection(move*GameClock::GetDeltaTime());

	}
	if (GameController::GetMouse().IsLeftButtonTrigger())
	{
		RayVector ray = GameController::GetMouse().Intersection(m_viewCamera);
		if (RaySphereIntersect(*m_primitiveObject->GetCollider(), ray._origin,ray._direction))
		{
			m_primitiveObject->ChangePivotState();
		}
	}
	
	return true;
}

void SceneWindowView::Render(){
	m_viewCamera.Render();

	//m_primitiveObject->Render(m_colorShader.get());

	m_testPivot->Render(m_colorShader.get());
	return;
}

void SceneWindowView::UIRender(){

	return;
}