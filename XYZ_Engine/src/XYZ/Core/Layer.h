#pragma once
#include "Event/Event.h"


namespace XYZ {


	class LayerStack;
	class Layer
	{
		friend class LayerStack;
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {} //Init
		virtual void OnDetach() {} //Destroy
		virtual void OnUpdate(float ts) {} //Update

	protected:
		int32_t m_Key;
	};

}
