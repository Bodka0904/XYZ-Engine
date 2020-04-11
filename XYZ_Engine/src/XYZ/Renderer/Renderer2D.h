#pragma once

#include "OrthoCamera.h"
#include "RendererBatchSystem2D.h"
#include "RenderCommandQueue.h"


namespace XYZ {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthoCamera& camera);
		static void Submit(CommandI& command, unsigned int size);
		static void Flush();
		static void EndScene();

	private:
		static Renderer2D* s_Instance;

		RenderCommandQueue m_CommandQueue;
		std::shared_ptr<RendererBatchSystem2D> m_SortSystem;
	};
}