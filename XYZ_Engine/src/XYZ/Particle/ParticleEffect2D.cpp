#include "stdafx.h"
#include "ParticleEffect2D.h"
#include "XYZ/Renderer/Renderer2D.h"
#include "XYZ/Renderer/RenderCommand.h"
#include "XYZ/Renderer/MaterialManager.h"

namespace XYZ {

	void RenderInstanced(std::shared_ptr<Material> material, std::shared_ptr<VertexArray> vao, uint32_t count)
	{
		material->Bind();
		vao->Bind();
		RenderCommand::DrawInstanced(vao, count);
	}


	ParticleEffect2D::ParticleEffect2D(size_t maxParticles, std::shared_ptr<Material> material, std::shared_ptr<Material> renderMaterial)
		: m_Material(material), m_RenderMaterial(renderMaterial)
	{
		m_Material->Set("u_Speed", 1.0f);
		m_Material->Set("u_Gravity", -9.8f);

		m_Material->Set("u_Emitter", glm::vec2(0, 0));
		m_Material->Set("u_NumberRows", 1.0f);
		m_Material->Set("u_NumberColumns", 1.0f);
		m_Material->SetRoutine("blueColor");



		if (maxParticles > sc_MaxParticlesPerEffect)
		{
			XYZ_LOG_WARN("Max number of particles per effect is ", sc_MaxParticlesPerEffect);
			m_MaxParticles = sc_MaxParticlesPerEffect;
		}
		else
			m_MaxParticles = maxParticles;


		m_Vertices.resize(m_MaxParticles);
		m_Data.resize(m_MaxParticles);

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

		m_PropsStorage = ShaderStorageBuffer::Create((uint32_t)m_MaxParticles * (uint32_t)sizeof(ParticleData));



		// Rendering setup , TODO add particle renderable
		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddShaderStorageBuffer(m_VertexStorage);


		ParticleQuad m_ParQuad = ParticleQuad(glm::vec2(0), glm::vec2(1.0f));
		std::shared_ptr<XYZ::VertexBuffer> squareVBpar;
		squareVBpar = XYZ::VertexBuffer::Create(m_ParQuad.squareVert, sizeof(m_ParQuad.squareVert));
		squareVBpar->SetLayout({
			{  0, XYZ::ShaderDataType::Float3, "a_Position" },
			{  1, XYZ::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_VertexArray->AddVertexBuffer(squareVBpar);

		uint32_t squareIndpar[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<XYZ::IndexBuffer> squareIBpar;
		squareIBpar = XYZ::IndexBuffer::Create(squareIndpar, sizeof(squareIndpar) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(squareIBpar);
	}

	ParticleEffect2D::~ParticleEffect2D()
	{
	}


	void ParticleEffect2D::Render()
	{
		Command< std::shared_ptr<Material>, std::shared_ptr<VertexArray>, uint32_t>cmd(RenderInstanced, m_RenderMaterial, m_VertexArray, m_ParticlesInExistence);
		Renderer2D::Submit(cmd, sizeof(cmd));

	}

	void ParticleEffect2D::Bind(const ParticleEmitter& emitter)
	{
		emitter.material->Bind();

		m_PropsStorage->BindRange(emitter.offset * sizeof(ParticleData), emitter.numParticles * (uint32_t)sizeof(ParticleData), 1);
		m_VertexStorage->BindRange(emitter.offset * sizeof(ParticleData), emitter.numParticles * (uint32_t)sizeof(ParticleVertex), 0);

		//m_PropsStorage->BindRange(0, m_ParticlesInExistence * (uint32_t)sizeof(ParticleData), 1);
		//m_VertexStorage->BindRange(0, m_ParticlesInExistence * (uint32_t)sizeof(ParticleVertex), 0);
		m_Material->GetShader()->Compute(32, 32, 1);
	}

	void ParticleEffect2D::PushParticle(const ParticleProps2D& particle)
	{
		if (m_ParticlesInExistence < m_MaxParticles)
		{
			float dist = 1.0f / m_MaxParticles;
			uint32_t i = m_ParticlesInExistence;

			m_Vertices[i].position.x = particle.position.x;
			m_Vertices[i].position.y = particle.position.y;
			m_Vertices[i].position.z = dist * i;
			m_Vertices[i].color = particle.colorBegin;
			m_Vertices[i].angle = 0.0f;
			m_Vertices[i].texCoordOffset = glm::vec2(0);

			m_Data[i].defaultPosition.x = particle.position.x;
			m_Data[i].defaultPosition.y = particle.position.y;
			m_Data[i].colorBegin = particle.colorBegin;
			m_Data[i].colorEnd = particle.colorEnd;
			m_Data[i].sizeBegin = particle.sizeBegin;
			m_Data[i].sizeEnd = particle.sizeEnd;
			m_Data[i].rotation = particle.rotation;
			m_Data[i].lifeTime = particle.lifeTime;
			m_Data[i].startVelocity = particle.velocity;
			m_Data[i].endVelocity = particle.velocity;

			m_VertexStorage->Update(&m_Vertices[i], sizeof(ParticleVertex), m_ParticlesInExistence * sizeof(ParticleVertex));
			m_PropsStorage->Update(&m_Data[i], sizeof(ParticleData), m_ParticlesInExistence * sizeof(ParticleData));

			m_ParticlesInExistence++;
		}
		else
			XYZ_LOG_WARN("Reached maximum number of particles");
	}

}