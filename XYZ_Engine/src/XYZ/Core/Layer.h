#pragma once
#include "Event/Event.h"


namespace XYZ {

	//struct LayerMask : public Type<LayerMask>
	//{
	//	int32_t mask;
	//};


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
