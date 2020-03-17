#include "stdafx.h"
#include "ParticleEffect.h"


namespace XYZ {
	ParticleEffect::ParticleEffect()
		:emitter(glm::vec2(0)), textureColumns(1), textureRows(1)
	{
	}

	ParticleEffect::ParticleEffect(const glm::vec2& emitPosition, const std::vector<ParticleProps2D>& particles, std::shared_ptr<VertexArray>& vao)
		: emitter(emitPosition), textureColumns(1), textureRows(1)
	{
		m_Vertices.resize(particles.size());
		m_Data.resize(particles.size());
		float dist = 1.0f / particles.size();

		for (size_t i = 0; i < particles.size(); ++i)
		{
			m_Vertices[i].position.x = particles[i].position.x;
			m_Vertices[i].position.y = particles[i].position.y;
			m_Vertices[i].position.z = dist * i;
			m_Vertices[i].color = particles[i].colorBegin;
			m_Vertices[i].angle = 0.0f;
			m_Vertices[i].texCoordOffset = glm::vec2(0);

			m_Data[i].colorBegin = particles[i].colorBegin;
			m_Data[i].colorEnd = particles[i].colorEnd;
			m_Data[i].sizeBegin = particles[i].sizeBegin;
			m_Data[i].sizeEnd = particles[i].sizeEnd;
			m_Data[i].rotation = particles[i].rotation;
			m_Data[i].lifeTime = particles[i].lifeTime;
			m_Data[i].startVelocity = particles[i].velocity;
			m_Data[i].endVelocity = particles[i].velocity;
		}

		BufferLayout buflayout = {
				{2, ShaderDataType::Float4, "a_IColor",			  1},
				{3, ShaderDataType::Float4, "a_IPosition",		  1},
				{4, ShaderDataType::Float2, "a_ITexCoordOffset",  1},
				{5, ShaderDataType::Float2, "a_ISize",			  1},
				{6, ShaderDataType::Float,  "a_IAngle",			  1},
				{7, ShaderDataType::Float,  "a_IAlignment",		  1},
				{8, ShaderDataType::Float,  "a_IAlignment2",	  1},
				{9, ShaderDataType::Float,  "a_IAlignment3",	  1}
		};
		VertexStorage = ShaderStorageBuffer::Create((float*)m_Vertices.data(), (unsigned int)m_Vertices.size() * (unsigned int)sizeof(ParticleVertex));
		VertexStorage->SetLayout(buflayout);
		vao->AddShaderStorageBuffer(VertexStorage);

		PropsStorage = ShaderStorageBuffer::Create((float*)m_Data.data(), (unsigned int)m_Data.size() * (unsigned int) sizeof(ParticleData));
	}

	void ParticleEffect::Bind()
	{
		PropsStorage->BindRange(0, (int)m_Data.size() * (int)sizeof(ParticleData), 1);
		VertexStorage->BindRange(0, (int)m_Vertices.size() * (int)sizeof(ParticleVertex), 0);
	}
}