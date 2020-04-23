#pragma once

#include <XYZ.h>

struct Quad
{
	Quad(glm::vec2 pos, glm::vec2 size)
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



class TestLayer : public XYZ::Layer
{
public:
	TestLayer();
	virtual ~TestLayer();

	void AddEntity(XYZ::Entity entity) { };
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float dt) override;

private:
	void OnMouseButtonPress(XYZ::event_ptr event);

	XYZ::HandlerID m_MouseButtonPress;

private:

	std::shared_ptr<XYZ::OrthoCameraController> m_CameraController;
	std::shared_ptr<XYZ::VertexArray> m_VAO;

	std::shared_ptr<XYZ::Material> m_Material;
	std::shared_ptr<XYZ::PhysicsSystem> m_PhysicsSystem;


	XYZ::Entity m_ParticleEntity;

	std::shared_ptr<XYZ::ParticleSystem2D> m_ParticleSystem;
	std::shared_ptr<XYZ::Material> m_ParticleMaterial;


	int m_Speed = 1.0f;
	float m_Color = 0.0f;


	XYZ::Entity m_SpriteEntity;
	XYZ::SpriteAnimationController m_AnimController;
	std::shared_ptr<XYZ::SpriteSystem> m_SpriteSystem;

	std::vector<XYZ::Entity> m_TestEntities;
};