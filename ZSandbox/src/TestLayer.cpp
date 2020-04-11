#include "TestLayer.h"

#include <random>




TestLayer::TestLayer()
	: Layer()
{
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	m_MouseButtonPress = XYZ::EventManager::Get().AddHandler(XYZ::EventType::MouseButtonPressed, std::bind(&TestLayer::OnMouseButtonPress, this, std::placeholders::_1));


	XYZ::ECSManager::Get()->RegisterComponent<XYZ::Transform2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::RigidBody2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::ParticleEffect2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::Renderable2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::SpriteAnimation>();


	m_PhysicsSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::PhysicsSystem>();
	m_ParticleSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::ParticleSystem2D>();
	m_SpriteSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::SpriteSystem>();

	XYZ::Renderer::Init();
	m_CameraController = std::make_shared<XYZ::OrthoCameraController>(XYZ::Input::GetWindowSize().first / XYZ::Input::GetWindowSize().second);

	m_Material = XYZ::Material::Create(XYZ::Shader::Create("TextureShader", "../XYZ_Engine/Assets/Shaders/DefaultShader.glsl"));
	

	m_Entity = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_Entity, new XYZ::Transform2D{});
	XYZ::ECSManager::Get()->AddComponent(m_Entity, new XYZ::RigidBody2D{});

	m_Material->Set("u_ViewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/player_sprite.png"));
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/animation.png"), 1);
	m_Material->SetFlags(XYZ::RenderFlags::TransparentFlag);
	//////////////////////////////////////////////////////////////////////////////////////

	
	m_ParticleEntity = XYZ::ECSManager::Get()->CreateEntity();


	m_ParticleMaterial = XYZ::Material::Create(XYZ::Shader::Create("ParticleShader","../XYZ_Engine/Assets/Shaders/Particle/ParticleShader.glsl"));
	m_ParticleMaterial->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/bubbles.png"));
	m_ParticleMaterial->SetFlags(XYZ::RenderFlags::InstancedFlag);

	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::ParticleEffect2D(10000,XYZ::Material::Create(XYZ::Shader::Create("../XYZ_Engine/Assets/Shaders/Particle/ParticleComputeShader.glsl")), m_ParticleMaterial));
	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::Transform2D{});
	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::RigidBody2D{});
	

	m_Body = &XYZ::ECSManager::Get()->GetComponent<XYZ::RigidBody2D>(m_ParticleEntity);
	m_Transform = &XYZ::ECSManager::Get()->GetComponent<XYZ::Transform2D>(m_ParticleEntity);
	m_Effect = &XYZ::ECSManager::Get()->GetComponent<XYZ::ParticleEffect2D>(m_ParticleEntity);


	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<> dist(-2, 2);

	std::vector<XYZ::ParticleProps2D> particles;
	particles.resize(5000);
	for (size_t i = 0; i < particles.size(); ++i)
	{
		particles[i].position = glm::vec2(0, 0);
		particles[i].sizeBegin = 0.2f;
		particles[i].sizeEnd = 0.0f;
		particles[i].colorEnd = glm::vec4(1, 0, 1, 0);
		particles[i].colorBegin = glm::vec4(0, 1, 1, 1);
		particles[i].velocity = glm::vec2(dist(rng), fabs(dist(rng)));
		particles[i].lifeTime = fabs(dist(rng));
		particles[i].rotation = dist(rng) * 50;
	}
	m_SubEffect = new XYZ::ParticleSubEffect2D(m_Effect, particles);

	std::vector<XYZ::ParticleProps2D> particles2;
	particles2.resize(5000);
	for (size_t i = 0; i < particles.size(); ++i)
	{
		particles2[i].position = glm::vec2(0, 0);
		particles2[i].sizeBegin = 0.2f;
		particles2[i].sizeEnd = 0.0f;
		particles2[i].colorEnd = glm::vec4(1, 0, 0, 0);
		particles2[i].colorBegin = glm::vec4(0, 0, 1, 1);
		particles2[i].velocity = glm::vec2(dist(rng), fabs(dist(rng)));
		particles2[i].lifeTime = fabs(dist(rng));
		particles2[i].rotation = dist(rng) * 50;
	}
	m_SubEffect2 = new XYZ::ParticleSubEffect2D(m_Effect, particles2);	
	for (int i = 0; i < 10; ++i)
	{
		float posZ = i / 10.0f;
		XYZ::Entity entity = XYZ::ECSManager::Get()->CreateEntity();
		XYZ::ECSManager::Get()->AddComponent(entity, new XYZ::Renderable2D{
		m_Material,
		glm::vec4(1),
		glm::vec4(0.0f, 0.0f, 1.0f / 6.0f, 1.0f),
		glm::vec3(i,0,0.7),
		glm::vec2(1),
		0.0f,
		true,
		1
		});
	}

	m_SpriteEntity = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::Renderable2D{
		m_Material,
		glm::vec4(1),
		glm::vec4(0.0f, 0.0f, 1.0f / 8.0f, 1.0f / 3.0f),
		glm::vec3(0,0,1),
		glm::vec2(1)});

	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::SpriteAnimation(3, 8, 240, 90));
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::RigidBody2D{});
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::Transform2D{});

	
	
	m_SpriteBody = &XYZ::ECSManager::Get()->GetComponent<XYZ::RigidBody2D>(m_SpriteEntity);
	m_SpriteTransform = &XYZ::ECSManager::Get()->GetComponent<XYZ::Transform2D>(m_SpriteEntity);
	m_Sprite = &XYZ::ECSManager::Get()->GetComponent<XYZ::Renderable2D>(m_SpriteEntity);
	m_SpriteAnimation = &XYZ::ECSManager::Get()->GetComponent<XYZ::SpriteAnimation>(m_SpriteEntity);
	m_SpriteAnimation->SetFrameInterval(0, 5, 0.5f);

	m_AnimController.AddAnimation("idle", XYZ::Animation{
		std::pair<int,int>(16,17),
		0.3f
		});
	m_AnimController.AddAnimation("walkright", XYZ::Animation{
		std::pair<int,int>(8,9),
		0.3f
		});
	m_AnimController.AddAnimation("walkleft", XYZ::Animation{
		std::pair<int,int>(10,11),
		0.3f
		});
	m_AnimController.AddAnimation("walkup", XYZ::Animation{
		std::pair<int,int>(22,23),
		0.3f
		});
	m_AnimController.AddAnimation("walkdown", XYZ::Animation{
		std::pair<int,int>(20,21),
		0.3f
		});	
}

void TestLayer::OnDetach()
{
	delete m_SubEffect;
	delete m_SubEffect2;

	XYZ::EventManager::Get().RemoveHandler(XYZ::EventType::MouseButtonPressed, m_MouseButtonPress);
}

void TestLayer::OnUpdate(float dt)
{
	m_PhysicsSystem->Update(dt);
	
	m_SpriteSystem->Update(dt);
	m_CameraController->OnUpdate(dt);
	m_Material->Set("u_ViewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());
	m_ParticleMaterial->Set("u_ViewProjection", m_CameraController->GetCamera().GetViewProjectionMatrix());

	XYZ::RenderCommand::Clear();
	XYZ::RenderCommand::SetClearColor(glm::vec4(0.2, 0.2, 0.5, 1));	
	// Must be here, for correct ordering, it is not part of sorting system yet
	m_ParticleSystem->Update(dt);
	XYZ::Renderer2D::BeginScene(m_CameraController->GetCamera());
	

	XYZ::Renderer2D::Flush();
	XYZ::Renderer2D::EndScene();
	

	//Movement
	if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_LEFT))
	{
		m_SpriteBody->velocity.x = -m_Speed;
		m_SpriteBody->velocity.y = 0;
		m_AnimController.StartAnimation("walkleft");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_RIGHT))
	{
		m_SpriteBody->velocity.x = m_Speed;
		m_SpriteBody->velocity.y = 0;
		m_AnimController.StartAnimation("walkright");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_DOWN))
	{
		m_SpriteBody->velocity.y = -m_Speed;
		m_SpriteBody->velocity.x = 0;
		m_AnimController.StartAnimation("walkdown");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_UP))
	{
		m_SpriteBody->velocity.y = m_Speed;
		m_SpriteBody->velocity.x = 0;
		m_AnimController.StartAnimation("walkup");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else
	{
		m_SpriteBody->velocity.x = 0;
		m_SpriteBody->velocity.y = 0;
		m_AnimController.StartAnimation("idle");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}

	m_SpriteTransform->pos.x += m_SpriteBody->velocity.x * dt;
	m_SpriteTransform->pos.y += m_SpriteBody->velocity.y * dt;
	m_Sprite->position = glm::vec3(m_SpriteTransform->pos, 1);

	m_SubEffect2->emitter = m_SpriteTransform->pos;


	auto [x, y] = XYZ::Input::GetMousePosition();
	float width = (float)XYZ::Input::GetWindowSize().first;
	float height = (float)XYZ::Input::GetWindowSize().second;
	auto bounds = m_CameraController->GetBounds();

	auto pos = m_CameraController->GetCamera().GetPosition();

	x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
	y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
	x *= m_CameraController->GetZoomLevel();
	y *= m_CameraController->GetZoomLevel();

	m_SubEffect->emitter = glm::vec2(x + pos.x, y + pos.y);
	
}


void TestLayer::OnMouseButtonPress(XYZ::event_ptr event)
{
	std::shared_ptr<XYZ::MouseButtonPressEvent> e = std::dynamic_pointer_cast<XYZ::MouseButtonPressEvent>(event);
	if (e->GetButton() == XYZ::MouseCode::XYZ_MOUSE_BUTTON_RIGHT)
	{
		// Temporary
		m_Material->ReloadShader();
	}
}
