#include "Map.h"

#include <random>


Map::Map()
{	
}

void Map::Init(int width, std::shared_ptr<XYZ::Material> material)
{

	m_Init = true;

	m_Width = width;
	m_Material = material;
	m_Walls.resize(width * width);

	m_BackGround = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_BackGround, XYZ::Renderable2D(
		m_Material,
		glm::vec4(1),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec3(10, 10, -0.5),
		glm::vec2(20),
		0.0f,
		true,
		2
	));
	
}

void Map::Destroy()
{
	if (m_Init)
	{
		for (size_t i = 0; i < m_Walls.size(); ++i)
		{
			XYZ::ECSManager::Get()->DestroyEntity(m_Walls[i]);
		}
		XYZ::ECSManager::Get()->DestroyEntity(m_BackGround);
		m_Init = false;
	}
}

void Map::SetTile(int row, int col)
{
	m_Walls[((m_Width-1) * row) + col] = XYZ::ECSManager::Get()->CreateEntity();
	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width - 1) * row) + col], XYZ::Renderable2D(
		m_Material,
		glm::vec4(1),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
		glm::vec3(0, 0, 0),
		glm::vec2(1),
		0.0f,
		true,
		1
	));

	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width-1) * row) + col], XYZ::SpriteAnimation(1, 12, 720, 60));
	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width-1) * row) + col], XYZ::RigidBody2D{});
	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width-1) * row) + col], XYZ::GridPosition(row, col));
	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width-1) * row) + col], XYZ::InterpolatedMovement{});
	XYZ::ECSManager::Get()->AddComponent(m_Walls[((m_Width-1) * row) + col], XYZ::LayerMask(1));
}


void Map::Generate(int numOfTiles)
{
	std::set<std::pair<int,int> > m_Tiles;
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> dist(0, m_Width-1); // define the range

	for (int i = 0; i < numOfTiles; ++i)
	{
		if (!m_Tiles.insert({ dist(eng),dist(eng) }).second)
			i--;
	}

	for (auto it : m_Tiles)
	{
		SetTile(it.first, it.second);
	}
}


