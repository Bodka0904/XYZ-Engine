#pragma once

#include "OrthoCamera.h"
#include "XYZ/Renderer/Texture.h"


namespace XYZ {
	class Renderer2D
	{

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		// Draws here:
	private:
		
	};
}