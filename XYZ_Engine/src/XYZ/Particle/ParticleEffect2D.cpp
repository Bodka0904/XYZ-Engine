#include "stdafx.h"
#include "ParticleEffect2D.h"


namespace XYZ {
	ParticleEffect2D::ParticleEffect2D(size_t maxParticles, std::shared_ptr<Material> material)
		: m_Material(material)
	{
		m_Material->Set("u_Speed", 1.0f);
		m_Material->Set("u_Gravity", -9.8f);
		m_Material->SetRoutine("blueColor");


		if (maxParticles > sc_MaxParticlesPerEffect)
		{
			XYZ_LOG_WARN("Max number of particles per effect is ", sc_MaxParticlesPerEffect);
			m_MaxParticles = sc_MaxParticlesPerEffect;
		}
		else
			m_MaxParticles = maxParticles;

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
		m_VertexStorage = ShaderStorageBuffer::Create((uint32_t)m_MaxParticles * (uint32_t)sizeof(ParticleVertex));
		m_VertexStorage->SetLayout(buflayout);

		m_PropsStorage = ShaderStorageBuffer::Create((uint32_t)m_MaxParticles * (uint32_t) sizeof(ParticleData));
	}


	void ParticleEffect2D::ConnectToVertexArray(std::shared_ptr<VertexArray>& vao)
	{
		vao->AddShaderStorageBuffer(m_VertexStorage);
	}

	void ParticleEffect2D::Bind()
	{
		for (auto subEffect : m_SubEffects)
		{
			subEffect->m_MaterialI->Set("u_Emitter", subEffect->emitter);
			subEffect->m_MaterialI->Set("u_NumberRows", (float)subEffect->textureRows);
			subEffect->m_MaterialI->Set("u_NumberColumns", (float)subEffect->textureColumns);
			subEffect->m_MaterialI->Bind();

			m_PropsStorage->BindRange((uint32_t)subEffect->m_Index * (uint32_t)sizeof(ParticleData), (uint32_t)subEffect->m_Data.size() * (uint32_t)sizeof(ParticleData), 1);
			m_VertexStorage->BindRange((uint32_t)subEffect->m_Index * (uint32_t)sizeof(ParticleVertex), (uint32_t)subEffect->m_Vertices.size() * (uint32_t)sizeof(ParticleVertex), 0);
			m_Material->GetShader()->Compute(32, 32, 1);
		}
	}
	ParticleSubEffect2D::ParticleSubEffect2D(ParticleEffect2D* effect, const std::vector<ParticleProps2D>& particles)
		: m_Effect(effect), m_MaterialI(MaterialInstance::Create(effect->m_Material)), emitter(glm::vec2(0)), textureRows(1), textureColumns(1)
	{
		size_t numParticles;
		if (particles.size() > effect->m_MaxParticles - effect->m_ParticlesInExistence)
		{
			XYZ_LOG_WARN("Attempting to create particles over limit ", effect->m_MaxParticles);
			numParticles = effect->m_MaxParticles - effect->m_ParticlesInExistence;
		}
		else
			numParticles = particles.size();


		m_Vertices.resize(numParticles);
		m_Data.resize(numParticles);

		m_Index = effect->m_ParticlesInExistence;
		effect->m_ParticlesInExistence += numParticles;

		float dist = 1.0f / effect->m_MaxParticles;

		for (size_t i = 0; i < numParticles; ++i)
		{
			m_Vertices[i].position.x = particles[i].position.x;
			m_Vertices[i].position.y = particles[i].position.y;
			m_Vertices[i].position.z = dist * (i + m_Index);
			m_Vertices[i].color = particles[i].colorBegin;
			m_Vertices[i].angle = 0.0f;
			m_Vertices[i].texCoordOffset = glm::vec2(0);

			m_Data[i].defaultPosition.y = particles[i].position.x;
			m_Data[i].defaultPosition.x = particles[i].position.y;
			m_Data[i].colorBegin = particles[i].colorBegin;
			m_Data[i].colorEnd = particles[i].colorEnd;
			m_Data[i].sizeBegin = particles[i].sizeBegin;
			m_Data[i].sizeEnd = particles[i].sizeEnd;
			m_Data[i].rotation = particles[i].rotation;
			m_Data[i].lifeTime = particles[i].lifeTime;
			m_Data[i].startVelocity = particles[i].velocity;
			m_Data[i].endVelocity = particles[i].velocity;
		}

		effect->m_VertexStorage->Update((float*)m_Vertices.data(), (uint32_t)m_Vertices.size() * (uint32_t)sizeof(ParticleEffect2D::ParticleVertex), (uint32_t)m_Index * (uint32_t)sizeof(ParticleEffect2D::ParticleVertex));
		effect->m_PropsStorage->Update((float*)m_Data.data(), (uint32_t)m_Data.size() * (uint32_t)sizeof(ParticleEffect2D::ParticleData), (uint32_t)m_Index * (uint32_t)sizeof(ParticleEffect2D::ParticleData));

		effect->m_SubEffects.insert(this);
	}
	ParticleSubEffect2D::~ParticleSubEffect2D()
	{
		auto it = m_Effect->m_SubEffects.find(this);
		if (it != m_Effect->m_SubEffects.end())
		{
			auto effect = it;
			effect++;
			for (effect; effect != m_Effect->m_SubEffects.end(); ++effect)
				(*effect)->m_Index -= (*it)->m_Vertices.size();


			m_Effect->m_ParticlesInExistence -= m_Vertices.size();
			m_Effect->m_SubEffects.erase(this);
		}

	}
}