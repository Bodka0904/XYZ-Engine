#pragma once
#include "XYZ/Renderer/VertexArray.h"
#include <glm/glm.hpp>
#include <vector>

namespace XYZ {
	struct ParticleProps2D
	{
		glm::vec4 colorBegin;
		glm::vec4 colorEnd;
		glm::vec2 position;
		glm::vec2 velocity;
		float sizeBegin;
		float sizeEnd;
		float lifeTime;
		float rotation;
	};
	class ParticleEffect
	{
	public:
		ParticleEffect();
		ParticleEffect(const glm::vec2& emitPosition, const std::vector<ParticleProps2D>& particles, std::shared_ptr<VertexArray>& vao);
		void Bind();

		glm::vec2 emitter;
		int textureColumns;
		int textureRows;
	private:
		struct ParticleVertex
		{
			glm::vec4 color;
			glm::vec4 position;
			glm::vec2 texCoordOffset;
			glm::vec2 size;
			float	  angle;
			float	  alignment;
			float	  alignment2;
			float	  alignment3;
		};
		struct ParticleData
		{
			glm::vec4 colorBegin;
			glm::vec4 colorEnd;
			glm::vec2 startVelocity;
			glm::vec2 endVelocity;
			float sizeBegin;
			float sizeEnd;
			float rotation;
			float lifeTime;
			float timeAlive = 0.0f;

			float alignment;
			float alignment2;
			float alignment3;
		};


		std::vector<ParticleVertex> m_Vertices;
		std::vector<ParticleData> m_Data;

		std::shared_ptr<ShaderStorageBuffer> VertexStorage;
		std::shared_ptr<ShaderStorageBuffer> PropsStorage;
	};

}