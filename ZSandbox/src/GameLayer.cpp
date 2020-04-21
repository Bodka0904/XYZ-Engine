#include "GameLayer.h"

#include <random>

#include "imgui/imgui.h"
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>

GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::RigidBody2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::GridPosition>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::ParticleEffect2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::Renderable2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::SpriteAnimation>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::InterpolatedMovement>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::LayerMask>();

	m_PhysicsSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::PhysicsSystem>();
	m_PhysicsSystem->CreateGrid(21, 21, 1);
	m_ParticleSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::ParticleSystem2D>();
	m_SpriteSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::SpriteSystem>();

	XYZ::Renderer::Init();


	m_Camera = std::make_shared<XYZ::OrthoCamera>(0, 20, 0, 20);

	m_Material = XYZ::Material::Create(XYZ::Shader::Create("TextureShader", "../XYZ_Engine/Assets/Shaders/DefaultShader.glsl"));
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/bomb.png"), 0);
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/wall.png"), 1);
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Repeat, "../XYZ_Engine/Assets/Textures/background.png"), 2);


	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/player_sprite.png"), 3);
	m_Material->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
	m_Material->SetFlags(XYZ::RenderFlags::TransparentFlag);

	

	////m_Players.resize(2);
	//m_Players[0].Init(m_Camera, m_Material, glm::vec3(5, 5, 0), m_Bombs);
	//m_Players[1].Init(m_Camera, m_Material, glm::vec3(2, 2, 0.5f), m_Bombs);
	//
	//m_Players[0].UseControlsMode(0);
	//m_Players[1].UseControlsMode(1);

	
	InitBackgroundParticles();
}

void GameLayer::OnDetach()
{
	XYZ::Renderer2D::Shutdown();
	delete m_SubEffect;
}

void GameLayer::OnUpdate(float dt)
{
	XYZ::RenderCommand::Clear();
	XYZ::RenderCommand::SetClearColor(glm::vec4(0.2, 0.2, 0.5, 1));
	XYZ::Renderer2D::BeginScene(*m_Camera);
	XYZ::Renderer2D::Flush();
	XYZ::Renderer2D::EndScene();

	m_Menu.Update();

	if (m_Menu.GameStart())
		RestartGame();


	if (m_Menu.GameRunning())
	{
		// Start game loop
		
		m_PhysicsSystem->Update(dt);
		m_SpriteSystem->Update(dt);

		m_Material->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
		for (size_t i = 0; i < m_Bombs.size(); ++i)
		{
			if (m_Bombs[i].Update(dt, *m_Camera, m_DamagedCells))
			{
				auto cellsToDamage = m_Bombs[i].InflictDamage();
				m_DamagedCells.insert(std::end(m_DamagedCells), std::begin(cellsToDamage), std::end(cellsToDamage));

				m_Bombs[i].Destroy();
				m_Bombs[i] = m_Bombs[m_Bombs.size() - 1];
				m_Bombs.pop_back();
			}
		}

		for (int i = 0; i < m_Players.size(); i++)
		{
			m_Players[i].Update(m_Bombs, m_DamagedCells);
			if (m_Players[i].IsDead())
				m_Players.erase(m_Players.begin() + i);

		}
		if (m_Players.size() == 1)
			m_Menu.SetEndGame();

		m_DamagedCells.clear();

	}
	else
	{
		m_ParticleMaterial->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
		m_ParticleSystem->Update(dt);
	}
}

void GameLayer::RestartGame()
{	
	for (size_t i = 0; i < m_Players.size(); ++i)
		m_Players[i].Destroy();
	m_Players.clear();

	for (size_t i = 0; i < m_Bombs.size(); ++i)
		m_Bombs[i].Destroy();
	m_Bombs.clear();

	m_Map.Init(20, m_Material);
	m_Map.Generate(40);

	m_Players.resize(2);
	m_Players[0].Init(m_Camera, m_Material, glm::vec3(5, 5, 0), m_Bombs);
	m_Players[1].Init(m_Camera, m_Material, glm::vec3(2, 2, 0.5f), m_Bombs);

	m_Players[0].UseControlsMode(0);
	m_Players[1].UseControlsMode(1);
}

void GameLayer::InitBackgroundParticles()
{
	m_ParticleEntity = XYZ::ECSManager::Get()->CreateEntity();
	m_ParticleMaterial = XYZ::Material::Create(XYZ::Shader::Create("ParticleShader", "../XYZ_Engine/Assets/Shaders/Particle/ParticleShader.glsl"));
	m_ParticleMaterial->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/bubbles.png"));
	m_ParticleMaterial->SetFlags(XYZ::RenderFlags::InstancedFlag);


	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, XYZ::ParticleEffect2D(1000, XYZ::Material::Create(XYZ::Shader::Create("../XYZ_Engine/Assets/Shaders/Particle/ParticleComputeShader.glsl")), m_ParticleMaterial));
	auto effect = XYZ::ECSManager::Get()->GetComponent<XYZ::ParticleEffect2D>(m_ParticleEntity);


	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<> dist(-1, 1);

	std::vector<XYZ::ParticleProps2D> particles;
	particles.resize(1000);


	float distance = 20.0f / particles.size();
	for (size_t i = 0; i < particles.size(); ++i)
	{
		particles[i].position = glm::vec2(distance * i, 20);
		particles[i].sizeBegin = 2.0f;
		particles[i].sizeEnd = 0.0f;
		particles[i].colorEnd = glm::vec4(1, 0, 1, 0);
		particles[i].colorBegin = glm::vec4(0, 1, 1, 1);
		particles[i].velocity = glm::vec2(dist(rng), -0.2f);
		particles[i].lifeTime = fabs(dist(rng)) * 10;
		particles[i].rotation = dist(rng) * 50;
	}

	m_SubEffect = new XYZ::ParticleSubEffect2D(effect, particles);
	m_SubEffect->emitter = glm::vec2(0, 0);
}
