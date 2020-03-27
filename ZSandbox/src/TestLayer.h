#pragma once

#include <XYZ.h>

struct Quad
{
	Quad(glm::vec2 pos,glm::vec2 size)
		: Size(size)
	{
		squareVert[0] += pos.x - size.x;
		squareVert[5] += pos.x + size.x;
		squareVert[10] += pos.x + size.x;
		squareVert[15] += pos.x - size.x;

		squareVert[1] += pos.y - size.y;
		squareVert[6] += pos.y - size.y;
		squareVert[11] += pos.y + size.y;
		squareVert[16] += pos.y + size.y;
	}

	void SetPosition(glm::vec2 pos)
	{
		squareVert[0] = pos.x - Size.x;
		squareVert[5] = pos.x + Size.x;
		squareVert[10] = pos.x + Size.x;
		squareVert[15] = pos.x - Size.x;

		squareVert[1] = pos.y - Size.y;
		squareVert[6] = pos.y - Size.y;
		squareVert[11] = pos.y + Size.y;
		squareVert[16] = pos.y + Size.y;
	}

	float squareVert[20] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	glm::vec2 Size;
};


struct ParticleQuad
{
	ParticleQuad(glm::vec2 pos, glm::vec2 size)
		: Size(size)
	{
		squareVert[0] += pos.x - size.x;
		squareVert[5] += pos.x + size.x;
		squareVert[10] += pos.x + size.x;
		squareVert[15] += pos.x - size.x;

		squareVert[1] += pos.y - size.y;
		squareVert[6] += pos.y - size.y;
		squareVert[11] += pos.y + size.y;
		squareVert[16] += pos.y + size.y;
	}
		
	float squareVert[20] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};
	glm::vec2 Size;
};


class TestLayer : public XYZ::Layer
{
public:
	TestLayer();
	virtual ~TestLayer();

	void AddEntity(XYZ::Entity entity) { m_Entities.push_back(entity); };
	virtual void OnAttach() override; 
	virtual void OnDetach() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(XYZ::Event& event) override;

private:
	std::vector<XYZ::Entity> m_Entities;

	//std::shared_ptr<XYZ::OrthoCamera> m_Camera;
	std::shared_ptr<XYZ::OrthoCameraController> m_CameraController;
	std::shared_ptr<XYZ::VertexArray> m_VAO;

	std::shared_ptr<XYZ::Material> m_Material;
	std::shared_ptr<XYZ::PhysicsSystem> m_PhysicsSystem;

	XYZ::Entity m_Entity;


	XYZ::Entity m_ParticleEntity;

	XYZ::ParticleSubEffect2D* m_SubEffect;
	XYZ::ParticleSubEffect2D* m_SubEffect2;

	std::shared_ptr<XYZ::VertexArray> m_ParticleVAO;
	std::shared_ptr<XYZ::ParticleSystem2D> m_ParticleSystem;
	std::shared_ptr<XYZ::Material> m_ParticleMaterial;
	ParticleQuad* m_ParQuad;

	XYZ::RigidBody2D* m_Body;
	XYZ::Transform2D* m_Transform;
	XYZ::ParticleEffect2D* m_Effect;
	
	Quad* m_Quad;
	float m_Speed = 1.0f;
	float m_JumpSpeed = 5.0f;
	float m_Color = 0.0f;


	std::shared_ptr<XYZ::VertexArray> m_SpriteVAO;
	XYZ::Entity m_SpriteEntity;
	XYZ::Sprite* m_Sprite;
	XYZ::RigidBody2D* m_SpriteBody;
	XYZ::Transform2D* m_SpriteTransform;
	XYZ::SpriteAnimation* m_SpriteAnimation;
	XYZ::SpriteAnimationController m_AnimController;
	std::shared_ptr<XYZ::SpriteSystem> m_SpriteSystem;
};