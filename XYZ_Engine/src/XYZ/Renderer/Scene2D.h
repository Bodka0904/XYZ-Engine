#pragma once
#include "XYZ/Renderer/Scene.h"
#include "XYZ/ECS/ECSManager.h"

namespace XYZ {
	class Scene2D : public Scene
	{
	public:



	private:
		std::unordered_map<std::string, Entity> m_Entities;
	};
}