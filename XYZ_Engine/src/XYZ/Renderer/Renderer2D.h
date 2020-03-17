#pragma once

#include "Camera.h"
#include "XYZ/Renderer/Texture.h"


namespace XYZ {
	class Renderer2D
	{

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		// Draws here:
	private:
		
	};
}