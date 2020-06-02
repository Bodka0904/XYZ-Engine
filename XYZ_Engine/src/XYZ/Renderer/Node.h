#pragma once
#include "XYZ/Physics/PhysicsComponent.h"
#include "XYZ/Renderer/Renderable2D.h"

namespace XYZ {


	struct Node
	{
		void SetParent(Node* parent);
		void Render();

		ComponentWrapper<Transform2D> Transform;
		ComponentWrapper<Renderable2D> Renderable;
	
	private:
		Node* Parent;
		Node* FirstChild;
		Node* NextSibling;
	};
}