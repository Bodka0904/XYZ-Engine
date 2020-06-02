#pragma once

#include <XYZ.h>


class GameLayer : public XYZ::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float ts) override;
	virtual void OnImGuiRender() override;

private:

	void InitBackgroundParticles(XYZ::Entity entity);
private:
	std::shared_ptr<XYZ::PhysicsSystem> m_PhysicsSystem;
	std::shared_ptr<XYZ::ParticleSystem2D> m_ParticleSystem;
	std::shared_ptr<XYZ::SpriteSystem> m_SpriteSystem;
	std::shared_ptr<XYZ::InterpolatedMovementSystem> m_InterSystem;
	std::shared_ptr<XYZ::GridCollisionSystem> m_GridCollisionSystem;
	std::shared_ptr<XYZ::RealGridCollisionSystem> m_RealGridCollisionSystem;

	std::shared_ptr<XYZ::Material> m_Material;
	std::shared_ptr<XYZ::Material> m_ParticleMaterial;
	std::shared_ptr<XYZ::OrthoCameraController> m_CameraController;



	XYZ::Entity m_ParticleEntity;


	int m_PlayableArea = 20;


	std::shared_ptr<XYZ::AudioSource> m_Audio;

	std::shared_ptr<XYZ::Material> m_FluidMaterial;
};