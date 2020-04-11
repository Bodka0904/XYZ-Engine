#pragma once
#include "Event/Event.h"

namespace XYZ {
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {} //Init
		virtual void OnDetach() {} //Destroy
		virtual void OnUpdate(float dt) {} //Update

	};

}
