#include "stdafx.h"
#include "ParticleEffect2D.h"
#include "XYZ/Renderer/Renderer2D.h"
#include "XYZ/Renderer/RenderCommand.h"
#include "XYZ/Renderer/MaterialManager.h"

namespace XYZ {

	void RenderInstanced(std::shared_ptr<VertexArray> vao,std::shared_ptr<IndirectBuffer> ibo)
	{
		vao->Bind();
		ibo->Bind();
		RenderCommand::DrawElementsIndirect(nullptr);
	}


	ParticleEffect2D::ParticleEffect2D(uint32_t maxParticles, std::shared_ptr<Material> material, std::shared_ptr<Material> renderMaterial)
		: 
		m_Material(material),
		m_Renderable{
			VertexArray::Create(),
			IndirectBuffer::Create(nullptr, sizeof(DrawElementsIndirectCommand)),
			renderMaterial
		},
		m_Counter(AtomicCounter::Create(1))
	{
		m_Shader = m_Material->GetShader();
	
		m_Material->Set("u_Speed", 1.0f);
		m_Material->Set("u_Gravity", -2.8f);
		m_Material->Set("u_Collider", glm::vec4(5, 0, 10, 5));
		m_Material->Set("u_Loop", (int)m_Loop);
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

		m_PropsStorage = ShaderStorageBuffer::Create((uint32_t)m_MaxParticles * (uint32_t)sizeof(ParticleInformation));
	
		m_Renderable.VertexArray->AddShaderStorageBuffer(m_VertexStorage);


		float vert[20] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<XYZ::VertexBuffer> squareVBpar;
		squareVBpar = XYZ::VertexBuffer::Create(vert, sizeof(vert));
		squareVBpar->SetLayout({
			{  0, XYZ::ShaderDataType::Float3, "a_Position" },
			{  1, XYZ::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_Renderable.VertexArray->AddVertexBuffer(squareVBpar);

		uint32_t squareIndpar[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<XYZ::IndexBuffer> squareIBpar;
		squareIBpar = XYZ::IndexBuffer::Create(squareIndpar, sizeof(squareIndpar) / sizeof(uint32_t));
		m_Renderable.VertexArray->SetIndexBuffer(squareIBpar);

		
		m_Counter->BindBase(COUNTER_BINDING);
		m_Renderable.IndirectBuffer->BindBase(INDIRECT_BINDING);
		m_PropsStorage->BindBase(PROPS_BINDING);
		m_VertexStorage->BindBase(VERTEX_BINDING);
	}

	ParticleEffect2D::~ParticleEffect2D()
	{
	}


	void ParticleEffect2D::Render()
	{
		m_Renderable.Material->Bind();
		Command<std::shared_ptr<VertexArray>,std::shared_ptr<IndirectBuffer>>cmd(RenderInstanced, m_Renderable.VertexArray, m_Renderable.IndirectBuffer);
		Renderer2D::Submit(cmd, sizeof(cmd));		
	}

	void ParticleEffect2D::Update(float dt)
	{
		// Emission	
		float raise = m_Emitter.Rate * (m_Emitter.EmitDuration * dt);
		int emittedParticles = (int)floor(m_Emitter.EmittedParticles);
		if (m_Emitter.EmittedParticles + raise <= m_MaxParticles)
		{
			m_Emitter.EmittedParticles += raise;
			emittedParticles = (int)floor(m_Emitter.EmittedParticles);	
			m_Material->Set("u_ParticlesInExistence", emittedParticles);
		}

		m_Material->Bind();

		m_PropsStorage->BindRange(0,  emittedParticles * sizeof(ParticleInformation), PROPS_BINDING);
		m_VertexStorage->BindRange(0, emittedParticles * sizeof(ParticleVertex), VERTEX_BINDING);
		m_Material->GetShader()->Compute(32, 32, 1);
		m_PlayTime += dt;
	}

	void ParticleEffect2D::SetParticles(ParticleVertex* vertexBuffer, ParticleInformation* particleInfo)
	{
		m_VertexStorage->Update(vertexBuffer, m_MaxParticles * sizeof(ParticleVertex), 0);
		m_PropsStorage->Update(particleInfo, m_MaxParticles * sizeof(ParticleInformation), 0);
	}

	void ParticleEffect2D::SetParticlesRange(ParticleVertex* vertexBuffer, ParticleInformation* particleInfo, uint32_t offset, uint32_t count)
	{
		XYZ_ASSERT(offset + count <= m_MaxParticles, "Attempting to set particles out of range");
		m_VertexStorage->Update(vertexBuffer, count * sizeof(ParticleVertex), offset * sizeof(ParticleVertex));
		m_PropsStorage->Update(particleInfo, count * sizeof(ParticleInformation), offset * sizeof(ParticleVertex));
	}

	uint32_t ParticleEffect2D::GetParticles(ParticleVertex* vertexBuffer, ParticleInformation* particleInfo)
	{
		m_VertexStorage->GetSubData(vertexBuffer, m_MaxParticles * sizeof(ParticleVertex), 0);
		m_PropsStorage->GetSubData(particleInfo, m_MaxParticles * sizeof(ParticleInformation), 0);
		
		return m_MaxParticles;
	}

	uint32_t ParticleEffect2D::GetParticlesRange(ParticleVertex* vertexBuffer, ParticleInformation* particleInfo, uint32_t offset, uint32_t count)
	{
		XYZ_ASSERT(offset + count <= m_MaxParticles, "Attempting to get particles out of range");
		m_VertexStorage->GetSubData(vertexBuffer, count * sizeof(ParticleVertex), offset * sizeof(ParticleVertex));
		m_PropsStorage->GetSubData(particleInfo, count * sizeof(ParticleInformation), offset * sizeof(ParticleVertex));

		return count + offset;
	}

}