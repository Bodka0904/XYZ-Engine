#pragma once
#include "XYZ/Physics/PhysicsComponent.h"
#include "XYZ/Renderer/Renderable2D.h"

namespace XYZ {
	class SceneNode
	{
	public:
		void SetParent(SceneNode* parent);
		void Render();

	private:
		ComponentWrapper<Transform2D> Transform;
		ComponentWrapper<Renderable2D> Renderable;

		SceneNode* Parent;
		SceneNode* FirstChild;
		SceneNode* NextSibling;
	};
}