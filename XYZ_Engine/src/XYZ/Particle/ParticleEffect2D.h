#pragma once
#include "XYZ/Renderer/VertexArray.h"
#include "XYZ/Renderer/Renderable2D.h"
#include "XYZ/Renderer/Material.h"
#include "XYZ/Utils/DataStructures/FreeList.h"

#include <glm/glm.hpp>
#include <set>
#include <vector>

namespace XYZ {

	struct ParticleQuad
	{
		ParticleQuad(glm::vec2 pos, glm::vec2 size)
			: Size(size)
		{
			squareVert[0] += pos.x - size.x;
			squareVert[5] += pos.x + size.x;
			squareVert[10] += pos.x + size.x;
			squareVert[15] += pos.x - size.x;

			squareVert[1] += pos.y - size.y;
			squareVert[6] += pos.y - size.y;
			squareVert[11] += pos.y + size.y;
			squareVert[16] += pos.y + size.y;
		}

		float squareVert[20] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		glm::vec2 Size;
	};


	/**
	* @struct ParticleProps2D
	* @brief properties of single particle
	*/
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


	struct ParticleEmitter : public Type<ParticleEmitter>
	{
		glm::vec2 position;
		glm::vec2 size;
		int offset;
		int numParticles;
		std::shared_ptr<MaterialInstance> material;
	};

	/**
	* 
	*/
	class ParticleEffect2D : public Type<ParticleEffect2D>
	{
		friend class ParticleSubEffect2D;
	public:
		ParticleEffect2D(size_t maxParticles, std::shared_ptr<Material> material, std::shared_ptr<Material> renderMaterial);
		~ParticleEffect2D();

		void Render();
		void Bind(const ParticleEmitter& emitter);
		void PushParticle(const ParticleProps2D& particle);

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

			float	  alignment = 0.0f;
			float	  alignment2 = 0.0f;
			float	  alignment3 = 0.0f;
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

			float alignment = 0.0f;
		};
		
	private:
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<Material> m_Material;
		std::shared_ptr<Material> m_RenderMaterial;

		uint32_t m_ParticlesInExistence = 0;
		uint32_t m_MaxParticles = 0;

		std::shared_ptr<ShaderStorageBuffer> m_VertexStorage;
		std::shared_ptr<ShaderStorageBuffer> m_PropsStorage;

		std::vector<ParticleVertex> m_Vertices;
		std::vector<ParticleData> m_Data;

		static constexpr size_t sc_MaxParticlesPerEffect = 10000;
	};

}