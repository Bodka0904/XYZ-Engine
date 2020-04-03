#pragma once
#include "XYZ/Renderer/VertexArray.h"
#include "XYZ/Renderer/Material.h"

#include <glm/glm.hpp>
#include <set>
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
	class ParticleEffect2D
	{
		friend class ParticleSubEffect2D;
	public:
		ParticleEffect2D(size_t maxParticles,std::shared_ptr<Material> material);

		void ConnectToVertexArray(std::shared_ptr<VertexArray>& vao);
		void Bind();

		std::shared_ptr<Material> GetMaterial() { return m_Material; }
		size_t GetNumExistingParticles() { return m_ParticlesInExistence; }

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
			glm::vec2 defaultPosition;
			float sizeBegin;
			float sizeEnd;
			float rotation;
			float lifeTime;
			float timeAlive = 0.0f;

			float alignment;
		};

	private:
		std::shared_ptr<Material> m_Material;
		std::set<ParticleSubEffect2D*> m_SubEffects;
		size_t m_ParticlesInExistence = 0;
		size_t m_MaxParticles = 0;


		std::shared_ptr<ShaderStorageBuffer> m_VertexStorage;
		std::shared_ptr<ShaderStorageBuffer> m_PropsStorage;

		static constexpr size_t sc_MaxParticlesPerEffect = 10000;
	};


	class ParticleSubEffect2D
	{
		friend class ParticleEffect2D;
	public:
		ParticleSubEffect2D(ParticleEffect2D* effect, const std::vector<ParticleProps2D>& particles);
		~ParticleSubEffect2D();

	public:
		glm::vec2 emitter;
		int textureColumns;
		int textureRows;

	private:
		ParticleEffect2D* m_Effect;
		std::shared_ptr<MaterialInstance> m_MaterialI;

		std::vector<ParticleEffect2D::ParticleVertex> m_Vertices;
		std::vector<ParticleEffect2D::ParticleData> m_Data;

		size_t m_Index;

	};


	struct ParticleComponent
	{
		std::shared_ptr<ParticleEffect2D> effect;
		std::shared_ptr<Material> material;
	};
}