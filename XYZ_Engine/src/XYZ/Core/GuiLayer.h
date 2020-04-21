#pragma once
#include "Layer.h"


namespace XYZ {
	class GuiLayer : public Layer
	{
	public:
		GuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
	};
}