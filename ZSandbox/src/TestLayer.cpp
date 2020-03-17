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
	XYZ::ECSManager::Get()->Init();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::Transform2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::RigidBody2D>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::ParticleEffect>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::Sprite>();
	XYZ::ECSManager::Get()->RegisterComponent<XYZ::SpriteAnimation>();


	m_PhysicsSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::PhysicsSystem>();
	m_ParticleSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::ParticleSystem>();
	m_SpriteSystem = XYZ::ECSManager::Get()->RegisterSystem<XYZ::SpriteSystem>();

	XYZ::Renderer::Init();
	m_Camera = std::make_shared<XYZ::Camera>(0, XYZ::Input::GetWindowSize().first, 0, XYZ::Input::GetWindowSize().second);	
	m_Material = XYZ::Material::Create(XYZ::Shader::Create("TextureShader", "../XYZ_Engine/Assets/Shaders/DefaultShader.glsl"));
	

	m_Entity = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_Entity, new XYZ::Transform2D{});
	XYZ::ECSManager::Get()->AddComponent(m_Entity, new XYZ::RigidBody2D{});
	XYZ::ECSManager::Get()->SaveEntity(m_Entity);

	m_Material->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
	m_Material->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp, "../XYZ_Engine/Assets/Textures/animation.png"));
	


	//////////////////////////////////////////////////////////////////////////////////////

	

	m_ParticleEntity = XYZ::ECSManager::Get()->CreateEntity();

	m_ParticleVAO = XYZ::VertexArray::Create();

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<> dist(-5, 5);

	std::vector<XYZ::ParticleProps2D> particles;
	particles.resize(100000);
	for (size_t i = 0; i < particles.size(); ++i)
	{
		particles[i].position = glm::vec2(0, 0);
		particles[i].sizeBegin = 1.0f;
		particles[i].sizeEnd = 0.0f;
		particles[i].colorEnd = glm::vec4(1, 0, 1, 0);
		particles[i].colorBegin = glm::vec4(0, 1, 1, 1);
		particles[i].velocity = glm::vec2(dist(rng), fabs(dist(rng))*3);
		particles[i].lifeTime = fabs(dist(rng))*4;
		particles[i].rotation = dist(rng) * 50;
	}

	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::ParticleEffect(glm::vec2(200,200),particles,m_ParticleVAO));
	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::Transform2D{});
	XYZ::ECSManager::Get()->AddComponent(m_ParticleEntity, new XYZ::RigidBody2D{});

	m_ParQuad = new ParticleQuad(glm::vec2(0), glm::vec2(50.0f));
	std::shared_ptr<XYZ::VertexBuffer> squareVBpar;
	squareVBpar = XYZ::VertexBuffer::Create(m_ParQuad->squareVert, sizeof(m_ParQuad->squareVert), XYZ::BufferUsage::Dynamic);
	squareVBpar->SetLayout({
		{  0, XYZ::ShaderDataType::Float3, "a_Position" },
		{  1, XYZ::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_ParticleVAO->AddVertexBuffer(squareVBpar);
	

	uint32_t squareIndpar[] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<XYZ::IndexBuffer> squareIBpar;
	squareIBpar = XYZ::IndexBuffer::Create(squareIndpar, sizeof(squareIndpar) / sizeof(uint32_t));
	m_ParticleVAO->SetIndexBuffer(squareIBpar);

	
	XYZ::ECSManager::Get()->SaveEntity(m_ParticleEntity);
	m_Body = &XYZ::ECSManager::Get()->GetComponent<XYZ::RigidBody2D>(m_ParticleEntity);
	m_Transform = &XYZ::ECSManager::Get()->GetComponent<XYZ::Transform2D>(m_ParticleEntity);
	m_Effect = &XYZ::ECSManager::Get()->GetComponent<XYZ::ParticleEffect>(m_ParticleEntity);
	m_Effect->textureColumns = 8.0f;
	m_Effect->textureRows = 8.0f;
	
	m_ParticleMaterial = XYZ::Material::Create(XYZ::Shader::Create("../XYZ_Engine/Assets/Shaders/Particle/DefaultParticleShader.glsl"));
	m_ParticleMaterial->Set("u_Texture", XYZ::Texture2D::Create(XYZ::TextureWrap::Clamp,"../XYZ_Engine/Assets/Textures/fire.png"));


	m_SpriteEntity = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::Sprite(glm::vec3(200,200,0),glm::vec2(100,100),glm::vec4(1,1,1,1),glm::vec4(0.0f,0.0f,1.0f/6.0f,1.0f)));
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::SpriteAnimation(1, 6, 96, 16));
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::RigidBody2D{});
	XYZ::ECSManager::Get()->AddComponent(m_SpriteEntity, new XYZ::Transform2D{});
	XYZ::ECSManager::Get()->SaveEntity(m_SpriteEntity);
	
	m_SpriteBody = &XYZ::ECSManager::Get()->GetComponent<XYZ::RigidBody2D>(m_SpriteEntity);
	m_SpriteTransform = &XYZ::ECSManager::Get()->GetComponent<XYZ::Transform2D>(m_SpriteEntity);
	m_SpriteTransform->pos.y = 200;
	m_Sprite = &XYZ::ECSManager::Get()->GetComponent<XYZ::Sprite>(m_SpriteEntity);
	m_SpriteAnimation = &XYZ::ECSManager::Get()->GetComponent<XYZ::SpriteAnimation>(m_SpriteEntity);
	m_SpriteAnimation->SetFrameInterval(0, 5, 0.5f);

	m_SpriteVAO = XYZ::VertexArray::Create();
	std::shared_ptr<XYZ::VertexBuffer> Vbo = XYZ::VertexBuffer::Create((float*)m_Sprite->GetVertices(), 4 * sizeof(XYZ::Vertex), XYZ::BufferUsage::Dynamic);
	Vbo->SetLayout(XYZ::BufferLayout{
		{0, XYZ::ShaderDataType::Float4, "a_Color" },
		{1, XYZ::ShaderDataType::Float3, "a_Position" },
		{2, XYZ::ShaderDataType::Float2, "a_TexCoord" },
		{3, XYZ::ShaderDataType::Float,  "a_TextureID" },
		});
	m_SpriteVAO->AddVertexBuffer(Vbo);

	uint32_t squareInd[] = { 0, 1, 2, 2, 3, 0 };
	std::shared_ptr<XYZ::IndexBuffer> squareIB;
	squareIB = XYZ::IndexBuffer::Create(squareInd, sizeof(squareInd) / sizeof(uint32_t));
	m_SpriteVAO->SetIndexBuffer(squareIB);


	m_AnimController.AddAnimation("idle", XYZ::Animation{
		std::pair<int,int>(0,1),
		0.3f
		});
	m_AnimController.AddAnimation("walkright", XYZ::Animation{
		std::pair<int,int>(2,3),
		0.5f
		});
	m_AnimController.AddAnimation("walkleft", XYZ::Animation{
		std::pair<int,int>(4,5),
		0.5f
		});
}

void TestLayer::OnDetach()
{
	
}

void TestLayer::OnUpdate(float ts)
{
	m_PhysicsSystem->Update(ts);
	m_ParticleSystem->Update(ts);
	m_SpriteSystem->Update(ts);

	XYZ::RenderCommand::Clear();
	XYZ::RenderCommand::SetClearColor(glm::vec4(0.2, 0.2, 0.5, 1));
	XYZ::Renderer2D::BeginScene(*m_Camera);
	m_ParticleMaterial->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
	m_ParticleMaterial->Bind();
	m_ParticleVAO->Bind();
	XYZ::RenderCommand::DrawInstanced(m_ParticleVAO, 100000);
	m_Material->Set("u_ViewProjection", m_Camera->GetViewProjectionMatrix());
	m_Material->Bind();
	m_SpriteVAO->Bind();
	XYZ::RenderCommand::DrawIndexed(m_SpriteVAO);
	XYZ::Renderer2D::EndScene();

	m_SpriteVAO->GetVertexBuffer()[0]->Update((float*)m_Sprite->GetVertices(), 4 * sizeof(XYZ::Vertex));
	//Movement

	if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_SPACE))
	{
		m_SpriteBody->velocity.y = m_JumpSpeed;
	}
	if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_LEFT))
	{
		m_SpriteBody->velocity.x = -m_Speed;
		m_AnimController.StartAnimation("walkleft");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else if (XYZ::Input::IsKeyPressed(XYZ::KeyCode::XYZ_KEY_RIGHT))
	{
		m_SpriteBody->velocity.x = m_Speed;
		m_AnimController.StartAnimation("walkright");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	else
	{
		m_SpriteBody->velocity.x = 0;
		m_AnimController.StartAnimation("idle");
		m_AnimController.UpdateSpriteAnimation(m_SpriteAnimation);
	}
	
	m_SpriteTransform->pos.x += m_SpriteBody->velocity.x;
	m_Sprite->SetPosition(glm::vec3(m_SpriteTransform->pos, 1));
	m_Effect->emitter = glm::vec2(XYZ::Input::GetMousePosition().first, XYZ::Input::GetWindowSize().second - XYZ::Input::GetMousePosition().second);
}

void TestLayer::OnEvent(XYZ::Event& event)
{
	if (event.GetEventType() == XYZ::EventType::MouseButtonPressed)
	{
		XYZ::MouseButtonPressEvent& e = (XYZ::MouseButtonPressEvent&)event;

		if (e.GetButton() == XYZ::MouseCode::XYZ_MOUSE_BUTTON_RIGHT)
		{
			// Temporary
			m_Material->ReloadShader();	
		}
	}
	else if (event.GetEventType() == XYZ::EventType::WindowResized)
	{
		
		XYZ::WindowResizeEvent& e = (XYZ::WindowResizeEvent&) event;
		m_Camera->SetProjection(0, e.GetWidth(), 0, e.GetHeight());
	}
}
