#pragma once

#include <XYZ.h>

#include "Bomb.h"
#include "Map.h"
#include "Player.h"
#include "Menu.h"

class GameLayer : public XYZ::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float dt) override;


private:
	void RestartGame();
	void InitBackgroundParticles();
private:
	std::shared_ptr<XYZ::PhysicsSystem> m_PhysicsSystem;
	std::shared_ptr<XYZ::ParticleSystem2D> m_ParticleSystem;
	std::shared_ptr<XYZ::SpriteSystem> m_SpriteSystem;

	std::shared_ptr<XYZ::Material> m_Material;
	std::shared_ptr<XYZ::Material> m_ParticleMaterial;
	std::shared_ptr<XYZ::OrthoCamera> m_Camera;


	// Testing environment
	std::vector<Player> m_Players;
	std::vector<Bomb> m_Bombs;

	XYZ::Entity m_ParticleEntity;
	XYZ::Entity m_EmitterEntity1;
	XYZ::Entity m_EmitterEntity2;

	int m_NumParticles = 0;

	Map m_Map;
	Menu m_Menu;
	XYZ::HandlerID m_PutBomb;
	std::vector<std::pair<int, int>> m_DamagedCells;
};
